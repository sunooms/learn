all: protoc async_server_pool  

protoc:
	protoc --proto_path=../include --cpp_out=. ../include/hy_dynamic_dmp.proto

async_server_pool: protoc io_service_pool_server.cpp dlib_flow.cpp hy_dynamic_dmp.pb.cc
	g++ -g -Wall -o async_server_pool io_service_pool_server.cpp dlib_flow.cpp hy_dynamic_dmp.pb.cc -lprotobuf -lboost_date_time -lboost_system -lboost_thread



clean:
	rm -f *.o async_server_pool  hy_dynamic_dmp.pb.h hy_dynamic_dmp.pb.cc
