WORKSPACE_HOME = 0
WORKSPACE_LAPTOP = 1
WORKSPACE = $(WORKSPACE_HOME)

ifeq ($(WORKSPACE), $(WORKSPACE_HOME))
PREFIX = aarch64-none-elf
TOOLCHAIN = /storage/gcc-arm-9.2-2019.12-x86_64-aarch64-none-elf/lib/gcc/aarch64-none-elf/9.2.1
else
PREFIX = /home/david/gcc-arm-9.2-2019.12-x86_64-aarch64-none-elf/bin/aarch64-none-elf
TOOLCHAIN = /home/david/gcc-arm-9.2-2019.12-x86_64-aarch64-none-elf/lib/gcc/aarch64-none-elf/9.2.1
endif

LD = $(PREFIX)-ld
AS = $(PREFIX)-gcc
GCC = $(PREFIX)-gcc
GPP = $(PREFIX)-g++
OBJCOPY = $(PREFIX)-objcopy

OUTPUT = kernel8
SRC = src
INC = include
BUILD = build

LINKER = linker.ld

RPI_VERSION_3 = 3
RPI_VERSION_4 = 4
RPI_VERSION ?= $(RPI_VERSION_4)

ifeq ($(RASPI_VERSION), $(RPI_VERSION_3))
CXX_FLAGS = -mcpu=cortex-a53
else
CXX_FLAGS = -mcpu=cortex-a72
endif
CXX_FLAGS += -Wall
CXX_FLAGS += -O2
CXX_FLAGS += -ffreestanding
CXX_FLAGS += -nostdinc
CXX_FLAGS += -nostdlib
CXX_FLAGS += -nostartfiles
CXX_FLAGS += -fpermissive
# Disable trying to use exceotion table
# and disalbe g++ inserting guards for statics
# and disable automatic object destruction at exit
# as we don't have standard libs implementing it (libc)
CXX_FLAGS += -fno-exceptions
CXX_FLAGS += -fno-rtti
CXX_FLAGS += -fno-threadsafe-statics
# Includes
CXX_FLAGS += -I $(INC)
CXX_FLAGS += -I $(TOOLCHAIN)/include

ifeq ($(RPI_VERSION), $(RPI_VERSION_3))
AS_FLAGS = -mcpu=cortex-a53
else
AS_FLAGS = -mcpu=cortex-a72
endif
AS_FLAGS += -I $(INC)
AS_FLAGS += -I $(TOOLCHAIN)/include

ifeq ($(RPI_VERSION), $(RPI_VERSION_3))
DEFINES = -DRPI_VERSION=3
else
DEFINES = -DRPI_VERSION=4
endif

HIGH_PERI_DISABLED = 0
HIGH_PERI_ENABLED = 1

ifeq ($(HIGH_PERI), $(HIGH_PERI_ENABLED))
STUB_DEFINES = -DHIGH_PERI
DEFINES += -DHIGH_PERI
endif 

ifeq ($(RPI_VERSION), $(RPI_VERSION_4_VIRT))
DEFINES += -DGIC_BASE=0x08000000
endif

all: clean $(BUILD) $(OUTPUT).img

clean:
	rm $(OUTPUT).elf $(OUTPUT).img $(BUILD)/*.o >/dev/null 2>/dev/null || true
	rm -rf $(BUILD)

armstub8: armstub8-custom.S
	rm $(BUILD)/armstub8-custom_S.o $(BUILD)/armstub8-custom.out armstub8-custom.bin >/dev/null 2>/dev/null || true
	$(AS) $(AS_FLAGS) -DGIC -DBCM2711=1 $(STUB_DEFINES) -c $< -o $(BUILD)/armstub8-custom_S.o
	$(LD) -Ttext 0x0 $(BUILD)/armstub8-custom_S.o -o $(BUILD)/armstub8-custom.out
	$(OBJCOPY) -S -O binary $(BUILD)/armstub8-custom.out armstub8-custom.bin

$(BUILD):
	mkdir $(BUILD)

$(BUILD)/%_c.o: $(SRC)/%.c $(BUILD)
	$(GPP) $(CXX_FLAGS) $(DEFINES) -c $< -o $@

$(BUILD)/%_cpp.o: $(SRC)/%.cpp $(BUILD)
	$(GPP) $(CXX_FLAGS) $(DEFINES) -c $< -o $@

$(BUILD)/%_S.o: $(SRC)/%.S $(BUILD)
	$(AS) $(AS_FLAGS) -c $< -o $@

$(BUILD)/font_psf.o: font.psf
	$(LD) -r -b binary -o $(BUILD)/font_psf.o font.psf

C_FILES = $(wildcard $(SRC)/*.c)
CPP_FILES = $(wildcard $(SRC)/*.cpp)
ASM_FILES = $(wildcard $(SRC)/*.S)
#FONT_FILES = $(wildcard $(SRC)/*.psf)
OBJ_FILES = $(C_FILES:$(SRC)/%.c=$(BUILD)/%_c.o)
OBJ_FILES = $(CPP_FILES:$(SRC)/%.cpp=$(BUILD)/%_cpp.o)
OBJ_FILES += $(ASM_FILES:$(SRC)/%.S=$(BUILD)/%_S.o)
#OBJ_FILES += $(FONT_FILES:$(SRC)/%.psf=$(BUILD)/%_psf.o)
OBJ_FILES += $(BUILD)/font_psf.o

$(OUTPUT).img: $(LINKER) $(OBJ_FILES) $(BUILD)
	$(LD) -nostdlib -nostartfiles -T $(LINKER) -o $(OUTPUT).elf $(OBJ_FILES)
	$(OBJCOPY) $(OUTPUT).elf -O binary $(OUTPUT).img

QEMU = qemu-system-aarch64

ifeq ($(RPI_VERSION), $(RPI_VERSION_3))
QEMU_FLAGS = -machine raspi3
QEMU_FLAGS += -cpu cortex-a53
else
QEMU_FLAGS = -machine virt,gic-version=2
QEMU_FLAGS += -cpu cortex-a72
endif
QEMU_FLAGS += -m 1024M
QEMU_FLAGS += -kernel $(OUTPUT).elf

#QEMU = qemu-system-aarch64 -M raspi3 -m 128M -kernel $(OUTPUT).elf 
#VIRT_QEMU = qemu-system-aarch64 -M virt,gic-version=2, -cpu cortex-a53 -m 1024M -kernel $(OUTPUT).elf 
debug:
	$(QEMU) $(QEMU_FLAGS) -d in_asm -serial null -serial stdio
	
#-smp 4 -nographic -S -s 

run: 
	$(QEMU) $(QEMU_FLAGS) -serial null -serial stdio

copy:
	cp $(OUTPUT).img /media/david/boot/kernel8.img

stub-copy:
	cp armstub8-custom.bin /media/david/boot/armstub8-custom.bin
