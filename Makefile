BINUTILS_VERSION := 2.38
TOOL_DIR=$(CURDIR)/tools
CLEANFILES :=
TARGET:=arm-elf-eabi

all: example.gba

binutils-$(BINUTILS_VERSION)-build/Makefile: binutils-$(BINUTILS_VERSION)/configure
	@mkdir $(@D)
	cd $(@D) && $(abspath $<) --prefix=$(TOOL_DIR) --target=$(TARGET) --disable-nls --disable-gdb

AS := $(TOOL_DIR)/bin/$(TARGET)-as
LD := $(TOOL_DIR)/bin/$(TARGET)-ld
OBJCOPY := $(TOOL_DIR)/bin/$(TARGET)-objcopy
CC := clang -target arm -mcpu=arm7tdmi -mthumb -Oz
RM := rm

$(AS) $(LD) $(OBJCOPY): binutils-$(BINUTILS_VERSION)-build/Makefile
	$(MAKE) -C binutils-$(BINUTILS_VERSION)-build all
	$(MAKE) -C binutils-$(BINUTILS_VERSION)-build install

OBJS:=

ASM_SOURCES := crt0.s
ASM_OBJS := $(ASM_SOURCES:%.s=objs/%.o)
OBJS += $(ASM_OBJS)
C_SRCS := main.c
C_OBJS := $(C_SRCS:%.c=objs/%.o)
OBJS += $(C_OBJS)
CLEANFILES += $(OBJS)

$(ASM_OBJS): objs/%.o: %.s $(AS)
	$(AS) "$<" -o "$@"
$(C_OBJS): objs/%.o: %.c
	$(CC) $< -c -o $@

objs/example.elf: $(OBJS) $(LD)
	$(LD) -T gba_cart.ld $(OBJS) --gc-sections -o $@
CLEANFILES += objs/example.elf

example.gba: objs/example.elf $(OBJCOPY)
	$(OBJCOPY) -O binary objs/example.elf $@
CLEANFILES += example.gba

clean:
	$(RM) -f $(CLEANFILES)
.PHONY: clean
