#!/bin/bash
git_commit=`cd ../; git log -n 1 --format="%h"`
build_time=`date +%Y%m%d`
ffmpeg_info=\""ffmpeg_version..git_commit:${git_commit}..build_time:${build_time}"\"

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
--disable-decoder=ssh264_ss268 \
--disable-decoder=sshevc_ss268 \
--shlibdir=host/dynamic \
--libdir=host/static \
--pkg-config=pkg-config \
--extra-cflags="-I./../3rdparty/include -g -rdynamic -funwind-tables -ffunction-sections -DFFMPEG_LIBRARY_VERSION=${ffmpeg_info}" \
--extra-ldflags="-L./../3rdparty/lib/9.1.0"
