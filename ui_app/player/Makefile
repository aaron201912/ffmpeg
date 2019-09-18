CROSS_COMPILE ?=arm-linux-gnueabihf-
CC  = $(CROSS_COMPILE)gcc
CPP = $(CROSS_COMPILE)g++
AR  = $(CROSS_COMPILE)ar
STRIP  = $(CROSS_COMPILE)strip

COM_FLAGS = -Wall -O2 -fPIC -mcpu=cortex-a9 -mfpu=neon-fp16 -mfloat-abi=hard -mthumb-interwork -marm
#C_FLAGS	= $(COM_FLAGS) -std=c11
#CPP_FLAGS  = $(COM_FLAGS) -std=c++11
C_FLAGS	   = $(COM_FLAGS) -std=gnu99
CPP_FLAGS  = $(COM_FLAGS) -std=gnu++11

#user path
USR_PATH = ../../ffmpeg-4.1.3/host

INCLUDES += -I. -I../guilib/include/ -I../../sstar/include/ -I./mediastream/ -I$(USR_PATH)/include
TARGET_NAME  = playerWnd

CPP_SRCS  =

C_SRCS    =  $(wildcard *.c */*.c)

CPP_OBJS = $(patsubst %.cpp, %.cpp.o, $(CPP_SRCS))
C_OBJS = $(patsubst %.c, %.c.o, $(C_SRCS))

LIB_PATH = -L../guilib/dynamic -L../guilib/static/8.2.1 -L../../sstar/lib -L$(USR_PATH)/lib/static
LIB_NAME += -lz -lmgncs -lmgutils -lmgeff -lmgplus -lminigui_ths -lfreetype -ljpeg -lpng -lmgncs4touch -lchipmunk -lts -lmi_vdec -lmi_sys -lmi_divp -lmi_disp -lmi_panel -lmi_ao -lmi_gfx -lmi_hdmi -lmi_common -ldl -lavformat -lavcodec -lavutil -lswscale -lswresample

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
	@$(CPP) -o $@ $(C_OBJS) $(CPP_OBJS) $(LIB_PATH) $(LIB_NAME) -lm -lpthread
	@$(STRIP) --strip-unneeded $@

%.c.o : %.c
	@echo "compile $@"
	@$(CC) $(C_FLAGS) $(INCLUDES) $(DEFINES) -c $< -o $@

%.cpp.o : %.cpp
	@echo "compile $@"
	@$(CPP) $(CPP_FLAGS) $(INCLUDES) $(DEFINES) -c $< -o $@
