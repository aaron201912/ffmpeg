#!/bin/bash
if [ -e "$1" ]; then
    alkaid_path=$1
else
    alkaid_path=./../..
fi
echo "alkaid_path = ${alkaid_path}"

cflags+=" -I${alkaid_path}/project/release/include"
cflags+=" -I./../3rdparty/include"
cflags+=" -I./../3rdparty/include/sstar"
echo "cflags = ${cflags}"

ldflags+=" -L${alkaid_path}/project/release/chip/m6/xvr/common/glibc/9.1.0/mi_libs/dynamic"
ldflags+=" -L./../3rdparty/lib/9.1.0"
echo "ldflags = ${ldflags}"

./configure \
--prefix=host \
--disable-ffmpeg \
--disable-ffprobe \
--disable-ffplay \
--disable-programs \
--disable-symver \
--disable-doc \
--disable-htmlpages \
--disable-manpages \
--disable-podpages \
--disable-txtpages \
--disable-avresample \
--disable-avfilter \
--disable-avdevice \
--disable-postproc \
--enable-swscale \
--enable-swresample \
--enable-avcodec \
--enable-avformat \
--enable-small \
--enable-cross-compile \
--arch=arm \
--target-os=linux \
--cross-prefix=arm-linux-gnueabihf- \
--enable-shared \
--disable-gpl \
--enable-nonfree \
--enable-openssl \
--enable-pthreads \
--enable-error-resilience \
--enable-debug \
--disable-encoders \
--disable-hwaccels \
--disable-muxers \
--disable-indevs \
--disable-outdevs \
--disable-devices \
--disable-filters \
--disable-decoder=ssh264 \
--disable-decoder=sshevc \
--shlibdir=host/dynamic \
--libdir=host/static \
--pkg-config=pkg-config \
--extra-cflags="${cflags} -g -rdynamic -funwind-tables -ffunction-sections" \
--extra-ldflags="${ldflags}"
