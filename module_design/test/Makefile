all: test

SRC: main.cpp \
	./core/session.cpp \
	module_manager.cpp \
	flow.cpp \
	ad_select.cpp

%.o:%.cpp
	g++ -g -Wall -std=c++11 -c $<

test:
	g++ -g -Wall -std=c++11 -I. -I../ main.cpp ../core/session.cpp ../driver/module_manager.cpp ../interface/flow.cpp ../module/flow_component/ad_select.cpp ../driver/dispatcher.cpp ../driver/mod_config_helper.cpp ../core/dsp_session.cpp ../module/flow/sohu_flow.cpp ../module/flow/youku_flow.cpp

clean:
	rm -rf *.o test a.out
