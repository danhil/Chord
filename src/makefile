#
# Macros
#

SHELL := /bin/bash

CC = /usr/bin/g++
LNK_OPTIONS =
CCFLAGS = -v
OBJECTDIR = ./object

CC_OPTIONS = -DOSX=1

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

HEADER = $(shell for file in `find . -name *.h`;do echo $$file; done)

#
# Included headers in chordlib

INCLUDE = -I.\
		-Ip2p\
		-Iutil\
		-Iutil/mongoose_http_server\
		-Iutil/mongoose_http_server/callbacks\
		-Iutil/crypto\

all: PREBUILD LIBCHORD libchord.a POSTBUILD

PREBUILD :
	mkdir -p $(OBJECTDIR)

POSTBUILD :
	mkdir -p include
	cp -p $(HEADER) include

LIBCHORD : \
		$(OBJECTDIR)/HTTP_Client.o\
		$(OBJECTDIR)/sha1.o\
		$(OBJECTDIR)/callbacks.o\
		$(OBJECTDIR)/chord_callbacks.o\
		$(OBJECTDIR)/mongoose.o\
		$(OBJECTDIR)/Thread.o\
		$(OBJECTDIR)/Node.o\
		$(OBJECTDIR)/ChordOverlay.o\
		$(OBJECTDIR)/ChordNode.o\
		$(OBJECTDIR)/SingletonChord.o\
		$(OBJECTDIR)/http_operations.o\
		$(OBJECTDIR)/TransportHTTP.o\
		$(OBJECTDIR)/Request.o


# Create a static library of chord
libchord.a:
	@echo Archiving chordlib...
	rm -f libchord.a
	ar -cvq libchord.a $(OBJECTDIR)/*.o

clean :
		rm -r -v $(OBJECTDIR)
		rm -r -v ./include
		rm libchord.a

install : chordlib
	@echo --= HEADERS =--
	@echo $(HEADER)
	@echo --= CLEANING OLD HEADERS =--
	mkdir -p /usr/include/chord
	@echo --=  =--
	cp $(HEADER) /usr/include/chord

#
# Build the parts of chordlib
#

# Item # 2 -- HTTP_Client --
$(OBJECTDIR)/HTTP_Client.o : util/HTTP_Client.cpp
	$(CC) $(CCFLAGS) $(CC_OPTIONS) util/HTTP_Client.cpp -c $(INCLUDE) -o $(OBJECTDIR)/HTTP_Client.o


## Item # 3 -- crypt_argchk --
#./crypt_argchk.o : engine/helper/crypto/tomcrypt/algorithms/crypt_argchk.c
#	$(CC) $(CCFLAGS) $(CC_OPTIONS) engine/helper/crypto/tomcrypt/algorithms/crypt_argchk.c -c $(INCLUDE) -o $(OBJECTDIR)/crypt_argchk.o
#
## Item # 4 -- sha1 --
#./sha1.o : engine/helper/crypto/tomcrypt/algorithms/sha1.c
#	$(CC) $(CCFLAGS) $(CC_OPTIONS) engine/helper/crypto/tomcrypt/algorithms/sha1.c -c $(INCLUDE) -o $(OBJECTDIR)/sha1.o
#
## Item # 5 -- md5 --
#./md5.o : engine/helper/crypto/tomcrypt/algorithms/md5.c
#	$(CC) $(CCFLAGS) $(CC_OPTIONS) engine/helper/crypto/tomcrypt/algorithms/md5.c -c $(INCLUDE) -o $(OBJECTDIR)/md5.o

# Item # 5 -- sha1 --
$(OBJECTDIR)/sha1.o: util/crypto/sha1.cpp
	$(CC) $(CCFLAGS) $(CC_OPTIONS) util/crypto/sha1.cpp -c $(INCLUDE) -o $(OBJECTDIR)/sha1.o

# Item # 6 -- callbacks --
$(OBJECTDIR)/callbacks.o : util/mongoose_http_server/callbacks/callbacks.cpp
	$(CC) $(CCFLAGS) $(CC_OPTIONS) util/mongoose_http_server/callbacks/callbacks.cpp -c $(INCLUDE) -o $(OBJECTDIR)/callbacks.o

# Item # 8 -- chord_callbacks --
$(OBJECTDIR)/chord_callbacks.o : util/mongoose_http_server/callbacks/chord_callbacks.cpp
	$(CC) $(CCFLAGS) $(CC_OPTIONS) util/mongoose_http_server/callbacks/chord_callbacks.cpp -c $(INCLUDE) -o $(OBJECTDIR)/chord_callbacks.o


# Item # 9 -- mongoose --
$(OBJECTDIR)/mongoose.o : util/mongoose_http_server/mongoose.cpp
	$(CC) $(CCFLAGS) $(CC_OPTIONS) util/mongoose_http_server/mongoose.cpp -c $(INCLUDE) -o $(OBJECTDIR)/mongoose.o


# Item # 10 -- Thread --
$(OBJECTDIR)/Thread.o : util/ThreadUtil.cpp
	$(CC) $(CCFLAGS) $(CC_OPTIONS) util/ThreadUtil.cpp -c $(INCLUDE) -o $(OBJECTDIR)/Thread.o


# Item # 11 -- Node --
$(OBJECTDIR)/Node.o : p2p/Node.cpp
	$(CC) $(CCFLAGS) $(CC_OPTIONS) p2p/Node.cpp -c $(INCLUDE) -o $(OBJECTDIR)/Node.o


# Item # 12 -- ChordOverlay --
$(OBJECTDIR)/ChordOverlay.o : p2p/ChordOverlay.cpp
	$(CC) $(CCFLAGS) $(CC_OPTIONS) p2p/ChordOverlay.cpp -c $(INCLUDE) -o $(OBJECTDIR)/ChordOverlay.o


# Item # 13 -- ChordNode --
$(OBJECTDIR)/ChordNode.o : p2p/ChordNode.cpp
	$(CC) $(CCFLAGS) $(CC_OPTIONS) p2p/ChordNode.cpp -c $(INCLUDE) -o $(OBJECTDIR)/ChordNode.o


# Item # 14 -- SingletonChord --
$(OBJECTDIR)/SingletonChord.o : p2p/SingletonChord.cpp
	$(CC) $(CCFLAGS) $(CC_OPTIONS) p2p/SingletonChord.cpp -c $(INCLUDE) -o $(OBJECTDIR)/SingletonChord.o


# Item # 15 -- http_operations --
$(OBJECTDIR)/http_operations.o : p2p/http_operations.cpp
	$(CC) $(CCFLAGS) $(CC_OPTIONS) p2p/http_operations.cpp -c $(INCLUDE) -o $(OBJECTDIR)/http_operations.o


# Item # 16 -- TransportHTTP --
$(OBJECTDIR)/TransportHTTP.o : p2p/TransportHTTP.cpp
	$(CC) $(CCFLAGS) $(CC_OPTIONS) p2p/TransportHTTP.cpp -c $(INCLUDE) -o $(OBJECTDIR)/TransportHTTP.o

# Item # 17 -- TransportHTTP --
$(OBJECTDIR)/Request.o : p2p/Request.cpp
	$(CC) $(CCFLAGS) $(CC_OPTIONS) p2p/Request.cpp -c $(INCLUDE) -o $(OBJECTDIR)/Request.o


##### END RUN ####
