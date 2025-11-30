#include "drivers/serial.h"
#include "stdio.h"

bool debug_on = true;

#define SERIAL_LOG_SIZE 8192 
#define COM1 0x3f8

static char serial_log[SERIAL_LOG_SIZE];
static size_t serial_log_index = 0;

static inline bool is_transmit_empty() {
    return (inb(COM1 + 5) & 0x20) != 0; 
} 

char *serial_get_log() {
    if (serial_log_index >= SERIAL_LOG_SIZE)
        serial_log_index = SERIAL_LOG_SIZE - 1;
    serial_log[serial_log_index] = '\0'; 
    return serial_log; 
}

void init_serial() {
    outb(COM1 + 1, 0x00); 
    outb(COM1 + 3, 0x80); 
    outb(COM1 + 0, 0x03); 
    outb(COM1 + 1, 0x00); 
    outb(COM1 + 3, 0x03); 
    outb(COM1 + 2, 0xC7); 
    outb(COM1 + 4, 0x0B); 
    outb(COM1 + 4, 0x1E); 
    outb(COM1 + 0, 0xAE); 

    if (inb(COM1 + 0) != 0xAE) {
        return 1;
    }
    outb(COM1 + 4, 0x0F);
    if (debug_on) {
        printf("Serial output is successfully enabled.");
    }
}

void serial_write_char(char c) {
    while (!is_transmit_empty());
    outb(COM1, c);

    if (serial_log_index < SERIAL_LOG_SIZE - 1) {
        serial_log[serial_log_index++] = c;
        serial_log[serial_log_index] = '\0';
    }
}

void serial_print(const char* str) {
    while (*str) serial_write_char(*str++);
}

void serial_clear_char() {
    serial_write_char('\b'); 
    serial_write_char(' ');  
    serial_write_char('\b'); 
}