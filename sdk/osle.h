#ifndef OSLE_H
#define OSLE_H

// ==== INTERRUPTS ====
#define INT_RETURN    0x20
#define INT_FS_FIND   0x21
#define INT_FS_CREATE 0x22
#define INT_FS_WRITE  0x23

// ==== PROCESS MANAGEMENT ====
#define PM_ARGS 0xFFBF

// ==== FILE SYSTEM ====
#define FS_FILES        40
#define FS_PATH_OFFSET  0
#define FS_PATH_SIZE    21
#define FS_FLAGS_OFFSET FS_PATH_SIZE
#define FS_FLAGS_SIZE   1
#define FS_SIZE_OFFSET  FS_PATH_SIZE + FS_PLAGS_SIZE
#define FS_SIZE_SIZE    2
#define FS_HEADER_SIZE  FS_PATH_SIZE + FS_FLAGS_SIZE + FS_SIZE_SIZE
#define FS_DATA_OFFSET  FS_HEADER_SIZE
#define FS_BLOCK_SIZE   9216

// File handle, used for IO operations
typedef unsigned file_handle_t;

// Return control to OSle
__attribute__((naked, noreturn)) 
void exit(void) {
  __asm__ volatile("int %0" :: "N"(INT_RETURN));
}

// Prints a null-terminated string on the screen at the current cursor position
static inline void puts(char *str, unsigned maxlen) { 
  __asm__ volatile(
    "mov $0x0E, %%ah\n"
    ".loop:\n"
    "  lodsb\n"
    "  test %%al, %%al\n"
    "  je .done\n"
    "  int $0x10\n"
    "  loop .loop\n"
    ".done:"
    :
    : "S"(str), "c"(maxlen)
    : "ax"
  );
}

// Prints a char on the screen at the current cursor position
static inline void putc(char c) {
  __asm__ volatile(
    "mov %0, %%al\n"
    "mov $0x0E, %%ah\n"
    "int $0x10\n"
    :
    : "r"(c)
    : "ax"
  );
}

static inline int ffind(const char* name, file_handle_t* file, void* filebuf) {
  unsigned char error;
  __asm__ volatile(
    "int %1"
    "setc %0"
    : "=q"(error)
    : "N"(INT_FS_FIND), "b"(filebuf), "D"(name)
    : "ax"
  );
  return error ? -1 : 0; 
} 

#endif
