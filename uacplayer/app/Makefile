CURRENT_PATH = $(shell pwd)
CROSS_COMPILE ?=arm-linux-gnueabihf-
CC  = $(CROSS_COMPILE)gcc
CPP = $(CROSS_COMPILE)g++
AR  = $(CROSS_COMPILE)ar

ALKAID_PATH ?= ./../../..
CHIP ?= ssd20x
LIBRARY ?= dynamic
DISPLAY ?= panel
GIT_COMMIT_INFO:="uacplayer library version: git_commit.$(shell cd ../../; git log -n 1 --format="%h") build_time.$(shell date +%Y%m%d)"

ifeq ($(CHIP),ssd20x)
    #$(shell echo "choose chip ssd20x" > $(shell tty))
else ifeq ($(CHIP),ss268)
    #$(shell echo "choose chip ss268" > $(shell tty))
else ifeq ($(CHIP),ss22x)
    #$(shell echo "choose chip ss22x" > $(shell tty))
else
    $(error invalid chip type)
endif

COM_FLAGS = -Wall -O2 -fPIC -mtune=cortex-a7 -march=armv7-a -mfpu=neon-vfpv4 -mfloat-abi=hard -mthumb-interwork -marm -g -funwind-tables -ffunction-sections -rdynamic
COM_FLAGS += -DSSPLAYER_VERSION=$(GIT_COMMIT_INFO)
C_FLAGS   = $(COM_FLAGS) #-std=c11
CPP_FLAGS = $(COM_FLAGS) -std=c++11

INCLUDES  = -I. -I$(CURRENT_PATH)/../include -I$(CURRENT_PATH)/../../ffmpeg-4.1.3/host/include -I$(CURRENT_PATH)/../../3rdparty/include/sstar -I$(ALKAID_PATH)/project/release/include -I./../src/uac/

ifeq ($(CHIP),ss268)
C_FLAGS   += -DCHIP_IS_SS268
CPP_FLAGS += -DCHIP_IS_SS268
SDK_LIB    = -L$(ALKAID_PATH)/project/release/chip/m6/dispcam/common/glibc/9.1.0/mi_libs/dynamic -L$(ALKAID_PATH)/project/release/chip/m6/sigma_common_libs/glibc/9.1.0/dynamic
APP_LIB    = -L. -L$(CURRENT_PATH)/../../3rdparty/lib/9.1.0
else ifeq ($(CHIP),ss22x)
C_FLAGS   += -DCHIP_IS_SS22X
CPP_FLAGS += -DCHIP_IS_SS22X
SDK_LIB    = -L$(ALKAID_PATH)/project/release/dispcam/p3/common/glibc/9.1.0/mi_libs/dynamic -L$(ALKAID_PATH)/project/release/dispcam/p3/common/glibc/9.1.0/ex_libs/dynamic
APP_LIB    = -L. -L$(CURRENT_PATH)/../../3rdparty/lib/9.1.0
else ifeq ($(CHIP),ssd20x)
C_FLAGS   += -DCHIP_IS_SSD20X -DUAC_SUPPORT
CPP_FLAGS += -DCHIP_IS_SSD20X -DUAC_SUPPORT
SDK_LIB    = -L$(ALKAID_PATH)/project/release/nvr/i2m/common/glibc/8.2.1/mi_libs/dynamic
APP_LIB    = -L. -L$(CURRENT_PATH)/../../3rdparty/lib/8.2.1
endif

TARGET_NAME = uacplayer


UAC_SRCS = $(wildcard $(CURRENT_PATH)/../src/uac/*.c)
APP_SRCS = $(wildcard $(CURRENT_PATH)/*.c)
APP_OBJS = $(patsubst %.c, %.o, $(APP_SRCS))

PLAYER_SRCS = $(wildcard $(CURRENT_PATH)/../src/*.c) $(UAC_SRCS)
PLAYER_OBJS = $(patsubst %.c, %.o, $(PLAYER_SRCS))
PLAYER_NAME = uacplayer
INSTALL_PATH = $(CURRENT_PATH)/../libs

ifeq ($(DISPLAY),hdmi)
C_FLAGS   += -DSUPPORT_HDMI
CPP_FLAGS += -DSUPPORT_HDMI
SDK_LIB   += -lmi_hdmi
else
SDK_LIB   += -lmi_panel
endif

ifeq ($(CHIP),ss268)
SDK_LIB += -lmi_vdec -lmi_scl -lmi_disp -lmi_ao -lmi_gfx -lmi_sys -lmi_common -lcam_os_wrapper
else ifeq ($(CHIP),ss22x)
SDK_LIB += -lmi_disp -lmi_ao -lmi_gfx -lmi_sys -lmi_common -lmi_divp -lcam_os_wrapper
else ifeq ($(CHIP),ssd20x)
SDK_LIB += -lmi_vdec -lmi_divp -lmi_disp -lmi_ao -lmi_gfx -lmi_sys -lmi_common -lmi_ai
endif

FFMPEG_LIB  = -L$(CURRENT_PATH)/../../ffmpeg-4.1.3/host/dynamic
FFMPEG_LIB += -lavformat -lavcodec -lavutil -lswscale -lswresample

APP_LIB    += -luacplayer
APP_LIB    += $(FFMPEG_LIB) $(SDK_LIB)
APP_LIB    += -lssl -lcrypto

.PHONY: prepare all finish

all: prepare shared_lib static_lib $(TARGET_NAME) finish

prepare:
	@echo
	@echo ">>>>========================================================"
	@echo "TARGET_NAME = $(TARGET_NAME)"
	@echo

finish:
	@echo "make done"
	@echo "<<<<========================================================"
	@echo

static_lib: $(PLAYER_OBJS)
	@echo "generate lib$(PLAYER_NAME).a"
	@$(AR) -rcs lib${PLAYER_NAME}.a $(PLAYER_OBJS)

shared_lib: $(PLAYER_OBJS)
	@echo "generate lib$(PLAYER_NAME).so"
	@$(CC) -fPIC -shared -o lib$(PLAYER_NAME).so $(PLAYER_OBJS) $(FFMPEG_LIB) $(SDK_LIB)

clean:
	@echo "clean $(PLAYER_NAME) object"
	rm -rf $(APP_OBJS) $(PLAYER_OBJS)
	@rm -rf $(TARGET_NAME) lib$(PLAYER_NAME).so lib$(PLAYER_NAME).a

distclean: clean
	@echo "clean $(PLAYER_NAME) libs"
	@rm -rf $(PLAYER_OBJS)
	@rm -rf $(INSTALL_PATH)
	@rm -rf lib$(PLAYER_NAME).*

install:
	@echo "install $(PLAYER_NAME) object"
	mkdir -p ${INSTALL_PATH}/include
	mkdir -p ${INSTALL_PATH}/dynamic
	mkdir -p ${INSTALL_PATH}/static
	cp -rf $(CURRENT_PATH)/../include/interface.h $(INSTALL_PATH)/include
	cp -rf $(CURRENT_PATH)/../include/player.h $(INSTALL_PATH)/include
	cp -rf lib$(PLAYER_NAME).so $(INSTALL_PATH)/dynamic
	cp -rf lib$(PLAYER_NAME).a $(INSTALL_PATH)/static

$(TARGET_NAME): $(APP_OBJS) $(APP_SRCS)
	@echo "generate $@"
	@$(CC) -o $@ $(APP_OBJS) $(APP_LIB) -ldl -lm -lpthread

.SUFFIXES: .c .o
%.o : %.c
	@echo "compile $@"
	@$(CC) $(C_FLAGS) $(INCLUDES) $(FFMPEG_LIB) $(SDK_LIB) -lm -ldl -lpthread -c $< -o $@

