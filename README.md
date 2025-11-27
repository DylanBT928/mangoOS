<div align="center">
  
# mangoOS 🥭

**Custom 64-bit operating system built with C and Limine**

<!-- ![GitHub Release](https://img.shields.io/github/v/release/DylanBT928/mangoOS) -->
<!-- ![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/DylanBT928/mangoOS) -->
<!-- ![GitHub License](https://img.shields.io/github/license/DylanBT928/mangoOS) -->

</div>

<p>mangoOS is a 64-bit x86-64 operating system built using C. It is powered by the Limine bootloader and utilizes the GNU Make build system. Its long-term direction is unknown for now, and the project may continue to grow or conclude as our goals evolve. Nonetheless, we are thrilled to learn about opearting systems and see where it leads us.</p>

## Prerequisites

- x86_64-elf Cross-Compiler
- GNU Make
- QEMU
- xorriso

> **Note:** You can build the cross-compiler by following the [OSDev GCC Cross-Compiler Guide](https://wiki.osdev.org/GCC_Cross-Compiler).

## Building
The build process is automated by the GNUmakefile. 
#### Build and Run
```bash
make run      # Generate the ISO and launch it in QEMU
```

#### Other Commands
```bash
make all      # Compile and link all source files
make iso      # Create a bootable ISO
make clean    # Remove all build artifacts
```

#### macOS
```bash
make all TOOLCHAIN=x86_64-elf
make iso TOOLCHAIN=x86_64-elf
make run TOOLCHAIN=x86_64-elf
```

## References

- [Operating Systems: Three Easy Pieces](https://pages.cs.wisc.edu/~remzi/OSTEP/)
- [OSDev Wiki](https://wiki.osdev.org/)

## License

This project is licensed under the Apache License 2.0. See the [LICENSE](./LICENSE) file for details.
