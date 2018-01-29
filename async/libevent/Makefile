all: async_time_test async_hello

async_time_test: time_test.o
	gcc -o async_time_test time_test.c -levent

async_hello: hello_world.o
	gcc -o async_hello hello_world.c -levent

clean:
	rm *.o async_time_test async_hello
