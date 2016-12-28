all: async_timer sync_timer async_timer_argu async_timer_memfunc


async_timer:using_a_timer_asynchronously.cpp
	g++ -g -Wall -o async_timer using_a_timer_asynchronously.cpp -lboost_date_time -lboost_system

sync_timer:using_a_timer_synchronously.cpp
	g++ -g -Wall -o sync_timer using_a_timer_synchronously.cpp -lboost_date_time -lboost_system

async_timer_argu:using_a_timer_asynchronously_with_arguments.cpp
	g++ -g -Wall -o async_timer_argu using_a_timer_asynchronously_with_arguments.cpp -lboost_date_time -lboost_system

async_timer_memfunc:using_a_timer_asynchronously_with_memfunction.cpp
	g++ -g -Wall -o async_timer_memfunc using_a_timer_asynchronously_with_memfunction.cpp -lboost_date_time -lboost_system

clean:
	rm -f *.o async_timer sync_timer async_timer_argu async_timer_memfunc
