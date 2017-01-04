#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <boost/thread.hpp>
#include <boost/aligned_storage.hpp>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>

#include "../include/package_message.h"
#include "dlib_flow.h"

using boost::asio::ip::tcp;

//http://blog.csdn.net/huang_xw/article/details/8475050

// a pool of io_service objects.
class io_service_pool :private boost::noncopyable
{
    public:
        // construct the io_service pool
        explicit io_service_pool(std::size_t pool_size) : next_io_service_(0)
    {
        if(pool_size == 0){
            throw std::runtime_error("io_service_pool size is 0");
        }

        // give all the io_sercies work to do so that their run() functions will not
        //   exit until they are explicitly stopped
        for(std::size_t i = 0; i < pool_size; ++i)
        {
            io_service_ptr io_service(new boost::asio::io_service);
            work_ptr work(new boost::asio::io_service::work(*io_service));
            io_services_.push_back(io_service);
            work_.push_back(work);
        }

    }

        // run all io_service objects in the pool
        void run()
        {
            // create a pool of threads to run all of the io_services
            std::vector<boost::shared_ptr<boost::thread> > threads;
            for(std::size_t i=0; i < io_services_.size(); ++i)
            {
                boost::shared_ptr<boost::thread> thread(new boost::thread(
                            boost::bind(&boost::asio::io_service::run, io_services_[i])));
                threads.push_back(thread);
            }

            // wait for all threads in the pool to exit
            for(std::size_t i=0; i< threads.size(); ++i) {
                threads[i]->join();
            }
        }

        // stop all io_service objects in the pool
        void stop()
        {
            // explicitly stop all io_services
            for(std::size_t i=0; i< io_services_.size(); ++i) {
                io_services_[i]->stop();
            }
        }

        // get an io_service to use
        boost::asio::io_service& get_io_service()
        {
            // use a round-robin scheme to choose the next io_service to use.
            boost::asio::io_service& io_service = *io_services_[next_io_service_];
            ++next_io_service_;
            if(next_io_service_ ==  io_services_.size()){
                next_io_service_ = 0;
            }

            return io_service;
        }

    private:
        typedef boost::shared_ptr<boost::asio::io_service> io_service_ptr;
        typedef boost::shared_ptr<boost::asio::io_service::work> work_ptr;

        // the pool of io_service
        std::vector<io_service_ptr> io_services_;

        // the work that keeps the io_services running
        std::vector<work_ptr> work_;

        // the next io_service to use for a connection
        std::size_t next_io_service_;
};

class session : public boost::enable_shared_from_this<session>
{
    public:
        session(boost::asio::io_service& io_service) :socket_(io_service)
    {
    }

        tcp::socket& socket()
        {
            return socket_;
        }

        void start()
        {
            boost::asio::async_read(socket_, boost::asio::buffer(read_msg_.data(), package_message::header_length),
                    boost::bind(&session::handle_read_header, this, boost::asio::placeholders::error));

        }

        void handle_read_header(const boost::system::error_code& error)
        {
            if(!error && read_msg_.decode_header())
            {
                printf("now headlen:%d bodylen:%ld\n", package_message::header_length, read_msg_.body_length());
                boost::asio::async_read(socket_, boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
                    boost::bind(&session::handle_read_body, this, boost::asio::placeholders::error));
            }
            else{
                delete this;
            }
        }

        void handle_read_body(const boost::system::error_code& error)
        {
            if(!error)
            {
                dlib_flow flow(read_msg_);
                if( flow.dynamic_process(write_msg_))
                {
                    boost::asio::async_write(socket_, boost::asio::buffer(write_msg_.data(), write_msg_.length()),
                            boost::bind(&session::handle_write, this, boost::asio::placeholders::error));
                }else{
                    delete this;
                }
            }else{
                delete this;
            }
        }

        void handle_write(const boost::system::error_code& error)
        {
            if(!error)
            {
                //read_msg_.clear();
                //write_msg_.clear();
                //boost::asio::async_read(socket_, boost::asio::buffer(read_msg_.data(), package_message::header_length),
                //    boost::bind(&session::handle_read_header, this, boost::asio::placeholders::error));
            }
            else{
                delete this;
            }
        }


    private:

        // the socket used to communicate with the client
        tcp::socket socket_;

        package_message read_msg_;
        package_message write_msg_;

};

typedef boost::shared_ptr<session> session_ptr;

class server
{
    public:
        server(short port, std::size_t io_service_pool_size)
            : io_service_pool_(io_service_pool_size),
              acceptor_(io_service_pool_.get_io_service(), tcp::endpoint(tcp::v4(), port))
    {
        session*  new_session = new session(io_service_pool_.get_io_service());
        acceptor_.async_accept(new_session->socket(),
                boost::bind(&server::handle_accept, this, new_session, boost::asio::placeholders::error));

    }

        void handle_accept(session* new_session, const boost::system::error_code& error)
        {
            if(!error)
            {
                new_session->start();
                new_session = new session(io_service_pool_.get_io_service());
                acceptor_.async_accept(new_session->socket(),
                    boost::bind(&server::handle_accept, this, new_session, boost::asio::placeholders::error));
            }else{
                delete new_session;
            }
        }

        void run()
        {
            io_thread_.reset(new boost::thread(boost::bind(&io_service_pool::run,
                            &io_service_pool_)));
        }

        void stop()
        {
            io_service_pool_.stop();
            io_thread_->join();
        }

    private:
        boost::shared_ptr<boost::thread> io_thread_;
        io_service_pool io_service_pool_;
        tcp::acceptor acceptor_;

};

int main(int argc, char **argv)
{
    try
    {
        if(argc != 2)
        {
            std::cerr << "Usage: server <port>\n";
            return 1;
        }

        using namespace std;  // for atoi
        server s(atoi(argv[1]), 10);

        s.run();

        getchar();

        s.stop();
    }
    catch(std::exception& e)
    {
        std::cerr << "Exceptin: " << e.what() << "/n";
    }

    return 0;
}
