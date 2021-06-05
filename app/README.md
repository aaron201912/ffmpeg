## 编译说明

    1. cd ffmpeg/app
	2. make clean; make DISPLAY=panel CHIP=ssd20x
	3. 将生成的mmplayer文件拷贝到目标板, 运行./mmplayer filename.mp4

    注: 示例编译选项默认选择使用panel输出, 硬件平台类型选择为SSD20X, 修改参数可选择hdmi输出, 切换到SS268平台
	make DISPLAY=hdmi CHIP=ss268