all:  sync_client async_daytime_server sync_daytime_server

sync_client:sync_tcp_daytime_client.cpp
	g++ -g -Wall -o sync_client sync_tcp_daytime_client.cpp -lboost_date_time -lboost_system

async_daytime_server:async_tcp_daytime_server.cpp
	g++ -g -Wall -o async_daytime_server async_tcp_daytime_server.cpp -lboost_date_time -lboost_system

sync_daytime_server:sync_tcp_daytime_server.cpp
	g++ -g -Wall -o sync_daytime_server sync_tcp_daytime_server.cpp -lboost_date_time -lboost_system


clean:
	rm -f *.o sync_client async_daytime_server sync_daytime_server
