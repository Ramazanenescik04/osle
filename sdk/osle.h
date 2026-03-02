#ifndef OSLE_H
#define OSLE_H

// ==== INTERRUPTS ====
#define INT_RETURN 0x20
#define INT_FS_FIND 0x21
#define INT_FS_CREATE 0x22
#define INT_FS_WRITE 0x23

// ==== PROCESS MANAGEMENT ====
#define PM_ARGS 0xFFBF

// ==== FILE SYSTEM ====
#define FS_FILES 40
#define FS_PATH_OFFSET 0
#define FS_PATH_SIZE 21
#define FS_FLAGS_OFFSET FS_PATH_SIZE
#define FS_FLAGS_SIZE 1
#define FS_SIZE_OFFSET FS_PATH_SIZE + FS_PLAGS_SIZE
#define FS_SIZE_SIZE 2
#define FS_HEADER_SIZE FS_PATH_SIZE + FS_FLAGS_SIZE + FS_SIZE_SIZE
#define FS_DATA_OFFSET FS_HEADER_SIZE
#define FS_DATA_SIZE 9192
#define FS_BLOCK_SIZE FS_DATA_SIZE + FS_HEADER_SIZE

typedef unsigned char byte_t;
typedef unsigned short word_t;
// File handle, used for IO operations
typedef byte_t handle_t;

typedef struct {
  byte_t name[FS_PATH_SIZE];
  byte_t flags;
  word_t size;
  byte_t data[FS_DATA_SIZE];
} file_t;

// Returns control back to OSle. Use it to exit from a running program and give
// control back to the OS.
//
// Usage:
//
//   exit();
__attribute__((naked, noreturn)) static inline void exit(void) {
  __asm__ volatile("int %0" ::"N"(INT_RETURN));
}

// Prints a char on the screen at the current cursor position.
//
// Usage:
//
//   putc('A');
static inline void putc(char c) {
  __asm__ volatile("mov %0, %%al\n"
                   "mov $0x0E, %%ah\n"
                   "int $0x10\n"
                   :
                   : "r"(c)
                   : "ax");
}

// Prints a null-terminated string on the screen at the current cursor position.
//
// Usage:
//
//   puts("Hello, world!", 13);
static inline void puts(const char *str, unsigned maxlen) {
  __asm__ volatile("mov $0x0E, %%ah\n"
                   ".loop:\n"
                   "  lodsb\n"
                   "  test %%al, %%al\n"
                   "  je .done\n"
                   "  int $0x10\n"
                   "  loop .loop\n"
                   ".done:"
                   :
                   : "S"(str), "c"(maxlen)
                   : "ax");
}

// Like puts, but appends a new line at the end.
//
// Usage:
//
//   putln("Hello, world!", 13);
static inline void putln(const char* str, unsigned maxlen) {
  puts(str, maxlen);
  putc('\n');
  putc('\r');
}

// Tries to locate a file whose path is a null-terminated string in path.
//
// If the file is found, its content will be loaded in file.
// If the file cannot be found, a non-zero value is returned.
// The file handle needed for write operations is stored in handle.
//
// The file buffer will include the entir_ file, headers included. See FILE
// SYSTEM in osle.inc for details.
//
// Usage:
//
//   file_t file;
//   handle_t handle;
//   if (open("myfile", &handle, &file)) {
//     // handle error
//   }
static inline int open(const char *path, handle_t *handle, file_t *file) {
  byte_t error;
  word_t ax;
  __asm__ volatile("int %2\n"
                   "setc %0\n"
                   : "=q"(error), "=a"(ax)
                   : "N"(INT_FS_FIND), "b"(file), "D"(path)
                   :);
  (*handle) = (byte_t)ax;
  return error;
}

// Tries to create a file at path, a null-terminated string.
//
// If successful, the file will be created on the disk and file will point to
// the memory area associated with that file. To update the file, use write().
// If the file cannot be created, a non-zero value is returned.
// The file handle needed for write operations is stored in handle.
//
// Usage:
//
//   file_t file;
//   handle_t handle;
//   if (create("newfile", &handle, &file)) {
//     // handle error
//   }
static inline int create(const char *path, handle_t *handle, file_t *file) {
  byte_t error;
  word_t ax;
  __asm__ volatile("int %2\n"
                   "setc %0\n"
                   : "=q"(error), "=a"(ax)
                   : "N"(INT_FS_CREATE), "b"(file), "D"(path)
                   :);
  (*handle) = (byte_t)ax;
  return error;
}

// Writes the file identified by handle to disk, updating its data with the
// content in file.
//
// In case of failure, a non-zero value is returned.
//
// Usage:
//
//   file_t file;
//   handle_t handle;
//   // ... open or create the file ...
//   if (write(handle, &file)) {
//     // handle error
//   }
static inline int write(handle_t handle, file_t *file) {
  byte_t error;
  __asm__ volatile("int %1\n"
                   "setc %0\n"
                   : "=q"(error)
                   : "N"(INT_FS_WRITE), "b"(file), "d"(handle)
                   :);
  return error;
}

// Reads a character from keyboard input.
//
// Usage:
//
//   char c = getc();
//   // c is the ASCII char inputed from keyboard
static inline char getc(void) {
  word_t ax;
  __asm__ volatile("mov $0x00, %%ah\n"
                   "int $0x16"
                   : "=a"(ax)::);
  return (char)(ax & 0xFF);
}

#endif
