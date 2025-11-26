.SUFFIXES:

# This is the name of the final executable
override OUTPUT := mangoOS

# User controllable toolchain and toolchain pref
TOOLCHAIN :=
TOOLCHAIN_PREFIX :=
ifneq ($(TOOLCHAIN),)
	ifeq ($(TOOLCHAIN_PREFIX),)
		TOOLCHAIN_PREFIX := $(TOOLCHAIN)-
	endif
endif

# User controllable C compiler command
ifneq ($(TOOLCHAIN_PREFIX),)
	CC := $(TOOLCHAIN_PREFIX)gcc
else
	CC := cc
endif

# User controllable Linker command
LD := $(TOOLCHAIN_PREFIX)ld

# Defaults overrides for variables if using "llvm" as toolchain
ifeq ($(TOOLCHAIN),llvm)
	CC := clang
	LD := ld.lld
endif

# Build variables
KERNEL_DIR := kernel
BOOTLOADER_DIR := bootloader/limine
ISO_DIR := iso_root
ISO := mangoOS-x86_64.iso
LINKER_SCRIPT := linker.lds

# User controllable C flags
CFLAGS := -g -O2 -pipe

# User controllable C preprocessor flags (none by default)
CPPFLAGS :=

# User controllable nasm flags
NASMFLAGS := -g

# User controllable Linker flags. We set none by default
LDFLAGS :=

# Check if CC is Clang
override CC_IS_CLANG := $(shell ! $(CC) --version 2>/dev/null | grep -q '^Target: '; echo $$?)

# If the C compiler is Clang, set the target as needed
ifeq ($(CC_IS_CLANG),1)
	override CC += \
		-target x86_64-unknown-none-elf
endif

# DO NOT CHANGE -------------------------------------------------
# Internal C flags
override CFLAGS += \
	-Wall \
	-Wextra \
	-std=gnu11 \
	-ffreestanding \
	-fno-stack-protector \
	-fno-stack-check \
	-fno-lto \
	-fno-PIC \
	-ffunction-sections \
	-fdata-sections \
	-m64 \
	-march=x86-64 \
	-mabi=sysv \
	-mno-80387 \
	-mno-mmx \
	-mno-sse \
	-mno-sse2 \
	-mno-red-zone \
	-mcmodel=kernel

# Internal C preproccessor flags
override CPPFLAGS := \
	-I kernel \
	-I bootloader/limine-protocol/include \
	$(CPPFLAGS) \
	-MMD \
	-MP

# Internal NASM flags
override NASMFLAGS := \
	-f elf64 \
	$(patsubst -g,-g -F dwarf,$(NASMFLAGS)) \
	-Wall

# Internal Linker flags
override LDFLAGS += \
	-m elf_x86_64 \
	-nostdlib \
	-static \
	-z max-page-size=0x1000 \
	--gc-sections \
	-T linker.lds

# DO NOT CHANGE ----------------------------------------------------------

# User "find" to glob all *c, *s, and *asm files in the tree and obtain the
# object and header dependency file names
override SRCFILES := $(shell find -L kernel -type f 2>/dev/null | LC_ALL=C sort)
override CFILES := $(filter %.c,$(SRCFILES))
override ASFILES := $(filter %.S,$(SRCFILES))
override NASMFILES := $(filter %.asm,$(SRCFILES))
override OBJ := $(addprefix obj/,$(CFILES:.c=.c.o) $(ASFILES:.S=.S.o) $(NASMFILES:.asm=.asm.o))
override HEADER_DEPS := $(addprefix obj/,$(CFILES:.c=.c.d) $(ASFILES:.S=.S.d))

# Default target. Must come before header dependencies
all: bin/$(OUTPUT)

# Header dependencies
-include $(HEADER_DEPS)

# Link rules for the final executable.
bin/$(OUTPUT): GNUmakefile linker.lds $(OBJ)
	mkdir -p "$(dir $@)"
	$(LD) $(LDFLAGS) $(OBJ) -o $@

# Compilation rules for *.c files.
obj/%.c.o: %.c GNUmakefile
	mkdir -p "$(dir $@)"
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

# Compilation rules for *.S files.
obj/%.S.o: %.S GNUmakefile
	mkdir -p "$(dir $@)"
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

# Compilation rules for *.asm (nasm) files.
obj/%.asm.o: %.asm GNUmakefile
	mkdir -p "$(dir $@)"
	nasm $(NASMFLAGS) $< -o $@
# -------------------------------------------------------------------------

# ISO generation
iso: bin/$(OUTPUT)
	mkdir -p $(ISO_DIR)/boot
	cp -v bin/$(OUTPUT) $(ISO_DIR)/boot/

	mkdir -p $(ISO_DIR)/boot/limine
	cp -v $(BOOTLOADER_DIR)/limine.conf \
	      $(BOOTLOADER_DIR)/limine-bios.sys \
	      $(BOOTLOADER_DIR)/limine-bios-cd.bin \
	      $(BOOTLOADER_DIR)/limine-uefi-cd.bin \
	      $(ISO_DIR)/boot/limine/

	mkdir -p $(ISO_DIR)/EFI/BOOT
	cp -v $(BOOTLOADER_DIR)/BOOTX64.EFI $(ISO_DIR)/EFI/BOOT/
	cp -v $(BOOTLOADER_DIR)/BOOTIA32.EFI $(ISO_DIR)/EFI/BOOT/

	xorriso -as mkisofs -R -r -J \
		-b boot/limine/limine-bios-cd.bin \
		-no-emul-boot -boot-load-size 4 -boot-info-table \
		--efi-boot boot/limine/limine-uefi-cd.bin \
		-efi-boot-part --efi-boot-image --protective-msdos-label \
		$(ISO_DIR) -o $(ISO)

	$(BOOTLOADER_DIR)/limine bios-install $(ISO)

# Run with QEMU
run: iso
	qemu-system-x86_64 -cdrom $(ISO)

clean:
	rm -rf obj bin $(ISO_DIR) $(ISO)

.PHONY: iso run clean all 