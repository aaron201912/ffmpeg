# ffmpeg
## DownLoad
    git clone https://github.com/aaron201912/ffmpeg.git 

## BuildCodeFlow
    1. cd ffmpeg-4.1.3
    2. for ssd20x use gcc toolchain 8.2.1 and run: sh config_for_ssd20x.sh
       for ssd222 use gcc toolchain 9.1.0 and run: sh config_for_ssd222.sh
    3. make clean;make -j4
    4. make install

    注: 生成的库文件安装路径在ffmpeg-4.1.3/host下, 开启https/dash等功能需要3rdparty/lib中的第三方动态库支持,并使能相应编译选项.
    1. 编辑ffmpeg_build.sh, 增加--enable-openssl编译选项可开启https协议支持, 默认打开. 如需关闭去掉该编译选项.
       使用1.1.0以上的openssl库时, 需编辑configure, 找到enabled openssl, 增加check_lib openssl openssl/ssl.h OPENSSL_init_ssl -lssl -lcrypto判断条件

    2. ffmpeg如果需要支持硬解4K视频, 则在config_for_ssd20x.sh的编译选项中增加如下参数：
    --extra-cflags="-I./../sstar/include -I./../3rdparty/include -g -rdynamic -funwind-tables -ffunction-sections -DSUPPORT_4K"
