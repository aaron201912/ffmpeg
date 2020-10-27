CURRENT_PATH = $(shell pwd)
CROSS_COMPILE ?=arm-linux-gnueabihf-
CC  = $(CROSS_COMPILE)gcc
CPP = $(CROSS_COMPILE)g++
AR  = $(CROSS_COMPILE)ar

# config hdmi or panel display
CONFIG_SUPPORT_HDMI = 

COM_FLAGS = -Wall -O2 -fPIC -mcpu=cortex-a9 -mfpu=neon-fp16 -mfloat-abi=hard -mthumb-interwork -marm -g -funwind-tables -ffunction-sections -rdynamic
C_FLAGS   = $(COM_FLAGS) -std=c11
CPP_FLAGS = $(COM_FLAGS) -std=c++11 

INCLUDES  = -I. -I$(CURRENT_PATH)/../include -I$(CURRENT_PATH)/../../ffmpeg-4.1.3/host/include -I$(CURRENT_PATH)/../../sstar/include
LIB_PATH  = -L. -L$(CURRENT_PATH)/../../ffmpeg-4.1.3/host/dynamic -L$(CURRENT_PATH)/../../sstar/lib -L$(CURRENT_PATH)/../../3rdparty/lib

TARGET_NAME  = ssplayer

CPP_SRCS  =  $(wildcard $(CURRENT_PATH)/*.cpp)
CPP_SRCS +=  $(wildcard $(CURRENT_PATH)/../src/*.cpp)
C_SRCS    =  $(wildcard $(CURRENT_PATH)/*.c)
C_SRCS   +=  $(wildcard $(CURRENT_PATH)/../src/*.c)

CPP_OBJS  = $(patsubst %.cpp, %.cpp.o, $(CPP_SRCS))
C_OBJS    = $(patsubst %.c, %.c.o, $(C_SRCS))

LIB_NAME  = -lavformat -lavcodec -lavutil -lswscale -lswresample
LIB_NAME += -lmi_vdec -lmi_divp -lmi_disp -lmi_ao -lmi_gfx -lmi_sys -lmi_common
LIB_NAME += -lssl -lcrypto
#LIB_NAME += -lxml2

ifeq ($(CONFIG_SUPPORT_HDMI),enable)
C_FLAGS   += -DSUPPORT_HDMI
CPP_FLAGS += -DSUPPORT_HDMI
LIB_NAME  += -lmi_hdmi
else
LIB_NAME  += -lmi_panel
endif

.PHONY: all prepare clean

all: prepare $(TARGET_NAME) finish

prepare:
	@echo
	@echo ">>>>========================================================"
	@echo "TARGET_NAME = $(TARGET_NAME)"
	@echo

clean:
	@rm -Rf $(CPP_OBJS)
	@rm -f $(C_OBJS)
	@rm -Rf $(TARGET_NAME)

finish:
	@echo "make done"
	@echo "<<<<========================================================"
	@echo

$(TARGET_NAME): $(CPP_OBJS) $(CPP_SRCS) $(C_OBJS) $(C_SRCS)
	@echo "generate $@"
	@$(CPP) -o $@ $(C_OBJS) $(CPP_OBJS) $(LIB_PATH) $(LIB_NAME) -ldl -lm -lpthread

%.c.o : %.c
	@echo "compile $@"
	@$(CC) $(C_FLAGS) $(INCLUDES) -c $< -o $@

%.cpp.o : %.cpp
	@echo "compile $@"
	@$(CPP) $(CPP_FLAGS) $(INCLUDES) -c $< -o $@

