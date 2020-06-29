# ffmpeg
DownLoad:
    git clone https://github.com/aaron201912/ffmpeg.git

BuildCodeFlow:
    1. cd ffmpeg-4.1.3
    2. ./ffmpeg_build.sh 
    3. make clean;make -j4
    4. make install

注: 生成的库文件安装路径在ffmpeg-4.1.3/host下, 默认编译选项支持https协议, 需要3rdparty/lib中的动态库支持. 去掉ffmpeg_build.sh中--enable-openssl编译选项可关闭https支持.
