ZLIB_VERSION=1.2.3
BZIP2_VERSION=1.0.6
MINIUPNPC_VERSION=1.3
OPENSSL_VERSION=1.0.2h
SPEEX_VERSION=1.2rc2
SPEEXDSP_VERSION=1.2rc3
LIBXML2_VERSION=2.9.3
LIBXSLT_VERSION=1.1.28
CURL_VERSION=7.46.0
LIBSSH_VERSION=0.5.4
PROTOBUF_VERSION=2.4.1

MAKEFILE_PATH=$(dir $(MAKEFILE_LIST))
LIBS_PATH?=$(MAKEFILE_PATH)../../../../libs

DOWNLOAD_PATH?=download
COPY_ANSWER?=

all: dirs zlib bzip2 miniupnpc openssl speex speexdsp curl libxml2 libxslt libssh protobuf copylibs

download: \
	$(DOWNLOAD_PATH)/zlib-$(ZLIB_VERSION).tar.gz \
	$(DOWNLOAD_PATH)/bzip2-$(BZIP2_VERSION).tar.gz \
	$(DOWNLOAD_PATH)/miniupnpc-$(MINIUPNPC_VERSION).tar.gz \
	$(DOWNLOAD_PATH)/openssl-$(OPENSSL_VERSION).tar.gz \
	$(DOWNLOAD_PATH)/speex-$(SPEEX_VERSION).tar.gz \
	$(DOWNLOAD_PATH)/speexdsp-$(SPEEXDSP_VERSION).tar.gz \
	$(DOWNLOAD_PATH)/libxml2-$(LIBXML2_VERSION).tar.gz \
	$(DOWNLOAD_PATH)/libxslt-$(LIBXSLT_VERSION).tar.gz \
	$(DOWNLOAD_PATH)/curl-$(CURL_VERSION).tar.gz \
	$(DOWNLOAD_PATH)/libssh-$(LIBSSH_VERSION).tar.gz \
	$(DOWNLOAD_PATH)/protobuf-$(PROTOBUF_VERSION).tar.gz

clean:
	rm -r -f libs

dirs:
	mkdir -p $(DOWNLOAD_PATH)
	mkdir -p libs
	gcc --version | head --lines 1 | tr ' ' '\n' | tail -1 >libs/gcc-version

$(DOWNLOAD_PATH)/zlib-$(ZLIB_VERSION).tar.gz:
	wget --no-check-certificate http://sourceforge.net/projects/libpng/files/zlib/$(ZLIB_VERSION)/zlib-$(ZLIB_VERSION).tar.gz/download -O $(DOWNLOAD_PATH)/zlib-$(ZLIB_VERSION).tar.gz

zlib: libs/zlib-$(ZLIB_VERSION)

libs/zlib-$(ZLIB_VERSION): $(DOWNLOAD_PATH)/zlib-$(ZLIB_VERSION).tar.gz
	# prepare
	rm -r -f libs/zlib-*
	tar xvf $(DOWNLOAD_PATH)/zlib-$(ZLIB_VERSION).tar.gz
	# build
	cd zlib-$(ZLIB_VERSION) && ./configure
	#cd zlib-$(ZLIB_VERSION) && make install prefix="`pwd`/../libs"
	cd zlib-$(ZLIB_VERSION) && make
	# copy files
	mkdir -p libs/zlib-$(ZLIB_VERSION).tmp/include
	cp zlib-$(ZLIB_VERSION)/zlib.h libs/zlib-$(ZLIB_VERSION).tmp/include/
	cp zlib-$(ZLIB_VERSION)/zconf.h libs/zlib-$(ZLIB_VERSION).tmp/include/
	mkdir -p libs/zlib-$(ZLIB_VERSION).tmp/lib
	cp zlib-$(ZLIB_VERSION)/libz.a libs/zlib-$(ZLIB_VERSION).tmp/lib/
	rm -r -f zlib-$(ZLIB_VERSION)
	mv libs/zlib-$(ZLIB_VERSION).tmp libs/zlib-$(ZLIB_VERSION)

bzip2: libs/bzip2-$(BZIP2_VERSION)

$(DOWNLOAD_PATH)/bzip2-$(BZIP2_VERSION).tar.gz:
	wget http://bzip.org/$(BZIP2_VERSION)/bzip2-$(BZIP2_VERSION).tar.gz -O $(DOWNLOAD_PATH)/bzip2-$(BZIP2_VERSION).tar.gz

libs/bzip2-$(BZIP2_VERSION): $(DOWNLOAD_PATH)/bzip2-$(BZIP2_VERSION).tar.gz
	# prepare
	rm -r -f libs/bzip2-*
	tar xvf $(DOWNLOAD_PATH)/bzip2-$(BZIP2_VERSION).tar.gz
	# build
	#cd bzip2-$(BZIP2_VERSION) && make install PREFIX="`pwd`/../libs"
	cd bzip2-$(BZIP2_VERSION) && make
	# copy files
	mkdir -p libs/bzip2-$(BZIP2_VERSION).tmp/include
	cp bzip2-$(BZIP2_VERSION)/bzlib.h libs/bzip2-$(BZIP2_VERSION).tmp/include/
	mkdir -p libs/bzip2-$(BZIP2_VERSION).tmp/lib
	cp bzip2-$(BZIP2_VERSION)/libbz2.a libs/bzip2-$(BZIP2_VERSION).tmp/lib/
	rm -r -f bzip2-$(BZIP2_VERSION)
	mv libs/bzip2-$(BZIP2_VERSION).tmp libs/bzip2-$(BZIP2_VERSION)

miniupnpc: libs/miniupnpc-$(MINIUPNPC_VERSION)

$(DOWNLOAD_PATH)/miniupnpc-$(MINIUPNPC_VERSION).tar.gz:
	wget http://miniupnp.free.fr/files/download.php?file=miniupnpc-$(MINIUPNPC_VERSION).tar.gz -O $(DOWNLOAD_PATH)/miniupnpc-$(MINIUPNPC_VERSION).tar.gz

libs/miniupnpc-$(MINIUPNPC_VERSION): $(DOWNLOAD_PATH)/miniupnpc-$(MINIUPNPC_VERSION).tar.gz
	# prepare
	rm -r -f libs/miniupnpc-*
	tar xvf $(DOWNLOAD_PATH)/miniupnpc-$(MINIUPNPC_VERSION).tar.gz
	tar xvf $(MAKEFILE_PATH)miniupnpc-$(MINIUPNPC_VERSION)-fix.tar.gz
	# build
	cd miniupnpc-$(MINIUPNPC_VERSION) && CC=gcc && export CC && make -f Makefile.mingw init libminiupnpc.a miniupnpc.dll
	# copy files
	mkdir -p libs/miniupnpc-$(MINIUPNPC_VERSION).tmp/include/miniupnpc
	cp miniupnpc-$(MINIUPNPC_VERSION)/*.h libs/miniupnpc-$(MINIUPNPC_VERSION).tmp/include/miniupnpc/
	mkdir -p libs/miniupnpc-$(MINIUPNPC_VERSION).tmp/lib
	cp miniupnpc-$(MINIUPNPC_VERSION)/miniupnpc.lib libs/miniupnpc-$(MINIUPNPC_VERSION).tmp/lib/
	mkdir -p libs/miniupnpc-$(MINIUPNPC_VERSION).tmp/bin
	cp miniupnpc-$(MINIUPNPC_VERSION)/miniupnpc.dll libs/miniupnpc-$(MINIUPNPC_VERSION).tmp/bin/
	rm -r -f miniupnpc-$(MINIUPNPC_VERSION)
	mv libs/miniupnpc-$(MINIUPNPC_VERSION).tmp libs/miniupnpc-$(MINIUPNPC_VERSION)

openssl: libs/openssl-$(OPENSSL_VERSION)

$(DOWNLOAD_PATH)/openssl-$(OPENSSL_VERSION).tar.gz:
	wget --no-check-certificate https://www.openssl.org/source/openssl-$(OPENSSL_VERSION).tar.gz -O $(DOWNLOAD_PATH)/openssl-$(OPENSSL_VERSION).tar.gz

libs/openssl-$(OPENSSL_VERSION): $(DOWNLOAD_PATH)/openssl-$(OPENSSL_VERSION).tar.gz
	# prepare
	rm -r -f libs/openssl-*
	tar xvf $(DOWNLOAD_PATH)/openssl-$(OPENSSL_VERSION).tar.gz
	# build
	#cd openssl-$(OPENSSL_VERSION) && ./config --prefix="`pwd`/../libs"
	#cd openssl-$(OPENSSL_VERSION) && make install
	cd openssl-$(OPENSSL_VERSION) && ./config shared
	cd openssl-$(OPENSSL_VERSION) && make
	# copy files
	mkdir -p libs/openssl-$(OPENSSL_VERSION).tmp/include/openssl
	cp openssl-$(OPENSSL_VERSION)/include/openssl/*.h libs/openssl-$(OPENSSL_VERSION).tmp/include/openssl/
	mkdir -p libs/openssl-$(OPENSSL_VERSION).tmp/bin
	cp openssl-$(OPENSSL_VERSION)/libeay32.dll libs/openssl-$(OPENSSL_VERSION).tmp/bin/
	cp openssl-$(OPENSSL_VERSION)/ssleay32.dll libs/openssl-$(OPENSSL_VERSION).tmp/bin/
	mkdir -p libs/openssl-$(OPENSSL_VERSION).tmp/lib
	cp openssl-$(OPENSSL_VERSION)/libcrypto.dll.a libs/openssl-$(OPENSSL_VERSION).tmp/lib/
	cp openssl-$(OPENSSL_VERSION)/libssl.dll.a libs/openssl-$(OPENSSL_VERSION).tmp/lib/
	rm -r -f openssl-$(OPENSSL_VERSION)
	mv libs/openssl-$(OPENSSL_VERSION).tmp libs/openssl-$(OPENSSL_VERSION)

speex: libs/speex-$(SPEEX_VERSION)

$(DOWNLOAD_PATH)/speex-$(SPEEX_VERSION).tar.gz:
	wget http://downloads.xiph.org/releases/speex/speex-$(SPEEX_VERSION).tar.gz -O $(DOWNLOAD_PATH)/speex-$(SPEEX_VERSION).tar.gz

libs/speex-$(SPEEX_VERSION): $(DOWNLOAD_PATH)/speex-$(SPEEX_VERSION).tar.gz
	# prepare
	rm -r -f libs/speex-*
	tar xvf $(DOWNLOAD_PATH)/speex-$(SPEEX_VERSION).tar.gz
	# build
	cd speex-$(SPEEX_VERSION) && ./configure
	#cd speex-$(SPEEX_VERSION) && make install exec_prefix="`pwd`/../libs"
	cd speex-$(SPEEX_VERSION) && make
	# copy files
	mkdir -p libs/speex-$(SPEEX_VERSION).tmp/include/speex
	cp speex-$(SPEEX_VERSION)/include/speex/*.h libs/speex-$(SPEEX_VERSION).tmp/include/speex/
	mkdir -p libs/speex-$(SPEEX_VERSION).tmp/lib
	cp speex-$(SPEEX_VERSION)/libspeex/.libs/libspeex.a libs/speex-$(SPEEX_VERSION).tmp/lib
	rm -r -f speex-$(SPEEX_VERSION)
	mv libs/speex-$(SPEEX_VERSION).tmp libs/speex-$(SPEEX_VERSION)

speexdsp: libs/speexdsp-$(SPEEXDSP_VERSION)

$(DOWNLOAD_PATH)/speexdsp-$(SPEEXDSP_VERSION).tar.gz:
	wget http://downloads.xiph.org/releases/speex/speexdsp-$(SPEEXDSP_VERSION).tar.gz -O $(DOWNLOAD_PATH)/speexdsp-$(SPEEXDSP_VERSION).tar.gz

libs/speexdsp-$(SPEEXDSP_VERSION): $(DOWNLOAD_PATH)/speexdsp-$(SPEEXDSP_VERSION).tar.gz
	# prepare
	rm -r -f libs/speexdsp-*
	tar xvf $(DOWNLOAD_PATH)/speexdsp-$(SPEEXDSP_VERSION).tar.gz
	# build
	cd speexdsp-$(SPEEXDSP_VERSION) && ./configure
	cd speexdsp-$(SPEEXDSP_VERSION) && make
	# copy files
	mkdir -p libs/speexdsp-$(SPEEXDSP_VERSION).tmp/include/speex
	cp speexdsp-$(SPEEXDSP_VERSION)/include/speex/*.h libs/speexdsp-$(SPEEXDSP_VERSION).tmp/include/speex/
	mkdir -p libs/speexdsp-$(SPEEXDSP_VERSION).tmp/lib
	cp speexdsp-$(SPEEXDSP_VERSION)/libspeexdsp/.libs/libspeexdsp.a libs/speexdsp-$(SPEEXDSP_VERSION).tmp/lib
	rm -r -f speexdsp-$(SPEEXDSP_VERSION)
	mv libs/speexdsp-$(SPEEXDSP_VERSION).tmp libs/speexdsp-$(SPEEXDSP_VERSION)

libxml2: libs/libxml2-$(LIBXML2_VERSION)

$(DOWNLOAD_PATH)/libxml2-$(LIBXML2_VERSION).tar.gz:
	wget ftp://xmlsoft.org/libxml2/libxml2-$(LIBXML2_VERSION).tar.gz -O $(DOWNLOAD_PATH)/libxml2-$(LIBXML2_VERSION).tar.gz

libs/libxml2-$(LIBXML2_VERSION): $(DOWNLOAD_PATH)/libxml2-$(LIBXML2_VERSION).tar.gz
	# prepare
	rm -r -f libs/libxml2-*
	tar xvf $(DOWNLOAD_PATH)/libxml2-$(LIBXML2_VERSION).tar.gz
	# build
	cd libxml2-$(LIBXML2_VERSION) && ./configure --without-iconv -enable-shared=no
	#cd libxml2-$(LIBXML2_VERSION) && make install exec_prefix="`pwd`/../libs"
	cd libxml2-$(LIBXML2_VERSION) && make
	# copy files
	mkdir -p libs/libxml2-$(LIBXML2_VERSION).tmp/include/libxml
	cp libxml2-$(LIBXML2_VERSION)/include/libxml/*.h libs/libxml2-$(LIBXML2_VERSION).tmp/include/libxml/
	mkdir -p libs/libxml2-$(LIBXML2_VERSION).tmp/lib
	cp libxml2-$(LIBXML2_VERSION)/.libs/libxml2.a libs/libxml2-$(LIBXML2_VERSION).tmp/lib/
	#rm -r -f libxml2-$(LIBXML2_VERSION) # see libxslt
	mv libs/libxml2-$(LIBXML2_VERSION).tmp libs/libxml2-$(LIBXML2_VERSION)

libxslt: libs/libxslt-$(LIBXSLT_VERSION)

$(DOWNLOAD_PATH)/libxslt-$(LIBXSLT_VERSION).tar.gz:
	wget ftp://xmlsoft.org/libxml2/libxslt-$(LIBXSLT_VERSION).tar.gz -O $(DOWNLOAD_PATH)/libxslt-$(LIBXSLT_VERSION).tar.gz

libs/libxslt-$(LIBXSLT_VERSION): $(DOWNLOAD_PATH)/libxml2-$(LIBXML2_VERSION).tar.gz $(DOWNLOAD_PATH)/libxslt-$(LIBXSLT_VERSION).tar.gz
	# prepare
	rm -r -f libs/libxslt-*
	tar xvf $(DOWNLOAD_PATH)/libxml2-$(LIBXML2_VERSION).tar.gz
	tar xvf $(DOWNLOAD_PATH)/libxslt-$(LIBXSLT_VERSION).tar.gz
	tar xvf $(MAKEFILE_PATH)libxslt-$(LIBXSLT_VERSION)-fix.tar.gz
	# build
	cd libxslt-$(LIBXSLT_VERSION) && ./configure --with-libxml-src=../libxml2-$(LIBXML2_VERSION) -enable-shared=no CFLAGS=-DLIBXML_STATIC
	cd libxslt-$(LIBXSLT_VERSION) && make
	# copy files
	mkdir -p libs/libxslt-$(LIBXSLT_VERSION).tmp/include/libxslt
	cp libxslt-$(LIBXSLT_VERSION)/libxslt/*.h libs/libxslt-$(LIBXSLT_VERSION).tmp/include/libxslt/
	mkdir -p libs/libxslt-$(LIBXSLT_VERSION).tmp/lib
	cp libxslt-$(LIBXSLT_VERSION)/libxslt/.libs/libxslt.a libs/libxslt-$(LIBXSLT_VERSION).tmp/lib/
	cp libxslt-$(LIBXSLT_VERSION)/libexslt/.libs/libexslt.a libs/libxslt-$(LIBXSLT_VERSION).tmp/lib/
	rm -r -f libxml2-$(LIBXML2_VERSION)
	rm -r -f libxslt-$(LIBXSLT_VERSION)
	mv libs/libxslt-$(LIBXSLT_VERSION).tmp libs/libxslt-$(LIBXSLT_VERSION)

curl: libs/curl-$(CURL_VERSION)

$(DOWNLOAD_PATH)/curl-$(CURL_VERSION).tar.gz:
	wget --no-check-certificate http://curl.haxx.se/$(DOWNLOAD_PATH)/curl-$(CURL_VERSION).tar.gz -O $(DOWNLOAD_PATH)/curl-$(CURL_VERSION).tar.gz

libs/curl-$(CURL_VERSION): $(DOWNLOAD_PATH)/curl-$(CURL_VERSION).tar.gz
	# prepare
	rm -r -f libs/curl-*
	tar xvf $(DOWNLOAD_PATH)/curl-$(CURL_VERSION).tar.gz
	# build
	cd curl-$(CURL_VERSION) && ./configure --disable-shared --with-ssl="`pwd`/../libs/openssl-$(OPENSSL_VERSION)"
	#cd curl-$(CURL_VERSION) && make install exec_prefix="`pwd`/../libs"
	cd curl-$(CURL_VERSION) && make
	# copy files
	mkdir -p libs/curl-$(CURL_VERSION).tmp/include/curl
	cp curl-$(CURL_VERSION)/include/curl/*.h libs/curl-$(CURL_VERSION).tmp/include/curl/
	mkdir -p libs/curl-$(CURL_VERSION).tmp/lib
	cp curl-$(CURL_VERSION)/lib/.libs/libcurl.a libs/curl-$(CURL_VERSION).tmp/lib/
	rm -r -f curl-$(CURL_VERSION)
	mv libs/curl-$(CURL_VERSION).tmp libs/curl-$(CURL_VERSION)

libssh: libs/libssh-$(LIBSSH_VERSION)

$(DOWNLOAD_PATH)/libssh-$(LIBSSH_VERSION).tar.gz:
	wget --no-check-certificate https://red.libssh.org/attachments/download/41/libssh-$(LIBSSH_VERSION).tar.gz -O $(DOWNLOAD_PATH)/libssh-$(LIBSSH_VERSION).tar.gz

libs/libssh-$(LIBSSH_VERSION): $(DOWNLOAD_PATH)/libssh-$(LIBSSH_VERSION).tar.gz
	# prepare
	rm -r -f libs/libssh-*
	tar xvf $(DOWNLOAD_PATH)/libssh-$(LIBSSH_VERSION).tar.gz
	tar xvf $(MAKEFILE_PATH)libssh-$(LIBSSH_VERSION)-fix.tar.gz
	# build
	mkdir -p libssh-$(LIBSSH_VERSION)/build
	cd libssh-$(LIBSSH_VERSION)/build && cmake .. -G"MSYS Makefiles" -DWITH_STATIC_LIB:BOOL=ON -DZLIB_LIBRARY:FILEPATH="`pwd`/../../libs/zlib-$(ZLIB_VERSION)/lib/libz.a" -DZLIB_INCLUDE_DIR:PATH="`pwd`/../../libs/zlib-$(ZLIB_VERSION)/include" -DOPENSSL_LIBRARIES:FILEPATH="`pwd`/../../libs/openssl-$(OPENSSL_VERSION)/lib/libcrypto.dll.a" -DOPENSSL_INCLUDE_DIRS:PATH="`pwd`/../../libs/openssl-$(OPENSSL_VERSION)/include"
	cd libssh-$(LIBSSH_VERSION)/build && make
	# copy files
	mkdir -p libs/libssh-$(LIBSSH_VERSION).tmp/lib
	cp libssh-$(LIBSSH_VERSION)/build/src/libssh.a libs/libssh-$(LIBSSH_VERSION).tmp/lib
	cp libssh-$(LIBSSH_VERSION)/build/src/threads/libssh_threads.a libs/libssh-$(LIBSSH_VERSION).tmp/lib
	mkdir -p libs/libssh-$(LIBSSH_VERSION).tmp/include/libssh
	cp libssh-$(LIBSSH_VERSION)/include/libssh/*.h libs/libssh-$(LIBSSH_VERSION).tmp/include/libssh/
	rm -r -f libssh-$(LIBSSH_VERSION)
	mv libs/libssh-$(LIBSSH_VERSION).tmp libs/libssh-$(LIBSSH_VERSION)

protobuf: libs/protobuf-$(PROTOBUF_VERSION)

$(DOWNLOAD_PATH)/protobuf-$(PROTOBUF_VERSION).tar.gz:
	wget --no-check-certificate https://github.com/google/protobuf/releases/download/v$(PROTOBUF_VERSION)/protobuf-$(PROTOBUF_VERSION).tar.gz -O $(DOWNLOAD_PATH)/protobuf-$(PROTOBUF_VERSION).tar.gz

libs/protobuf-$(PROTOBUF_VERSION): $(DOWNLOAD_PATH)/protobuf-$(PROTOBUF_VERSION).tar.gz
	# prepare
	rm -r -f libs/protobuf-*
	tar xvf $(DOWNLOAD_PATH)/protobuf-$(PROTOBUF_VERSION).tar.gz
	# build
	cd protobuf-$(PROTOBUF_VERSION) && ./configure --disable-shared
	#cd protobuf-$(PROTOBUF_VERSION) && make install exec_prefix="`pwd`/../libs"
	cd protobuf-$(PROTOBUF_VERSION) && make
	# copy files
	mkdir -p libs/protobuf-$(PROTOBUF_VERSION).tmp/include/protobuf
	cp -r protobuf-$(PROTOBUF_VERSION)/src/google/ libs/protobuf-$(PROTOBUF_VERSION).tmp/include/protobuf/
	mkdir -p libs/protobuf-$(PROTOBUF_VERSION).tmp/lib
	cp protobuf-$(PROTOBUF_VERSION)/src/.libs/libprotobuf.a libs/protobuf-$(PROTOBUF_VERSION).tmp/lib/
	mkdir -p libs/protobuf-$(PROTOBUF_VERSION).tmp/bin
	cp protobuf-$(PROTOBUF_VERSION)/src/protoc.exe libs/protobuf-$(PROTOBUF_VERSION).tmp/bin/
	rm -r -f protobuf-$(PROTOBUF_VERSION)
	mv libs/protobuf-$(PROTOBUF_VERSION).tmp libs/protobuf-$(PROTOBUF_VERSION)

copylibs:
	if [ "$(COPY_ANSWER)" = "" ] ; then \
	  read -p "Do you want to copy libs to retroshare? (y|n)" answer; \
	else \
	  answer=$(COPY_ANSWER) ; \
	fi ; \
	if [ "$$answer" = "y" ] ; then \
	  rm -r -f $(LIBS_PATH) ; \
	  mkdir -p $(LIBS_PATH) ; \
	  cp ./libs/gcc-version $(LIBS_PATH) ; \
	  find ./libs -mindepth 1 -maxdepth 1 -type d -not -name "*.tmp" -print -exec cp -r {}/. $(LIBS_PATH) \; ; \
	fi
