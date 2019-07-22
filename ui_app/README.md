1. 更新master_i6_ipc code至最新版本。

2. 编译kernel
	export ARCH=arm
	export CROSS_COMPILE=arm-linux-gnueabihf-
	export PATH=/opt/ToolChain/gcc-arm-8.2-2018.08-x86_64-arm-linux-gnueabihf/bin:$PATH
	make infinity2m_ssc011a_s01a_minigui_defconfig
	make -j8	
	将生成的uImage.xz拷贝到 project/release/nvr/i2m/011A/glibc/8.2.1/bin/kernel/nor 目录下
	
3. 设置sdk编译环境
	vi project/release/customer_tailor/nvr_i2m_display_glibc_tailor.mk
	添加 verify_app:=enable 并保存
	
	在project目录下：
	./setup_config.sh configs/nvr/i2m/8.2.1/nor.glibc-squashfs.011a.64
	
	将 sdk/verify/application/smarttalk/image.mk 替换为 RunEnv/image.mk
	清空 sdk/verify/application/smarttalk/RunFile， 将 RunEnv/RunFile 下文件拷贝至该目录
	
	将RunEnv/MountFile 放置在app同级目录

4. 编译sdk并烧录
	make image

5. 编译app
	cd player;
	make clean;make
	将生成的 playerWnd 拷贝至mount目录
	
	
	