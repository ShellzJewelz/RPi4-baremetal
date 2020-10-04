PREFIX = aarch64-none-elf
LD = $(PREFIX)-ld
AS = $(PREFIX)-as
GCC = $(PREFIX)-gcc
GPP = $(PREFIX)-g++
OBJCOPY = $(PREFIX)-objcopy

OUTPUT = kernel8
SRC = src
INC = includes
BUILD = build

LINKER = linker.ld

CXX_FLAGS = -mcpu=cortex-a53 -Wall -O2 -ffreestanding -nostdinc -nostdlib -nostartfiles -fpermissive -fno-exceptions -I $(INC)
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

C_FILES = $(wildcard $(SRC)/*.c)
CPP_FILES = $(wildcard $(SRC)/*.cpp)
ASM_FILES = $(wildcard $(SRC)/*.S)
OBJ_FILES = $(C_FILES:$(SRC)/%.c=$(BUILD)/%_c.o)
OBJ_FILES = $(CPP_FILES:$(SRC)/%.cpp=$(BUILD)/%_cpp.o)
OBJ_FILES += $(ASM_FILES:$(SRC)/%.S=$(BUILD)/%_s.o)

$(OUTPUT).img: $(LINKER) $(OBJ_FILES) $(BUILD)
	$(LD) -nostdlib -nostartfiles -T $(LINKER) -o $(OUTPUT).elf $(OBJ_FILES)
	$(OBJCOPY) $(OUTPUT).elf -O binary $(OUTPUT).img

QEMU = qemu-system-aarch64 -M raspi3 -m 1024M -kernel $(OUTPUT).img -serial null -serial stdio

debug:
	$(QEMU) -s -d in_asm
	
run: 
	$(QEMU)

copy:
	cp $(OUTPUT).img /media/david/boot/kernel8.img