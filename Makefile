ARCH = armv7-a
MCPU = cortex-a8

CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
LD = arm-none-eabi-ld
OC = arm-none-eabi-objcopy

LINKER_SCRIPT = ./homevilos.ld

ASM_SRCS = $(wildcard boot/*.S)
ASM_OBJS = $(patsubst boot/%.S, build/%.o, $(ASM_SRCS))

homevilos = build/homevilos.axf
homevilos_bin = build/homevilos.bin

.PHONY: all clean run debug gdb

all: $(homevilos)

clean:
	@rm -rf build

run: $(homevilos)
	qemu-system-arm -M realview-pb-a8 -kernel $(homevilos)

debug: $(homevilos)
	qemu-system-arm -M realview-pb-a8 -kernel $(homevilos) -S -gdb tcp::1234,ipv4 -nographic

gdb:
	arm-none-eabi-gdb

$(homevilos): $(ASM_OBJS) $(LINKER_SCRIPT)
	$(LD) -n -T $(LINKER_SCRIPT) -o $(homevilos) $(ASM_OBJS)
	$(OC) -O binary $(homevilos) $(homevilos_bin)

build/%.o: boot/%.S
	mkdir -p $(shell dirname $@)
	$(AS) -march=$(ARCH) -mcpu=$(MCPU) -g -o $@ $<
