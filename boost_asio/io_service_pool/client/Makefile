all: proto demo_client

SOURCES=$(notdir $(shell echo *.cpp hy_dynamic_dmp.pb.cc))
OBJECTS=$(addprefix ./, $(SOURCES:.CPP=.O))

proto:
	protoc --proto_path=../include --cpp_out=. ../include/hy_dynamic_dmp.proto

demo_client: $(OBJECTS)
	g++ -g -I/user/local/boost/include -o demo_client $(OBJECTS) -L/user/local/lib -lprotobuf -lboost_system

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.cc.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm demo_client  *.pb.cc *.pb.h
