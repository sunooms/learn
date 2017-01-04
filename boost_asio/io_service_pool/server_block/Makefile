all: proto demo_server

SOURCES=$(notdir $(shell echo *.cpp hy_dynamic_dmp.pb.cc))
OBJECTS=$(addprefix ./, $(SOURCES:.CPP=.O))

proto:
	protoc --proto_path=../include --cpp_out=. ../include/hy_dynamic_dmp.proto

demo_server:proto $(OBJECTS)
	g++ -g -I/user/local/boost/include -o demo_server $(OBJECTS) -L/user/local/lib -lprotobuf -lboost_thread 

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.cc.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	-rm demo_server *.pb.cc *.pb.h
