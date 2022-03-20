BINUTILS_VERSION := 2.38
TOOL_DIR=$(CURDIR)/tools
CLEANFILES :=

all: example.gba

binutils-$(BINUTILS_VERSION)-build/Makefile: binutils-$(BINUTILS_VERSION)/configure
	cd $@ && $< --prefix=$(TOOL_DIR) --target=arm-elf --disable-nls --disable-gdb

AS := $(TOOL_DIR)/bin/arm-elf-as
LD := $(TOOL_DIR)/bin/arm-elf-ld
OBJCOPY := $(TOOL_DIR)/bin/arm-elf-objcopy
RM := rm

$(AS) $(LD) $(OBJCOPY): binutils-$(BINUTILS_VERSION)-build/Makefile
	$(MAKE) -C binutils-$(BINUTILS_VERSION)-build all
	$(MAKE) -C binutils-$(BINUTILS_VERSION)-build install

ASM_SOURCES := main.s crt0.s
OBJS := $(ASM_SOURCES:%.s=objs/%.o)
CLEANFILES += $(OBJS)

$(OBJS): objs/%.o: %.s $(AS)
	$(AS) "$<" -o "$@"

objs/example.elf: $(OBJS) $(LD)
	$(LD) -T gba_cart.ld $(OBJS) --gc-sections -o $@
CLEANFILES += objs/example.elf

example.gba: objs/example.elf $(OBJCOPY)
	$(OBJCOPY) -O binary objs/example.elf $@
CLEANFILES += example.gba

clean:
	$(RM) -f $(CLEANFILES)
.PHONY: clean
