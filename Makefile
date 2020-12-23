ARCH = armv7-a
MCPU = cortex-a8

TARGET = rvpb

CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
LD = arm-none-eabi-ld
OC = arm-none-eabi-objcopy

LINKER_SCRIPT = ./homevilos.ld
MAP_FILE = build/homevilos.map

ASM_SRCS = $(wildcard boot/*.S)
ASM_OBJS = $(patsubst boot/%.S, build/%.os, $(ASM_SRCS))

VPATH = boot \
				hal/$(TARGET)

C_SRCS 	= $(notdir $(wildcard boot/*.c))
C_SRCS += $(notdir $(wildcard hal/$(TARGET)/*.c))
C_OBJS 	= $(patsubst %.c, build/%.o, $(C_SRCS))

INC_DIRS 	= -I include				\
						-I hal						\
						-I hal/$(TARGET)

CFLAGS = -c -g -std=c11

homevilos = build/homevilos.axf
homevilos_bin = build/homevilos.bin

.PHONY: all clean run debug gdb

all: $(homevilos)

clean:
	@rm -rf build

run: $(homevilos)
	qemu-system-arm -M realview-pb-a8 -kernel $(homevilos) -nographic

debug: $(homevilos)
	qemu-system-arm -M realview-pb-a8 -kernel $(homevilos) -S -gdb tcp::1234,ipv4 -nographic

gdb:
	arm-none-eabi-gdb

$(homevilos): $(ASM_OBJS) $(C_OBJS) $(LINKER_SCRIPT)
	$(LD) -n -T $(LINKER_SCRIPT) -o $(homevilos) $(ASM_OBJS) $(C_OBJS) -Map=$(MAP_FILE)
	$(OC) -O binary $(homevilos) $(homevilos_bin)

build/%.os: %.S
	mkdir -p $(shell dirname $@)
	$(CC) -march=$(ARCH) -mcpu=$(MCPU) $(INC_DIRS) $(CFLAGS) -o $@ $<

build/%.o: %.c
	mkdir -p $(shell dirname $@)
	$(CC) -march=$(ARCH) -mcpu=$(MCPU) $(INC_DIRS) $(CFLAGS) -o $@ $<
