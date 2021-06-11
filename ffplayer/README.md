## ffplayer
    A simple player based on ffmpeg&amp;ffplay.

    编译及运行方法：
    1. 将ffmpeg整个文件夹放到与project同一级目录下
    2. cd ffmpeg/ffplayer/app
    3. make DISPLAY=panel CHIP=ssd20x; 也可通过ALKAID_PATH参数指定project路径如: make DISPLAY=panel CHIP=ssd20x ALKAID_PATH=/home/user/ssd20x
    4. ffmpeg/ffplayer/app目录下会生成ssplayer可执行文件, 当前目录下会生成动态库文件libssplayer.so, 需要与ffmpeg库一起拷贝到板子中
    5. export动态库后, 在目标板上运行 ./ssplayer resources/cuc.flv

    注: 默认使用panel_1024_600显示, 平台选择SSD20X. 修改编译参数可配置hdmi输出, 切换SS268平台
    make DISPLAY=hdmi CHIP=ss268
