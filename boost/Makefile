
all: target boost_random

shuffle:
	g++ -o shuffle shuffle.cpp -std=c++11

target:
	g++ -o ptime ptime.cpp -lboost_date_time

boost_random: boost_random.cpp
	g++ -o boost_random boost_random.cpp

clean:
	rm -rf *.o ptime boost_random shuffle
