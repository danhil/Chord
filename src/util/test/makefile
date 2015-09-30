CC = /usr/bin/g++
CC_OPTIONS = -I. -I../include
CCFLAGS=-std=c++11

#if we passed ARCH=osx
ifeq ($(ARCH),osx)
	@echo Building for OSX ( ARCH=osx )
	CC_OPTIONS = -DOSX=1
endif

#if we passed ARCH=linux
ifeq ($(ARCH),linux)
	@echo Building for LINUX ( ARCH=linux )
	LNK_OPTIONS = -lpthread -lrt -ldl
endif

all: testdebug.out testthread.out

testdebug.out: DebugTest.cpp
	$(CC) $(CCFLAGS) $(CC_OPTIONS) $(LNK_OPTIONS) DebugTest.cpp -o testdebug.out

testthread.out: ThreadTest.cpp
	$(CC) $(CCFLAGS) $(CC_OPTIONS) $(LNK_OPTIONS) ThreadTest.cpp -o testthread.out



clean:
	rm *.out
