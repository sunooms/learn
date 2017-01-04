//
// blocking_tcp_echo_client.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2016 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>
#include "./hy_dynamic_dmp.pb.h"
#include "../include/dynamic_message_protocol.h"

using boost::asio::ip::tcp;
using namespace hy_dynamic_dmp;

enum { max_length = 10240 };

int parse_proto_response(char *data, int datalen)
{
    hyDMPResponse res;
    res.ParseFromArray(data, datalen);

    for(int i=0; i<res.mtad_size(); i++)
    {
        const MatAd &mtad = res.mtad(i);
        printf("now return matid:%s adid:%s adspace:%s\n", mtad.matid().c_str(), mtad.adid().c_str(), mtad.adspaceid().c_str());
    }
    return 0;
}

int build_proto_body(std::string &reqdata)
{
    hyDMPRequest req;

    req.set_id("test111");
    req.set_media("sohu");

    Ad  *ad1 = req.add_ad();
    Mat *mt11 = ad1->add_mat();
    mt11->set_id("mt1_1");
    Mat *mt12 = ad1->add_mat();
    mt12->set_id("mt1_2");
    Mat *mt13 = ad1->add_mat();
    mt13->set_id("mt1_3");
    ad1->set_id("ad1");
    ad1->set_adspaceid("adspace1");

    Ad  *ad2 = req.add_ad();
    Mat *mt21 = ad2->add_mat();
    mt21->set_id("mt2_1");
    Mat *mt22 = ad2->add_mat();
    mt22->set_id("mt2_2");
    ad2->set_id("ad2");
    ad2->set_adspaceid("adspace2");



    req.SerializeToString(&reqdata);
    return 0;
}

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

int main(int argc, char* argv[])
{
  try
  {
    if (argc != 3)
    {
      std::cerr << "Usage: blocking_tcp_echo_client <host> <port>\n";
      return 1;
    }

    boost::asio::io_service io_service;

    tcp::resolver resolver(io_service);
    tcp::resolver::query query(tcp::v4(), argv[1], argv[2]);
    tcp::resolver::iterator iterator = resolver.resolve(query);

    tcp::socket s(io_service);
    boost::asio::connect(s, iterator);

    using namespace std; // For strlen.

    std::string sendmsg;
    std::string reqdata;
    boost::system::error_code error;

    // here send request
    build_proto_body(reqdata);
    build_package(reqdata, sendmsg);
    boost::asio::write(s, boost::asio::buffer(sendmsg));

    // here receive response
    //read package header
    char reply[max_length];
    boost::asio::read(s, boost::asio::buffer(reply, sizeof(HyDynamicDmpHeader)), error );
    if (error == boost::asio::error::eof)
        return 0; // Connection closed cleanly by peer.
    else if(error){
        throw boost::system::system_error(error);
    }

    HyDynamicDmpHeader *resHead = (HyDynamicDmpHeader *)reply;
    HyDynamicDmpHeader::net2host(resHead);
    printf("now res len:%d\n", resHead->wLength);

    int bodylen = resHead->wLength - sizeof(HyDynamicDmpHeader);
    //read package body
    boost::asio::read(s, boost::asio::buffer(reply, bodylen), error );
    parse_proto_response(reply, bodylen);
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
