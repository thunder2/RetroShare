ZLIB_VERSION=1.2.11
BZIP2_VERSION=1.0.8
MINIUPNPC_VERSION=2.2.3
OPENSSL_VERSION=1.1.1w
SPEEX_VERSION=1.2.1
SPEEXDSP_VERSION=1.2.1
LIBXML2_MAIN_VERSION=2.14
LIBXML2_VERSION=$(LIBXML2_MAIN_VERSION).5
LIBXSLT_MAIN_VERSION=1.1
LIBXSLT_VERSION=$(LIBXSLT_MAIN_VERSION).43
CURL_VERSION=8.9.1
TCL_VERSION=8.6.10
SQLCIPHER_VERSION=4.5.0
FFMPEG_VERSION=4.4.6
RAPIDJSON_VERSION=1.1.0
XAPIAN_VERSION=1.4.29
ASIO_VERSION=1-34-2
#RNP_VERSION=0.17.1

# libaries for rnp
JSON_C_VERSION=0.18
BOTAN_VERSION=2.19.5

DOWNLOAD_PATH?=download
BUILD_PATH=build
LIBS_PATH?=libs

all: cleanlibs dirs zlib bzip2 miniupnpc openssl speex speexdsp libxml curl sqlcipher ffmpeg rapidjson xapian jsonc botan asio copylibs
#rnp

download: \
	$(DOWNLOAD_PATH)/zlib-$(ZLIB_VERSION).tar.gz \
	$(DOWNLOAD_PATH)/bzip2-$(BZIP2_VERSION).tar.gz \
	$(DOWNLOAD_PATH)/miniupnpc-$(MINIUPNPC_VERSION).tar.gz \
	$(DOWNLOAD_PATH)/openssl-$(OPENSSL_VERSION).tar.gz \
	$(DOWNLOAD_PATH)/speex-$(SPEEX_VERSION).tar.gz \
	$(DOWNLOAD_PATH)/speexdsp-$(SPEEXDSP_VERSION).tar.gz \
	$(DOWNLOAD_PATH)/libxml2-$(LIBXML2_VERSION).tar.xz \
	$(DOWNLOAD_PATH)/libxslt-$(LIBXSLT_VERSION).tar.xz \
	$(DOWNLOAD_PATH)/curl-$(CURL_VERSION).tar.gz \
	$(DOWNLOAD_PATH)/tcl$(TCL_VERSION)-src.tar.gz \
	$(DOWNLOAD_PATH)/sqlcipher-$(SQLCIPHER_VERSION).tar.gz \
	$(DOWNLOAD_PATH)/ffmpeg-$(FFMPEG_VERSION).tar.xz \
	$(DOWNLOAD_PATH)/rapidjson-$(RAPIDJSON_VERSION).tar.gz \
	$(DOWNLOAD_PATH)/xapian-core-$(XAPIAN_VERSION).tar.xz

clean:
	@rm -r -f $(BUILD_PATH)
	@rm -r -f $(LIBS_PATH)

dirs:
	@mkdir -p $(DOWNLOAD_PATH)
	@mkdir -p $(BUILD_PATH)
	@gcc --version | head --lines 1 | tr ' ' '\n' | tail -1 >$(BUILD_PATH)/gcc-version

WIN_LIBS_PATH := $(shell cygpath -m '$(abspath $(LIBS_PATH))' | sed 's/^[a-z]:/\U&/')

define CLEAN_PC_FILES
	find "$(1)" \( -name "*.pc" -o -name "*.la" \) -exec perl -i -pe \
		's{(^|[=\s'\'']|-[IL])/([a-zA-Z])/}{$$1\U$$2:/}g; \
		 s{((?:^|[=\s'\''])(?:-[IL])?|-[IL])[^ ='\''"\s]*/build/[^/\s'\'']+}{$$1$(WIN_LIBS_PATH)}g; \
		 s{(^|[=\s'\'']|-[IL])([a-z]):/}{$$1\U$$2:/}g;' {} +
endef

ifeq ($(MSYSTEM),MINGW32)
	RC_TARGET_FLAG := -F pe-i386
else ifneq ($(filter $(MSYSTEM),MINGW64),)
	RC_TARGET_FLAG :=
else
	$(error ERROR: Only MINGW32 and MINGW64 supported)
endif

$(DOWNLOAD_PATH)/zlib-$(ZLIB_VERSION).tar.gz:
	wget --no-check-certificate http://sourceforge.net/projects/libpng/files/zlib/$(ZLIB_VERSION)/zlib-$(ZLIB_VERSION).tar.gz/download -O $(DOWNLOAD_PATH)/zlib-$(ZLIB_VERSION).tar.gz

zlib: $(BUILD_PATH)/zlib-$(ZLIB_VERSION)

$(BUILD_PATH)/zlib-$(ZLIB_VERSION): $(DOWNLOAD_PATH)/zlib-$(ZLIB_VERSION).tar.gz
	# prepare
	rm -r -f $(BUILD_PATH)/zlib-*
	tar xvf $(DOWNLOAD_PATH)/zlib-$(ZLIB_VERSION).tar.gz
	# build
	cd zlib-$(ZLIB_VERSION) && make -j -f win32/Makefile.gcc libz.a
	# copy files
	mkdir -p $(BUILD_PATH)/zlib-$(ZLIB_VERSION).tmp/include
	cp zlib-$(ZLIB_VERSION)/zlib.h $(BUILD_PATH)/zlib-$(ZLIB_VERSION).tmp/include/
	cp zlib-$(ZLIB_VERSION)/zconf.h $(BUILD_PATH)/zlib-$(ZLIB_VERSION).tmp/include/
	mkdir -p $(BUILD_PATH)/zlib-$(ZLIB_VERSION).tmp/lib
	cp zlib-$(ZLIB_VERSION)/libz.a $(BUILD_PATH)/zlib-$(ZLIB_VERSION).tmp/lib/
	# cleanup
	rm -r -f zlib-$(ZLIB_VERSION)
	mv $(BUILD_PATH)/zlib-$(ZLIB_VERSION).tmp $(BUILD_PATH)/zlib-$(ZLIB_VERSION)

bzip2: $(BUILD_PATH)/bzip2-$(BZIP2_VERSION)

$(DOWNLOAD_PATH)/bzip2-$(BZIP2_VERSION).tar.gz:
	wget https://www.sourceware.org/pub/bzip2/bzip2-$(BZIP2_VERSION).tar.gz -O $(DOWNLOAD_PATH)/bzip2-$(BZIP2_VERSION).tar.gz

$(BUILD_PATH)/bzip2-$(BZIP2_VERSION): $(DOWNLOAD_PATH)/bzip2-$(BZIP2_VERSION).tar.gz
	# prepare
	rm -r -f $(BUILD_PATH)/bzip2-*
	tar xvf $(DOWNLOAD_PATH)/bzip2-$(BZIP2_VERSION).tar.gz
	# build
	#cd bzip2-$(BZIP2_VERSION) && make install PREFIX="`pwd`/../$(BUILD_PATH)"
	cd bzip2-$(BZIP2_VERSION) && make -j
	# copy files
	mkdir -p $(BUILD_PATH)/bzip2-$(BZIP2_VERSION).tmp/include
	cp bzip2-$(BZIP2_VERSION)/bzlib.h $(BUILD_PATH)/bzip2-$(BZIP2_VERSION).tmp/include/
	mkdir -p $(BUILD_PATH)/bzip2-$(BZIP2_VERSION).tmp/lib
	cp bzip2-$(BZIP2_VERSION)/libbz2.a $(BUILD_PATH)/bzip2-$(BZIP2_VERSION).tmp/lib/
	# cleanup
	rm -r -f bzip2-$(BZIP2_VERSION)
	mv $(BUILD_PATH)/bzip2-$(BZIP2_VERSION).tmp $(BUILD_PATH)/bzip2-$(BZIP2_VERSION)

miniupnpc: $(BUILD_PATH)/miniupnpc-$(MINIUPNPC_VERSION)

$(DOWNLOAD_PATH)/miniupnpc-$(MINIUPNPC_VERSION).tar.gz:
	wget http://miniupnp.free.fr/files/download.php?file=miniupnpc-$(MINIUPNPC_VERSION).tar.gz -O $(DOWNLOAD_PATH)/miniupnpc-$(MINIUPNPC_VERSION).tar.gz

$(BUILD_PATH)/miniupnpc-$(MINIUPNPC_VERSION): $(DOWNLOAD_PATH)/miniupnpc-$(MINIUPNPC_VERSION).tar.gz
	# prepare
	rm -r -f $(BUILD_PATH)/miniupnpc-*
	tar xvf $(DOWNLOAD_PATH)/miniupnpc-$(MINIUPNPC_VERSION).tar.gz
	# build
	cmake -S miniupnpc-$(MINIUPNPC_VERSION) -B miniupnpc-$(MINIUPNPC_VERSION)/build -DCMAKE_INSTALL_PREFIX="$(abspath $(LIBS_PATH))" -DCMAKE_RC_FLAGS="$(RC_TARGET_FLAG)" -DUPNPC_BUILD_STATIC=OFF -DUPNPC_BUILD_SHARED=ON -DUPNPC_BUILD_TESTS=OFF -DUPNPC_BUILD_SAMPLE=OFF
	cmake --build miniupnpc-$(MINIUPNPC_VERSION)/build --parallel
	# copy files
	# Workaround für miniupnpc 2.2.3 (CMakeLists.txt sucht Header im Hauptordner statt in include/)
	cp miniupnpc-$(MINIUPNPC_VERSION)/include/*.h miniupnpc-$(MINIUPNPC_VERSION)/
	cmake --install miniupnpc-$(MINIUPNPC_VERSION)/build --prefix "$(abspath $(BUILD_PATH)/miniupnpc-$(MINIUPNPC_VERSION).tmp)"
	# cleanup
	rm -r -f miniupnpc-$(MINIUPNPC_VERSION)
	mv $(BUILD_PATH)/miniupnpc-$(MINIUPNPC_VERSION).tmp $(BUILD_PATH)/miniupnpc-$(MINIUPNPC_VERSION)

openssl: $(BUILD_PATH)/openssl-$(OPENSSL_VERSION)

$(DOWNLOAD_PATH)/openssl-$(OPENSSL_VERSION).tar.gz:
	wget --no-check-certificate https://www.openssl.org/source/openssl-$(OPENSSL_VERSION).tar.gz -O $(DOWNLOAD_PATH)/openssl-$(OPENSSL_VERSION).tar.gz

$(BUILD_PATH)/openssl-$(OPENSSL_VERSION): $(DOWNLOAD_PATH)/openssl-$(OPENSSL_VERSION).tar.gz
	# prepare
	rm -r -f $(BUILD_PATH)/openssl-*
	tar xvf $(DOWNLOAD_PATH)/openssl-$(OPENSSL_VERSION).tar.gz
	# build
	cd openssl-$(OPENSSL_VERSION) && ./config shared --prefix="$(abspath $(LIBS_PATH))" --openssldir="$(abspath $(BUILD_PATH)/openssl-$(OPENSSL_VERSION).tmp/ssl)"
	cd openssl-$(OPENSSL_VERSION) && make -j build_libs
	# copy files
	cd openssl-$(OPENSSL_VERSION) && make install_dev INSTALLTOP_dir="$(abspath $(BUILD_PATH)/openssl-$(OPENSSL_VERSION).tmp)" ENGINESDIR_dir="$(abspath $(BUILD_PATH)/openssl-$(OPENSSL_VERSION).tmp)/lib/engines-1_1" OPENSSLDIR_dir="$(abspath $(BUILD_PATH)/openssl-$(OPENSSL_VERSION).tmp)"
	rm -f "$(BUILD_PATH)/openssl-$(OPENSSL_VERSION).tmp/lib/libcrypto.a" "$(BUILD_PATH)/openssl-$(OPENSSL_VERSION).tmp/lib/libssl.a"
	$(call CLEAN_PC_FILES,$(abspath $(BUILD_PATH)/openssl-$(OPENSSL_VERSION).tmp))
	# cleanup
	rm -r -f openssl-$(OPENSSL_VERSION)
	mv $(BUILD_PATH)/openssl-$(OPENSSL_VERSION).tmp $(BUILD_PATH)/openssl-$(OPENSSL_VERSION)

speex: $(BUILD_PATH)/speex-$(SPEEX_VERSION)

$(DOWNLOAD_PATH)/speex-$(SPEEX_VERSION).tar.gz:
	wget --no-check-certificate http://downloads.xiph.org/releases/speex/speex-$(SPEEX_VERSION).tar.gz -O $(DOWNLOAD_PATH)/speex-$(SPEEX_VERSION).tar.gz

$(BUILD_PATH)/speex-$(SPEEX_VERSION): $(DOWNLOAD_PATH)/speex-$(SPEEX_VERSION).tar.gz
	# prepare
	rm -r -f $(BUILD_PATH)/speex-*
	tar xvf $(DOWNLOAD_PATH)/speex-$(SPEEX_VERSION).tar.gz
	# build
	cd speex-$(SPEEX_VERSION) && ./configure --disable-shared --enable-static --prefix="$(abspath $(LIBS_PATH))"
	cd speex-$(SPEEX_VERSION) && make -j
	# copy files
	cd speex-$(SPEEX_VERSION) && make install prefix="$(abspath $(BUILD_PATH)/speex-$(SPEEX_VERSION).tmp)"
	$(call CLEAN_PC_FILES,$(abspath $(BUILD_PATH)/speex-$(SPEEX_VERSION).tmp))
	# cleanup
	rm -r -f speex-$(SPEEX_VERSION)
	mv $(BUILD_PATH)/speex-$(SPEEX_VERSION).tmp $(BUILD_PATH)/speex-$(SPEEX_VERSION)

speexdsp: $(BUILD_PATH)/speexdsp-$(SPEEXDSP_VERSION)

$(DOWNLOAD_PATH)/speexdsp-$(SPEEXDSP_VERSION).tar.gz:
	wget --no-check-certificate http://downloads.xiph.org/releases/speex/speexdsp-$(SPEEXDSP_VERSION).tar.gz -O $(DOWNLOAD_PATH)/speexdsp-$(SPEEXDSP_VERSION).tar.gz

$(BUILD_PATH)/speexdsp-$(SPEEXDSP_VERSION): $(DOWNLOAD_PATH)/speexdsp-$(SPEEXDSP_VERSION).tar.gz
	# prepare
	rm -r -f $(BUILD_PATH)/speexdsp-*
	tar xvf $(DOWNLOAD_PATH)/speexdsp-$(SPEEXDSP_VERSION).tar.gz
	# build
	cd speexdsp-$(SPEEXDSP_VERSION) && ./configure --disable-shared --enable-static --prefix="$(abspath $(LIBS_PATH))"
	cd speexdsp-$(SPEEXDSP_VERSION) && make -j
	# copy files
	cd speexdsp-$(SPEEXDSP_VERSION) && make install prefix="$(abspath $(BUILD_PATH)/speexdsp-$(SPEEXDSP_VERSION).tmp)"
	# copy missing header files
	cp speexdsp-$(SPEEXDSP_VERSION)/include/speex/*.h $(BUILD_PATH)/speexdsp-$(SPEEXDSP_VERSION).tmp/include/speex/
	$(call CLEAN_PC_FILES,$(abspath $(BUILD_PATH)/speexdsp-$(SPEEXDSP_VERSION).tmp))
	# cleanup
	rm -r -f speexdsp-$(SPEEXDSP_VERSION)
	mv $(BUILD_PATH)/speexdsp-$(SPEEXDSP_VERSION).tmp $(BUILD_PATH)/speexdsp-$(SPEEXDSP_VERSION)

libxml: \
	$(BUILD_PATH)/libxml2-$(LIBXML2_VERSION) \
	$(BUILD_PATH)/libxslt-$(LIBXSLT_VERSION)

$(DOWNLOAD_PATH)/libxml2-$(LIBXML2_VERSION).tar.xz:
	wget --no-check-certificate https://download.gnome.org/sources/libxml2/$(LIBXML2_MAIN_VERSION)/libxml2-$(LIBXML2_VERSION).tar.xz -O $(DOWNLOAD_PATH)/libxml2-$(LIBXML2_VERSION).tar.xz

$(DOWNLOAD_PATH)/libxslt-$(LIBXSLT_VERSION).tar.xz:
	wget --no-check-certificate https://download.gnome.org/sources/libxslt/$(LIBXSLT_MAIN_VERSION)/libxslt-$(LIBXSLT_VERSION).tar.xz -O $(DOWNLOAD_PATH)/libxslt-$(LIBXSLT_VERSION).tar.xz

$(BUILD_PATH)/libxml2-$(LIBXML2_VERSION) $(BUILD_PATH)/libxslt-$(LIBXSLT_VERSION): \
	$(DOWNLOAD_PATH)/libxml2-$(LIBXML2_VERSION).tar.xz \
	$(DOWNLOAD_PATH)/libxslt-$(LIBXSLT_VERSION).tar.xz
	# libxml2: prepare
	pacman --needed --noconfirm -S python3 pkg-config
	rm -r -f $(BUILD_PATH)/libxml2-*
	tar xvf $(DOWNLOAD_PATH)/libxml2-$(LIBXML2_VERSION).tar.xz
	# libxslt: prepare
	rm -r -f $(BUILD_PATH)/libxslt-*
	tar xvf $(DOWNLOAD_PATH)/libxslt-$(LIBXSLT_VERSION).tar.xz
	# libxml2: build
	cd libxml2-$(LIBXML2_VERSION) && ./configure --without-iconv -disable-shared --prefix="$(abspath $(LIBS_PATH))"
	cd libxml2-$(LIBXML2_VERSION) && make -j libxml2.la
	# libxslt: build
	cd libxslt-$(LIBXSLT_VERSION) && ./configure --with-libxml-src=../libxml2-$(LIBXML2_VERSION) -disable-shared CFLAGS=-DLIBXML_STATIC --prefix="$(abspath $(LIBS_PATH))"
	cd libxslt-$(LIBXSLT_VERSION) && make -j -C libxslt libxslt.la
	cd libxslt-$(LIBXSLT_VERSION) && make -j -C libexslt libexslt.la
	# libxml2: copy files
	cd libxml2-$(LIBXML2_VERSION) && make install-libLTLIBRARIES install-data-am prefix="$(abspath $(BUILD_PATH)/libxml2-$(LIBXML2_VERSION).tmp)" exec_prefix="$(abspath $(BUILD_PATH)/libxml2-$(LIBXML2_VERSION).tmp)"
	cd libxml2-$(LIBXML2_VERSION)/include && make install prefix="$(abspath $(BUILD_PATH)/libxml2-$(LIBXML2_VERSION).tmp)" exec_prefix="$(abspath $(BUILD_PATH)/libxml2-$(LIBXML2_VERSION).tmp)"
	$(call CLEAN_PC_FILES,$(abspath $(BUILD_PATH)/libxml2-$(LIBXML2_VERSION).tmp))
	# libxslt: copy files
	cd libxslt-$(LIBXSLT_VERSION)/libxslt && make install prefix="$(abspath $(BUILD_PATH)/libxslt-$(LIBXSLT_VERSION).tmp)" exec_prefix="$(abspath $(BUILD_PATH)/libxslt-$(LIBXSLT_VERSION).tmp)"
	cd libxslt-$(LIBXSLT_VERSION)/libexslt && make install prefix="$(abspath $(BUILD_PATH)/libxslt-$(LIBXSLT_VERSION).tmp)" exec_prefix="$(abspath $(BUILD_PATH)/libxslt-$(LIBXSLT_VERSION).tmp)"
	cd libxslt-$(LIBXSLT_VERSION) && make install-data-am prefix="$(abspath $(BUILD_PATH)/libxslt-$(LIBXSLT_VERSION).tmp)" exec_prefix="$(abspath $(BUILD_PATH)/libxslt-$(LIBXSLT_VERSION).tmp)"
	$(call CLEAN_PC_FILES,$(abspath $(BUILD_PATH)/libxslt-$(LIBXSLT_VERSION).tmp))
	# libxml2: cleanup
	rm -r -f libxml2-$(LIBXML2_VERSION)
	# libxslt: cleanup
	rm -r -f libxslt-$(LIBXSLT_VERSION)
	# finish
	mv $(BUILD_PATH)/libxml2-$(LIBXML2_VERSION).tmp $(BUILD_PATH)/libxml2-$(LIBXML2_VERSION)
	mv $(BUILD_PATH)/libxslt-$(LIBXSLT_VERSION).tmp $(BUILD_PATH)/libxslt-$(LIBXSLT_VERSION)

curl: $(BUILD_PATH)/curl-$(CURL_VERSION)

$(DOWNLOAD_PATH)/curl-$(CURL_VERSION).tar.gz:
	wget --no-check-certificate http://curl.haxx.se/download/curl-$(CURL_VERSION).tar.gz -O $(DOWNLOAD_PATH)/curl-$(CURL_VERSION).tar.gz

$(BUILD_PATH)/curl-$(CURL_VERSION): $(DOWNLOAD_PATH)/curl-$(CURL_VERSION).tar.gz
	# prepare
	rm -r -f $(BUILD_PATH)/curl-*
	tar xvf $(DOWNLOAD_PATH)/curl-$(CURL_VERSION).tar.gz
	# build
	cd curl-$(CURL_VERSION) && cmake -B build -DCMAKE_INSTALL_PREFIX="$(abspath $(LIBS_PATH))" -DCMAKE_PREFIX_PATH="$(abspath $(BUILD_PATH)/openssl-$(OPENSSL_VERSION))" -DBUILD_SHARED_LIBS=OFF -DBUILD_CURL_EXE=OFF -DBUILD_TESTING=OFF -DBUILD_LIBCURL_DOCS=OFF -DENABLE_CURL_MANUAL=OFF -DCURL_USE_OPENSSL=ON -DCURL_USE_LIBPSL=OFF -DOPENSSL_ROOT_DIR="$(abspath $(BUILD_PATH)/openssl-$(OPENSSL_VERSION))" -DOPENSSL_INCLUDE_DIR="$(abspath $(BUILD_PATH)/openssl-$(OPENSSL_VERSION)/include)" -DOPENSSL_SSL_LIBRARY="$(abspath $(BUILD_PATH)/openssl-$(OPENSSL_VERSION)/lib/libssl.dll.a)" -DOPENSSL_CRYPTO_LIBRARY="$(abspath $(BUILD_PATH)/openssl-$(OPENSSL_VERSION)/lib/libcrypto.dll.a)"
	cd curl-$(CURL_VERSION) && cmake --build build -j
	# copy files
	cd curl-$(CURL_VERSION) && cmake --install build --prefix "$(abspath $(BUILD_PATH)/curl-$(CURL_VERSION).tmp)"
	$(call CLEAN_PC_FILES,$(abspath $(BUILD_PATH)/curl-$(CURL_VERSION).tmp))
	# cleanup
	rm -r -f curl-$(CURL_VERSION)
	mv $(BUILD_PATH)/curl-$(CURL_VERSION).tmp $(BUILD_PATH)/curl-$(CURL_VERSION)

sqlcipher: $(BUILD_PATH)/sqlcipher-$(SQLCIPHER_VERSION)

$(DOWNLOAD_PATH)/tcl$(TCL_VERSION)-src.tar.gz:
	wget --no-check-certificate http://downloads.sourceforge.net/project/tcl/Tcl/$(TCL_VERSION)/tcl$(TCL_VERSION)-src.tar.gz -O $(DOWNLOAD_PATH)/tcl$(TCL_VERSION)-src.tar.gz

$(DOWNLOAD_PATH)/sqlcipher-$(SQLCIPHER_VERSION).tar.gz:
	wget --no-check-certificate https://github.com/sqlcipher/sqlcipher/archive/v$(SQLCIPHER_VERSION).tar.gz -O $(DOWNLOAD_PATH)/sqlcipher-$(SQLCIPHER_VERSION).tar.gz

$(BUILD_PATH)/sqlcipher-$(SQLCIPHER_VERSION): $(DOWNLOAD_PATH)/tcl$(TCL_VERSION)-src.tar.gz $(DOWNLOAD_PATH)/sqlcipher-$(SQLCIPHER_VERSION).tar.gz
	# prepare
	rm -r -f $(BUILD_PATH)/sqlcipher-*
	# tcl
	tar xvf $(DOWNLOAD_PATH)/tcl$(TCL_VERSION)-src.tar.gz
	mkdir -p tcl$(TCL_VERSION)/build
	cd tcl$(TCL_VERSION)/build && RC="windres $(RC_TARGET_FLAG)" ../win/configure
	cd tcl$(TCL_VERSION)/build && make -j tclsh86.exe
	# sqlcipher
	tar xvf $(DOWNLOAD_PATH)/sqlcipher-$(SQLCIPHER_VERSION).tar.gz
	cd sqlcipher-$(SQLCIPHER_VERSION) && ln -s ../tcl$(TCL_VERSION)/build/tclsh86.exe tclsh
	mkdir -p tcl$(TCL_VERSION)/lib
	ln -s `pwd`/tcl$(TCL_VERSION)/library `pwd`/tcl$(TCL_VERSION)/lib/tcl8.6
	# build
	cd sqlcipher-$(SQLCIPHER_VERSION) && PATH=.:$$PATH:$(abspath tcl$(TCL_VERSION)/build) && export LIBS="-L$(abspath $(BUILD_PATH)/openssl-$(OPENSSL_VERSION)/lib) -lgdi32 $$LIBS" && ./configure --disable-shared --enable-static --enable-tempstore=yes CFLAGS="-DSQLITE_HAS_CODEC -I$(abspath $(BUILD_PATH)/openssl-$(OPENSSL_VERSION)/include) -I$(abspath tcl$(TCL_VERSION)/generic)" LDFLAGS="-L$(abspath $(BUILD_PATH)/openssl-$(OPENSSL_VERSION)/lib) -lcrypto -lgdi32" --with-tcl="$(abspath tcl$(TCL_VERSION)/build)" --prefix="$(abspath $(LIBS_PATH))" && make -j
	# copy files
	cd sqlcipher-$(SQLCIPHER_VERSION) && make install prefix="$(abspath $(BUILD_PATH)/sqlcipher-$(SQLCIPHER_VERSION).tmp)"
	# fix path to openssl
	sed -i 's|$(abspath $(BUILD_PATH)/openssl-$(OPENSSL_VERSION)/lib)|$(abspath $(LIBS_PATH))/lib|g' "$(abspath $(BUILD_PATH)/sqlcipher-$(SQLCIPHER_VERSION).tmp)/lib/libsqlcipher.la" "$(abspath $(BUILD_PATH)/sqlcipher-$(SQLCIPHER_VERSION).tmp)/lib/pkgconfig/sqlcipher.pc"
	$(call CLEAN_PC_FILES,$(abspath $(BUILD_PATH)/sqlcipher-$(SQLCIPHER_VERSION).tmp))
	# cleanup
	rm -r -f sqlcipher-$(SQLCIPHER_VERSION)
	rm -r -f tcl$(TCL_VERSION)
	mv $(BUILD_PATH)/sqlcipher-$(SQLCIPHER_VERSION).tmp $(BUILD_PATH)/sqlcipher-$(SQLCIPHER_VERSION)

ffmpeg: $(BUILD_PATH)/ffmpeg-$(FFMPEG_VERSION)

$(DOWNLOAD_PATH)/ffmpeg-$(FFMPEG_VERSION).tar.xz:
	wget --no-check-certificate https://ffmpeg.org/releases/ffmpeg-$(FFMPEG_VERSION).tar.xz -O $(DOWNLOAD_PATH)/ffmpeg-$(FFMPEG_VERSION).tar.xz

$(BUILD_PATH)/ffmpeg-$(FFMPEG_VERSION): $(DOWNLOAD_PATH)/ffmpeg-$(FFMPEG_VERSION).tar.xz
	# prepare
	rm -r -f $(BUILD_PATH)/ffmpeg-*
	tar xvf $(DOWNLOAD_PATH)/ffmpeg-$(FFMPEG_VERSION).tar.xz
	# build
	cd ffmpeg-$(FFMPEG_VERSION) && ./configure --disable-shared --enable-static --disable-programs --disable-ffmpeg --disable-ffplay --disable-ffprobe --disable-doc --disable-htmlpages --disable-manpages --disable-podpages --disable-txtpages --disable-yasm --disable-everything --enable-encoder=mpeg4 --enable-decoder=mpeg4 --prefix="$(abspath $(LIBS_PATH))"
	cd ffmpeg-$(FFMPEG_VERSION) && make -j
	# copy files
	cd ffmpeg-$(FFMPEG_VERSION) && make install prefix="$(abspath $(BUILD_PATH)/ffmpeg-$(FFMPEG_VERSION).tmp)"
	$(call CLEAN_PC_FILES,$(abspath $(BUILD_PATH)/ffmpeg-$(FFMPEG_VERSION).tmp))
	# cleanup
	rm -r -f ffmpeg-$(FFMPEG_VERSION)
	mv $(BUILD_PATH)/ffmpeg-$(FFMPEG_VERSION).tmp $(BUILD_PATH)/ffmpeg-$(FFMPEG_VERSION)

rapidjson: $(BUILD_PATH)/rapidjson-$(RAPIDJSON_VERSION)

$(DOWNLOAD_PATH)/rapidjson-$(RAPIDJSON_VERSION).tar.gz:
	wget --no-check-certificate https://github.com/Tencent/rapidjson/archive/v$(RAPIDJSON_VERSION).tar.gz -O $(DOWNLOAD_PATH)/rapidjson-$(RAPIDJSON_VERSION).tar.gz

$(BUILD_PATH)/rapidjson-$(RAPIDJSON_VERSION): $(DOWNLOAD_PATH)/rapidjson-$(RAPIDJSON_VERSION).tar.gz
	# prepare
	rm -r -f $(BUILD_PATH)/rapidjson-*
	tar xvf $(DOWNLOAD_PATH)/rapidjson-$(RAPIDJSON_VERSION).tar.gz
	# build
	#mkdir -p rapidjson-$(RAPIDJSON_VERSION)/build
	#cd rapidjson-$(RAPIDJSON_VERSION)/build && cmake .. -G"MSYS Makefiles"
	#cd rapidjson-$(RAPIDJSON_VERSION)/build && make
	# copy files
	mkdir -p $(BUILD_PATH)/rapidjson-$(RAPIDJSON_VERSION).tmp/include
	cp -r rapidjson-$(RAPIDJSON_VERSION)/include/* $(BUILD_PATH)/rapidjson-$(RAPIDJSON_VERSION).tmp/include/
	# cleanup
	rm -r -f rapidjson-$(RAPIDJSON_VERSION)
	mv $(BUILD_PATH)/rapidjson-$(RAPIDJSON_VERSION).tmp $(BUILD_PATH)/rapidjson-$(RAPIDJSON_VERSION)

xapian: $(BUILD_PATH)/xapian-core-$(XAPIAN_VERSION)

$(DOWNLOAD_PATH)/xapian-core-$(XAPIAN_VERSION).tar.xz:
	wget --no-check-certificate https://oligarchy.co.uk/xapian/$(XAPIAN_VERSION)/xapian-core-$(XAPIAN_VERSION).tar.xz -O $(DOWNLOAD_PATH)/xapian-core-$(XAPIAN_VERSION).tar.xz

$(BUILD_PATH)/xapian-core-$(XAPIAN_VERSION): $(DOWNLOAD_PATH)/xapian-core-$(XAPIAN_VERSION).tar.xz
	# prepare
	rm -r -f $(BUILD_PATH)/xapian-core-*
	tar xvf $(DOWNLOAD_PATH)/xapian-core-$(XAPIAN_VERSION).tar.xz
	# build
	cd xapian-core-$(XAPIAN_VERSION) && ./configure --disable-shared --enable-static --disable-documentation --prefix="$(abspath $(LIBS_PATH))"
	cd xapian-core-$(XAPIAN_VERSION) && make -j libxapian.la
	# copy files
	cd xapian-core-$(XAPIAN_VERSION) && make install-libLTLIBRARIES install-incHEADERS install-xapianincludeHEADERS install-nodist_xapianincludeHEADERS install-nodist_pkgconfigDATA prefix="$(abspath $(BUILD_PATH)/xapian-core-$(XAPIAN_VERSION).tmp)"
	$(call CLEAN_PC_FILES,$(abspath $(BUILD_PATH)/xapian-core-$(XAPIAN_VERSION).tmp))
	# cleanup
	rm -r -f xapian-core-$(XAPIAN_VERSION)
	mv $(BUILD_PATH)/xapian-core-$(XAPIAN_VERSION).tmp $(BUILD_PATH)/xapian-core-$(XAPIAN_VERSION)

jsonc: $(BUILD_PATH)/json-c-$(JSON_C_VERSION)

$(BUILD_PATH)/json-c-$(JSON_C_VERSION):
	# prepare
	rm -r -f $(BUILD_PATH)/json-c-*
	[ -d "json-c-$(JSON_C_VERSION)" ] || git clone https://github.com/json-c/json-c.git --depth=1 --branch json-c-$(JSON_C_VERSION) "json-c-$(JSON_C_VERSION)"
	# build
	mkdir -p json-c-$(JSON_C_VERSION)/build
	cd json-c-$(JSON_C_VERSION)/build && cmake .. -G"MSYS Makefiles" -Wno-dev -DCMAKE_BUILD_TYPE="release" -DBUILD_SHARED_LIBS=off -DBUILD_STATIC_LIBS=on -DBUILD_TESTING=off -DCMAKE_CXX_FLAGS="-D__MINGW_USE_VC2005_COMPAT" -DCMAKE_INSTALL_PREFIX="$(abspath $(BUILD_PATH)/json-c-$(JSON_C_VERSION).tmp)"
	cmake --build json-c-$(JSON_C_VERSION)/build --parallel
	# copy files
	cmake --install json-c-$(JSON_C_VERSION)/build
	# fix package config file (bug in json-c with CMAKE_INSTALL_PREFIX)
	sed -i -E "s|=.*json-c-$(JSON_C_VERSION)\.tmp|=$$(cygpath -m '$(abspath $(LIBS_PATH))')|g" "$(abspath $(BUILD_PATH)/json-c-$(JSON_C_VERSION).tmp)/lib/pkgconfig/json-c.pc"
	# cleanup
	rm -r -f json-c-$(JSON_C_VERSION)
	mv $(BUILD_PATH)/json-c-$(JSON_C_VERSION).tmp $(BUILD_PATH)/json-c-$(JSON_C_VERSION)

botan: $(BUILD_PATH)/botan-$(BOTAN_VERSION)

$(BUILD_PATH)/botan-$(BOTAN_VERSION):
	# prepare
	pacman --needed --noconfirm -S python3
	rm -r -f $(BUILD_PATH)/botan-*
	[ -d "botan-$(BOTAN_VERSION)" ] || git clone https://github.com/randombit/botan.git --depth=1 --branch $(BOTAN_VERSION) "botan-$(BOTAN_VERSION)"
	# build
	if [ $(MSYSTEM) = "MINGW32" ] ; then cd botan-$(BOTAN_VERSION) && ./configure.py --os=mingw --cpu=x86_32 --build-targets=static --without-documentation --no-install-python-module --extra-cxxflags="-D__MINGW_USE_VC2005_COMPAT" --prefix="$(abspath $(LIBS_PATH))" ; fi
	if [ $(MSYSTEM) = "MINGW64" ] ; then cd botan-$(BOTAN_VERSION) && ./configure.py --os=mingw --cpu=x86_64 --build-targets=static --without-documentation --no-install-python-module --prefix="$(abspath $(LIBS_PATH))" ; fi
	cd botan-$(BOTAN_VERSION) && make -j libs
	# copy files
	# botan has fix in configure and cannot be installed in an other directory than configured prefix
	#cd botan-$(BOTAN_VERSION) && make install
	# use install.py to install in temp directory (see install target in Makefile)
	cd botan-$(BOTAN_VERSION) && python3 src/scripts/install.py --prefix="$(abspath $(BUILD_PATH)/botan-$(BOTAN_VERSION).tmp)" --build-dir="build" --bindir="$(abspath $(BUILD_PATH)/botan-$(BOTAN_VERSION).tmp)/bin" --libdir="$(abspath $(BUILD_PATH)/botan-$(BOTAN_VERSION).tmp)/lib" --docdir="share/doc" --includedir="include"
	$(call CLEAN_PC_FILES,$(abspath $(BUILD_PATH)/botan-$(BOTAN_VERSION).tmp))
	# cleanup
	rm -r -f botan-$(BOTAN_VERSION)
	mv $(BUILD_PATH)/botan-$(BOTAN_VERSION).tmp $(BUILD_PATH)/botan-$(BOTAN_VERSION)

asio: $(BUILD_PATH)/asio-$(ASIO_VERSION)

$(BUILD_PATH)/asio-$(ASIO_VERSION):
	# prepare
	rm -r -f $(BUILD_PATH)/asio-*
	pacman --needed --noconfirm -S autoconf automake
	[ -d "asio-$(ASIO_VERSION)" ] || git clone https://github.com/chriskohlhoff/asio.git --depth=1 --branch asio-$(ASIO_VERSION) "asio-$(ASIO_VERSION)"
	# build
	cd asio-$(ASIO_VERSION)/asio && autoreconf -fi
	cd asio-$(ASIO_VERSION)/asio && ./configure --prefix="$(abspath $(LIBS_PATH))"
	# copy files
	cd asio-$(ASIO_VERSION)/asio && make -C include install-nobase_includeHEADERS prefix="$(abspath $(BUILD_PATH)/asio-$(ASIO_VERSION).tmp)"
	cd asio-$(ASIO_VERSION)/asio && make install-noarch_pkgconfigDATA prefix="$(abspath $(BUILD_PATH)/asio-$(ASIO_VERSION).tmp)"
	$(call CLEAN_PC_FILES,$(abspath $(BUILD_PATH)/asio-$(ASIO_VERSION).tmp))
	# cleanup
	rm -r -f asio-$(ASIO_VERSION)
	mv $(BUILD_PATH)/asio-$(ASIO_VERSION).tmp $(BUILD_PATH)/asio-$(ASIO_VERSION)

rnp: $(BUILD_PATH)/rnp-$(RNP_VERSION)

$(BUILD_PATH)/rnp-$(RNP_VERSION):
	# prepare
	rm -r -f $(BUILD_PATH)/rnp-*
	[ -d "rnp-$(RNP_VERSION)" ] || git clone https://github.com/rnpgp/rnp.git --depth=1 --branch v$(RNP_VERSION) --recurse-submodules --shallow-submodules "rnp-$(RNP_VERSION)"
	# build
	mkdir -p rnp-$(RNP_VERSION)/build
	cd rnp-$(RNP_VERSION)/build && cmake .. -G"MSYS Makefiles" -Wno-dev -DCMAKE_INSTALL_PREFIX="$(abspath $(LIBS_PATH))" -DBUILD_SHARED_LIBS=yes -DBUILD_TESTING=off -DCMAKE_CXX_FLAGS="-D__MINGW_USE_VC2005_COMPAT -D__STDC_FORMAT_MACROS" -DBZIP2_INCLUDE_DIR="$(abspath $(BUILD_PATH)/bzip2-$(BZIP2_VERSION)/include)" -DBZIP2_LIBRARY_RELEASE="$(abspath $(BUILD_PATH)/bzip2-$(BZIP2_VERSION)/lib/libbz2.a)" -DBZIP2_LIBRARIES="$(abspath $(BUILD_PATH)/bzip2-$(BZIP2_VERSION)/lib/libbz2.a)" -DZLIB_INCLUDE_DIR="$(abspath $(BUILD_PATH)/zlib-$(ZLIB_VERSION)/include)" -DZLIB_LIBRARY="$(abspath $(BUILD_PATH)/zlib-$(ZLIB_VERSION)/lib/libz.a)" -DJSON-C_INCLUDE_DIR="$(abspath $(BUILD_PATH)/json-c-$(JSON_C_VERSION)/include/json-c)" -DJSON-C_LIBRARY="$(abspath $(BUILD_PATH)/json-c-$(JSON_C_VERSION)/lib/libjson-c.a)" -DBOTAN_INCLUDE_DIR="$(abspath $(BUILD_PATH)/botan-$(BOTAN_VERSION)/include/botan-$(firstword $(subst ., ,$(BOTAN_VERSION))))" -DBOTAN_LIBRARY="$(abspath $(BUILD_PATH)/botan-$(BOTAN_VERSION)/lib/libbotan-$(firstword $(subst ., ,$(BOTAN_VERSION))).a)"
	cmake --build rnp-$(RNP_VERSION)/build --parallel
	# copy files
	cmake --install rnp-$(RNP_VERSION)/build --component development --prefix "$(abspath $(BUILD_PATH)/rnp-$(RNP_VERSION).tmp)"
	# cleanup
	rm -r -f rnp-$(RNP_VERSION)
	mv $(BUILD_PATH)/rnp-$(RNP_VERSION).tmp $(BUILD_PATH)/rnp-$(RNP_VERSION)

cleanlibs:
	@echo "Remove libs"
	@rm -r -f $(LIBS_PATH);

copylibs:
	@echo "Copy libs";
	@set -e; \
	rm -rf "$(LIBS_PATH)"; \
	mkdir -p "$(LIBS_PATH)"; \
	cp -p "$(BUILD_PATH)/gcc-version" "$(LIBS_PATH)"; \
	for dir in "$(BUILD_PATH)"/*/; do \
		case "$$dir" in *.tmp/) continue ;; esac; \
		echo "$$dir"; \
		cp -rp "$$dir." "$(LIBS_PATH)"; \
	done; \
	if grep -rlE '([=[:space:]'\'']|-[IL])(/[a-zA-Z]/|[a-z]:/)|/build/' "$(LIBS_PATH)" --include="*.pc" --include="*.la"; then \
		echo "ERROR: Found not cleaned path in .pc or .la file!"; \
		exit 1; \
	fi
