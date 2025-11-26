CC := x86_64-elf-gcc
LD := x86_64-elf-ld
BUILD_DIR := bin
BOOTLOADER_DIR := bootloader/limine
KERNEL_DIR := kernel 
KERNEL := $(BUILD_DIR)/mangoOS
ISO_DIR := iso_root
ISO := mangoOS-x86_64.iso
LINKER_SCRIPT := linker.lds

# Collect all C files and convert to object files in BUILD_DIR
KERNEL_C_SRC_FILES := $(shell find $(KERNEL_DIR) -name '*.c')
KERNEL_C_OBJ_FILES := $(patsubst $(KERNEL_DIR)/%,$(BUILD_DIR)/%,$(KERNEL_C_SRC_FILES:.c=.o))
KERNEL_OBJ_FILES := $(KERNEL_C_OBJ_FILES)

$(BUILD_DIR)/libkernel.a: $(KERNEL_OBJ_FILES)
	ar rcs $@ $^

# Default target (build ISO)
all: iso

# Build and run the kernel in QEMU
run: iso
	qemu-system-x86_64 -cdrom $(ISO)

# Package ISO with Limine
iso: kernel
	mkdir -p $(ISO_DIR)/boot
	cp -v $(KERNEL) $(ISO_DIR)/boot/
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
	./$(BOOTLOADER_DIR)/limine bios-install $(ISO)

# Build only the kernel
kernel: $(KERNEL)

$(KERNEL): $(BUILD_DIR)/libkernel.a
	$(LD) -T $(LINKER_SCRIPT) -o $@ $^

$(BUILD_DIR)/%.o: $(KERNEL_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) -c $< -o $@

# Remove build artifacts
clean:
	rm -rf $(BUILD_DIR) $(ISO_DIR) $(ISO)

.PHONY: clean all kernel iso run