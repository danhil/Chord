#
# Macros
#

SHELL := /bin/bash

CC = /usr/bin/g++
LNK_OPTIONS =
CCFLAGS =
OBJECTDIR = ../object

#if we passed ARCH=osx
ifeq ($(ARCH),OSX)
	#@echo Building for OSX ( ARCH=osx )
	CC_OPTIONS = -DOSX=1
endif

#if we passed ARCH=linux
ifeq ($(ARCH),NIX)
	#@echo Building for LINUX ( ARCH=linux )
	LNK_OPTIONS = -lpthread -lrt -ldl
endif

HEADER = $(shell for file in `find . -name '*'.h`;do echo $$file; done)

INCLUDE = -I.\

all: PREBUILD LIBUTIL libutil.so POSTBUILD

PREBUILD :
	mkdir -p $(OBJECTDIR)

POSTBUILD :
	mkdir -p include
	rsync -a -v $(HEADER) include

LIBUTIL : \
		$(OBJECTDIR)/ThreadUtil.o\


# actualy archive the static library with ar.
libutil.so:
	@echo Archiving libutil...
	rm -f libutil.so
	ar -cvq libutil.so $(OBJECTDIR)/*.o

clean :
		rm -r -f $(OBJECTDIR)
		rm -r -f ./include
		rm -f libutil.so

install : libutil
	@echo --= HEADERS =--
	@echo $(HEADER)
	@echo --= CLEANING OLD HEADERS =--
	mkdir -p /usr/include/libutil
	@echo --= =--
	cp $(HEADER) /usr/include/libutil

#
# Build the parts of utility library
#

# Item # 1 --Thread support--
$(OBJECTDIR)/ThreadUtil.o : ./ThreadUtil.cpp
	$(CC) $(CCFLAGS) $(CC_OPTIONS) ./ThreadUtil.cpp -c $(INCLUDE) -o $(OBJECTDIR)/ThreadUtil.o

##### END RUN ####

