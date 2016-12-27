#include <iostream>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

int main()
{
    boost::asio::io_service io;

    //boost::asio::deadline_timer t(io, boost::posix_time::second(2));
    boost::asio::deadline_timer t(io, boost::posix_time::milliseconds(600));
    t.wait();

    std::cout << "Hello, world, xgm!" << std::endl;

    return 0;
}
