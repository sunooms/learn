#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>
#include <boost/date_time/local_time_adjustor.hpp>
#include <boost/date_time/c_local_time_adjustor.hpp>


using namespace std;
using namespace boost::posix_time;
using namespace boost::gregorian;
using namespace boost;

// here use c_local_adjustor for across the day
long get_past_days(time_t balance_date)
{
    using namespace boost::posix_time;
    using namespace boost::gregorian;
    using boost::date_time::c_local_adjustor;
    using boost::posix_time::from_time_t;

    ptime pt_now( second_clock::local_time() );
    ptime pt_post = c_local_adjustor<ptime>::utc_to_local(from_time_t(balance_date));

    return boost::lexical_cast<long>(pt_now.date() - pt_post.date());
}

int main()
{
    long dis_days = 0;
    ptime ptNow( second_clock::local_time() );
    ptime ptpost = from_time_t(1478405472);
    ptime p1=second_clock::local_time();
    std::cout << to_simple_string( ptNow ) << std::endl;
    std::cout << to_simple_string( p1 ) << std::endl;
    std::cout << to_simple_string( ptpost ) << std::endl;

    dis_days = boost::lexical_cast<long>(ptNow.date() - ptpost.date());

    printf("now dis_days :%ld\n", dis_days);

    ptime ptpost2 = from_time_t(1481046648);
    ptime ptpost3 = from_time_t(1481056648);

    dis_days = boost::lexical_cast<long>(ptpost2.date() - ptpost3.date());
    printf("now dis_days :%ld\n", dis_days);


    date d1 = day_clock::local_day();
    date d2 = d1 -days(2);

    // print YYYMMDD
    std::string strTime =  boost::gregorian::to_iso_string(boost::gregorian::day_clock::local_day());
    cout << strTime.c_str() << endl;

    std::string strTime2 =  boost::gregorian::to_iso_string(d2);
    cout << strTime2.c_str() << endl;

    // print yyymmdd-HH:MM:SS
    std::string strTime3 = boost::posix_time::to_iso_string(boost::posix_time::second_clock::local_time());
    // 这时候strTime里存放时间的格式是YYYYMMDDTHHMMSS，日期和时间用大写字母T隔开了
    int pos = strTime3.find('T');
    strTime3.replace(pos,1,std::string("-"));
    strTime3.replace(pos + 3,0,std::string(":"));
    strTime3.replace(pos + 6,0,std::string(":"));

    std::cout << strTime3.c_str() << std::endl;
    return 0;
}
