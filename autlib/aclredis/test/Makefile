CFLAGS = -c -g -W -O3 -Wall -Werror -nostartfiles -Wshadow \
		 -Wno-long-long -Wpointer-arith -D_REENTRANT \
		 -D_POSIX_PTHREAD_SEMANTICS -DLINUX2 \
		 -I ../include
BASE_PATH=../
LDFLAGS = -L$(BASE_PATH)/lib -lacl_cpp \
		  -L$(BASE_PATH)/lib -lprotocol \
		  -L$(BASE_PATH)/lib -lacl \
		  -lpthread
test: main.o
		g++ -o test main.o $(LDFLAGS)
main.o: main.cpp
		g++ $(CFLAGS) main.cpp -o main.o

clean:
	rm -rf main.o
