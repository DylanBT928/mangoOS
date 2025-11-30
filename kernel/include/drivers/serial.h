#ifndef SERIAL_H
#define SERIAL_H

#include <stdint.h>
#include <stdbool.h>

extern bool debug_on;

// Serial log functions
char* serial_get_log();

// Core serial functions
void serial_init();
void serial_write_char(char c);
void serial_print(const char* str);

// Extended output functions
void serial_print_hex(uint64_t num);
void serial_print_dec(int64_t num);
void serial_print_bin(uint64_t num);
void serial_printf(const char* fmt, ...);

// Terminal control
void serial_clear_char();

#endif
