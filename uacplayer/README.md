## uacplayer
    A simple player based on ffmpeg&amp;uacplay.
    针对ssd20x的player demo.
    audio pipeline:  ffmpeg将audio 解码成pcm后送给uac输出，uac设备收到pcm数据后做一些音效处理后再通过I2S送回给ssd20x，ssd20x最终将audio和video送给HDMI输出
    编译及运行方法：
    1. 将ffmpeg整个文件夹放到与project同一级目录下
    2. cd ffmpeg/uacplayer/app
    3. make DISPLAY=hdmi CHIP=ssd20x; 也可通过ALKAID_PATH参数指定project路径如: make DISPLAY=hdmi CHIP=ssd20x ALKAID_PATH=/home/user/ssd20x
    4. ffmpeg/uacplayer/app目录下会生成ssplayer可执行文件, 当前目录下会生成动态库文件libuacplayer.so, 需要与ffmpeg库一起拷贝到板子中
    5. export动态库后, 在目标板上运行 ./ssplayer HD-TCL-10S.mp4
    
