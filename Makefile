PREFIX = aarch64-none-elf
LD = $(PREFIX)-ld
AS = $(PREFIX)-gcc
GCC = $(PREFIX)-gcc
GPP = $(PREFIX)-g++
OBJCOPY = $(PREFIX)-objcopy

OUTPUT = kernel8
SRC = src
INC = include
BUILD = build
TOOLCHAIN = /storage/gcc-arm-9.2-2019.12-x86_64-aarch64-none-elf/lib/gcc/aarch64-none-elf/9.2.1

LINKER = linker.ld

CXX_FLAGS = -mcpu=cortex-a53
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

AS_FLAGS = -mcpu=cortex-a53 -I $(INC)

all: clean $(BUILD) $(OUTPUT).img

clean:
	rm $(OUTPUT).elf $(OUTPUT).img $(BUILD)/*.o >/dev/null 2>/dev/null || true
	rm -rf $(BUILD)

$(BUILD):
	mkdir $(BUILD)

$(BUILD)/%_c.o: $(SRC)/%.c $(BUILD)
	$(GPP) $(CXX_FLAGS) -c $< -o $@

$(BUILD)/%_cpp.o: $(SRC)/%.cpp $(BUILD)
	$(GPP) $(CXX_FLAGS) -c $< -o $@

$(BUILD)/%_s.o: $(SRC)/%.S $(BUILD)
	$(AS) $(AS_FLAGS) -c $< -o $@

$(BUILD)/font_psf.o: font.psf
	$(LD) -r -b binary -o $(BUILD)/font_psf.o font.psf

C_FILES = $(wildcard $(SRC)/*.c)
CPP_FILES = $(wildcard $(SRC)/*.cpp)
ASM_FILES = $(wildcard $(SRC)/*.S)
OBJ_FILES = $(C_FILES:$(SRC)/%.c=$(BUILD)/%_c.o)
OBJ_FILES = $(CPP_FILES:$(SRC)/%.cpp=$(BUILD)/%_cpp.o)
OBJ_FILES += $(ASM_FILES:$(SRC)/%.S=$(BUILD)/%_s.o)
OBJ_FILES += $(BUILD)/font_psf.o

$(OUTPUT).img: $(LINKER) $(OBJ_FILES) $(BUILD)
	$(LD) -nostdlib -nostartfiles -T $(LINKER) -o $(OUTPUT).elf $(OBJ_FILES)
	$(OBJCOPY) $(OUTPUT).elf -O binary $(OUTPUT).img

QEMU = qemu-system-aarch64 -smp 4 -M raspi3 -m 1024M -kernel $(OUTPUT).elf -serial null -serial stdio

debug:
	$(QEMU) -s -d in_asm
	
run: 
	$(QEMU)

copy:
	cp $(OUTPUT).img /media/david/boot/kernel8.img