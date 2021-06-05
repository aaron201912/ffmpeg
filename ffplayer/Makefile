CURRENT_PATH = $(shell pwd)
CROSS_COMPILE ?=arm-linux-gnueabihf-
CC  = $(CROSS_COMPILE)gcc
CPP = $(CROSS_COMPILE)g++
AR  = $(CROSS_COMPILE)ar

DISPLAY ?= panel
LIBRARY ?= dynamic
CHIP ?= ssd20x

COM_FLAGS = -Wall -O2 -fPIC -mcpu=cortex-a9 -mfpu=neon-fp16 -mfloat-abi=hard -mthumb-interwork -marm -g -funwind-tables -ffunction-sections -rdynamic
C_FLAGS   = $(COM_FLAGS) -std=c11
CPP_FLAGS = $(COM_FLAGS) -std=c++11

INC  = -I$(CURRENT_PATH)/include -I$(CURRENT_PATH)/../ffmpeg-4.1.3/host/include
LIB  = -L$(CURRENT_PATH)/../ffmpeg-4.1.3/host/dynamic

ifeq ($(CHIP),ss268)
C_FLAGS   += -DCHIP_IS_SS268
CPP_FLAGS += -DCHIP_IS_SS268
INC += -I$(CURRENT_PATH)/../sstar_ss268/include
LIB += -L$(CURRENT_PATH)/../sstar_ss268/lib
else ifeq ($(CHIP),ssd20x)
C_FLAGS   += -DCHIP_IS_SSD20X
CPP_FLAGS += -DCHIP_IS_SSD20X
INC += -I$(CURRENT_PATH)/../sstar/include
LIB += -L$(CURRENT_PATH)/../sstar/lib
endif

LIB += -lavformat -lavcodec -lavutil -lswscale -lswresample

ifeq ($(CHIP),ss268)
LIB += -lmi_vdec -lmi_disp -lmi_ao -lmi_gfx -lmi_sys -lmi_common -lcam_os_wrapper
else ifeq ($(CHIP),ssd20x)
LIB += -lmi_vdec -lmi_divp -lmi_disp -lmi_ao -lmi_gfx -lmi_sys -lmi_common
endif

LIB_SRCS = $(wildcard $(CURRENT_PATH)/src/*.c)
LIB_OBJS = $(patsubst %.c, %.o, $(LIB_SRCS))
LIB_NAME = ssplayer
LIB_PATH = libs

all: clean static_lib shared_lib install
	make -C $(CURRENT_PATH)/app DISPlAY=$(DISPlAY) LIBRARY=$(LIBRARY) CHIP=$(CHIP)

static_lib: $(LIB_OBJS)
	@echo "generate lib$(LIB_NAME).a"
	@$(AR) -rcs lib${LIB_NAME}.a $(LIB_OBJS)

shared_lib: $(LIB_OBJS)
	@echo "generate lib$(LIB_NAME).so"
	@$(CC) -fPIC -shared -o lib$(LIB_NAME).so $(LIB_OBJS) $(INC) $(LIB)

.PRONY: clean
clean: distclean
	@echo "clean $(LIB_NAME) object"
	@rm -rf $(LIB_OBJS)
	make -C $(CURRENT_PATH)/app clean

distclean:
	@echo "clean $(LIB_NAME) libs"
	@rm -rf $(LIB_PATH)
	@rm -rf lib$(LIB_NAME).*

install:
	@echo "install $(LIB_NAME) object"
	mkdir -p ${LIB_PATH}/include
	mkdir -p ${LIB_PATH}/dynamic
	mkdir -p ${LIB_PATH}/static
	cp -rf $(CURRENT_PATH)/include/interface.h $(LIB_PATH)/include
	cp -rf $(CURRENT_PATH)/include/player.h $(LIB_PATH)/include
	cp -rf lib$(LIB_NAME).so $(LIB_PATH)/dynamic
	cp -rf lib$(LIB_NAME).a $(LIB_PATH)/static

.SUFFIXES: .c .o
%.o : %.c
	@echo "compile $@"
	@$(CC) $(C_FLAGS) $(INC) $(LIB) -lm -ldl -lpthread -c $< -o $@
	