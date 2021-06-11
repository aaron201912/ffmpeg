CROSS_COMPILE ?=arm-linux-gnueabihf-
CC  = $(CROSS_COMPILE)gcc
CPP = $(CROSS_COMPILE)g++
AR  = $(CROSS_COMPILE)ar

ALKAID_PATH ?= ./../..
CHIP ?= ssd20x
LIBRARY ?= dynamic
DISPLAY ?= panel

ifeq ($(CHIP),ssd20x)
    #$(shell echo "choose chip ssd20x" > $(shell tty))
else ifeq ($(CHIP),ss268)
    #$(shell echo "choose chip ss268" > $(shell tty))
else
    $(error invalid chip type)
endif

COM_FLAGS = -Wall -O2 -fPIC -mcpu=cortex-a9 -mfpu=neon-fp16 -mfloat-abi=hard -mthumb-interwork -marm
C_FLAGS	   = $(COM_FLAGS) -std=c11
CPP_FLAGS  = $(COM_FLAGS) -std=c++11

TARGET_NAME  = mmplayer

CPP_SRCS  =
C_SRCS    =  SsPlayer.c

CPP_OBJS = $(patsubst %.cpp, %.cpp.o, $(CPP_SRCS))
C_OBJS = $(patsubst %.c, %.c.o, $(C_SRCS))

INCLUDES  = -I../ffmpeg-4.1.3/host/include/
INCLUDES += -I$(ALKAID_PATH)/project/release/include

LIB_PATH  = -L../ffmpeg-4.1.3/host/$(LIBRARY)
LIB_NAME  = -lavformat -lavcodec -lavutil -lswscale -lswresample
LIB_NAME += -lssl -lcrypto

ifeq ($(DISPLAY),hdmi)
C_FLAGS   += -DSUPPORT_HDMI
CPP_FLAGS += -DSUPPORT_HDMI
LIB_NAME  += -lmi_hdmi
else
LIB_NAME  += -lmi_panel
endif

ifeq ($(CHIP),ss268)
C_FLAGS   += -DCHIP_IS_SS268
CPP_FLAGS += -DCHIP_IS_SS268
LIB_PATH  += -L. -L$(ALKAID_PATH)/project/release/chip/m6/xvr/common/glibc/9.1.0/mi_libs/dynamic -L../3rdparty/lib/9.1.0
LIB_NAME  += -lmi_vdec -lmi_ao -lmi_disp -lmi_sys -lmi_common -lcam_os_wrapper -ldl
else ifeq ($(CHIP),ssd20x)
C_FLAGS   += -DCHIP_IS_SSD20X
CPP_FLAGS += -DCHIP_IS_SSD20X
LIB_PATH  += -L. -L$(ALKAID_PATH)/project/release/nvr/i2m/common/glibc/8.2.1/mi_libs/dynamic -L../3rdparty/lib/8.2.1
LIB_NAME  += -lmi_vdec -lmi_ao -lmi_divp -lmi_disp -lmi_sys -lmi_common -ldl
endif

.PHONY: all prepare clean

all: prepare $(TARGET_NAME) finish

prepare:
	@echo
	@echo ">>>>========================================================"
	@echo "TARGET_NAME = $(TARGET_NAME)"
	@echo


clean:
	@rm -rf $(CPP_OBJS)
	@rm -rf $(C_OBJS)
	@rm -rf $(TARGET_NAME)

finish:
	@echo "make done"
	@echo "<<<<========================================================"
	@echo

$(TARGET_NAME): $(CPP_OBJS) $(CPP_SRCS) $(C_OBJS) $(C_SRCS)
	@echo "generate $@"
	@$(CC) -o $@ $(C_OBJS) $(CPP_OBJS) $(LIB_PATH) $(LIB_NAME) -lm -lpthread

%.c.o : %.c
	@echo "compile $@"
	@$(CC) $(C_FLAGS) $(INCLUDES) $(DEFINES) -c $< -o $@

%.cpp.o : %.cpp
	@echo "compile $@"
	@$(CPP) $(CPP_FLAGS) $(INCLUDES) $(DEFINES) -c $< -o $@
