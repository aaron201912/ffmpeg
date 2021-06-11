# ffmpeg
## DownLoad
    git clone https://github.com/aaron201912/ffmpeg.git 

## BuildCodeFlow
    1. 拷贝ffmpeg整个文件夹到project同一级目录
    2. cd ffmpeg/ffmpeg-4.1.3
    3. for ssd20x use gcc toolchain 8.2.1 and run: sh config_for_ssd20x.sh
       for ssd222 use gcc toolchain 9.1.0 and run: sh config_for_ssd222.sh
       for ss268  use gcc toolchain 9.1.0 and run: sh config_for_ss268.sh
    4. make clean;make -j4;make install

    注: 生成的库文件安装路径在ffmpeg-4.1.3/host下, 开启https/dash等功能需要3rdparty/lib中的第三方动态库支持,并使能相应编译选项.
    1. 编辑config_for_ssd20x.sh, 增加--enable-openssl编译选项可开启https协议支持, 默认打开. 如需关闭去掉该编译选项.
       使用1.1.0以上的openssl库时, 需编辑configure, 找到enabled openssl, 增加check_lib openssl openssl/ssl.h OPENSSL_init_ssl -lssl -lcrypto判断条件

    2. ffmpeg如果需要支持硬解4K视频, 则在config_for_ssd20x.sh的编译选项中增加如下参数：
    cflags+=" -DSUPPORT_4K"

    3. 如果ffmpeg与project不在同一级目录, 配置ffmpeg时需要手动设置project路径, 否则编译不过, 如:
       sh config_for_ssd20x.sh /home/user/ssd20x

## Demo Note
    1. ffmpeg/app文件下的demo介绍调用ffmpeg接口串接简单播放音视频流程
    2. ffmpeg/ffplayer文件夹的demo实现比较完整播放器功能
    3. ffmpeg/ui_app文件夹已暂停维护
