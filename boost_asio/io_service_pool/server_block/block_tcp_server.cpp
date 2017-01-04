//
// blocking_tcp_echo_server.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2016 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include "../include/dynamic_message_protocol.h"
#include "./hy_dynamic_dmp.pb.h"

using boost::asio::ip::tcp;
using namespace hy_dynamic_dmp;

const int max_length = 10240;

typedef boost::shared_ptr<tcp::socket> socket_ptr;

int build_package(std::string &data, std::string &sendmsg)
{
    HyDynamicDmpHeader reqhead;

    reqhead.wLength = data.length() + sizeof(reqhead);

    printf("now package length:%d\n", reqhead.wLength);

    HyDynamicDmpHeader::host2net(&reqhead);

    sendmsg.assign((const char *)&reqhead, sizeof(reqhead));
    sendmsg += data;

    return 0;
}

int proto_parse_and_return(char *data, int datalen, std::string &retmsg)
{
    hyDMPRequest  req;
    hyDMPResponse res;
    req.ParseFromArray(data, datalen);
    printf("now req id:%s\n", req.id().c_str());
    printf("now req media:%s\n", req.media().c_str());

    res.set_id(req.id());

    for(int i=0; i<req.ad_size(); i++)
    {
        const Ad &ad = req.ad(i);

        for(int j=0; j<ad.mat_size(); j++)
        {
            const Mat &mat = ad.mat(j);

            MatAd *tmp_matad = res.add_mtad();
            tmp_matad->set_matid(mat.id());
            tmp_matad->set_adid(ad.id());
            tmp_matad->set_adspaceid(ad.adspaceid());
        }
    }

    std::string ret_data;
    res.SerializeToString(&ret_data);

    build_package(ret_data, retmsg);


    return 0;
}

void session(socket_ptr sock)
{
  try
  {
    for (;;)
    {
      char data[max_length];
      char test[]="haha, xgm test!";

      boost::system::error_code error;
      //size_t length = sock->read_some(boost::asio::buffer(data), error);
      //if (error == boost::asio::error::eof)
      //  break; // Connection closed cleanly by peer.
      //else if (error)
      //  throw boost::system::system_error(error); // Some other error.

      //boost::asio::write(*sock, boost::asio::buffer(data, length));

      //read package header
      boost::asio::read(*sock, boost::asio::buffer(data, sizeof(HyDynamicDmpHeader)), error );
      if (error == boost::asio::error::eof)
        break; // Connection closed cleanly by peer.
      else if(error){
          throw boost::system::system_error(error);
      }

      HyDynamicDmpHeader *reqHead = (HyDynamicDmpHeader *)data;
      HyDynamicDmpHeader::net2host(reqHead);
      printf("now req len:%d\n", reqHead->wLength);

      int bodylen = reqHead->wLength - sizeof(HyDynamicDmpHeader);
      //read package body
      boost::asio::read(*sock, boost::asio::buffer(data, bodylen), error );
      std::string retmsg;
      proto_parse_and_return(data, bodylen, retmsg);

      boost::asio::write(*sock, boost::asio::buffer(retmsg));

    }

  }
  catch (std::exception& e)
  {
    std::cerr << "Exception in thread: " << e.what() << "\n";
  }
}

void server(boost::asio::io_service& io_service, unsigned short port)
{
  tcp::acceptor a(io_service, tcp::endpoint(tcp::v4(), port));
  for (;;)
  {
    socket_ptr sock(new tcp::socket(io_service));
    a.accept(*sock);
    boost::thread t(boost::bind(session, sock));
  }
}

int main(int argc, char* argv[])
{
  try
  {
    if (argc != 2)
    {
      std::cerr << "Usage: blocking_tcp_echo_server <port>\n";
      return 1;
    }

    boost::asio::io_service io_service;

    using namespace std; // For atoi.
    server(io_service, atoi(argv[1]));
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
