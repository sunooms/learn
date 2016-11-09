#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/algorithm/string.hpp>


using namespace boost::posix_time;
using namespace boost::gregorian;

int main()
{
    long days = 0;
    ptime ptNow( second_clock::local_time() );
    ptime ptpost = from_time_t(1478405472);
    ptime p1=second_clock::local_time();
    std::cout << to_simple_string( ptNow ) << std::endl;
    std::cout << to_simple_string( p1 ) << std::endl;
    std::cout << to_simple_string( ptpost ) << std::endl;

    days = boost::lexical_cast<long>(ptNow.date() - ptpost.date());

    printf("now days :%ld\n", days);
    return 0;
}
