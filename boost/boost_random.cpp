#include <boost/random.hpp>
#include <boost/random.hpp>
#include <iostream>
#include <time.h>

int main()
{
    //boost::mt19937 gen(time(0));
    boost::mt19937 gen;
    boost::uniform_int<> dist(1,30);
    //boost::variate_generator<boost::mt19937 &, boost::uniform_int<> > die(gen,dist);
    boost::variate_generator<boost::mt19937, boost::uniform_int<> > die(boost::mt19937(time(0)), dist);


    for( int i=0; i<30; ++i ) {
        std::cout<<die() <<std::endl;

    }

}
