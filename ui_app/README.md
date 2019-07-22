1. 更新master_i6_ipc code至最新版本
2. 编译kernel
	export ARCH=arm
	export CROSS_COMPILE=arm-linux-gnueabihf-
	export PATH=/opt/ToolChain/gcc-arm-8.2-2018.08-x86_64-arm-linux-gnueabihf/bin:$PATH
	make infinity2m_ssc011a_s01a_minigui_defconfig
	make -j8
	将 arch/arm/boot/uImage.xz 拷贝至 project/release/nvr/i2m/011A/glibc/8.2.1/bin/kernel/nor
3. 设置sdk环境
	vi project/release/customer_tailor/nvr_i2m_display_glibc_tailor.mk
	添加 verify_app:=enable
	在project目录
	./setup_config.sh configs/nvr/i2m/8.2.1/nor.glibc-squashfs.011a.64
	
	将 sdk/verify/application/smarttalk/image.mk 替换为 RunEnv/image.mk 
	将 sdk/verify/application/smarttalk/RunFile 替换为 RunEnv/RunFile 
	
	编译sdk
	
4. 编译app
	在player下 make clean;make
	
5. 运行
	将生成 playerWnd 拷贝至 mount目录，将RunEnv/MountFile 拷贝至 playerWnd同级目录
	