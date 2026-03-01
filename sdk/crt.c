extern int main(void);

__attribute__((naked, section(".text.epilogue"), noreturn)) 
void _exit(void) {
  __asm__ volatile("int $0x20" :::);
}

__attribute__((naked, section(".text.prologue"), noreturn)) 
void _start(void) {
  __asm__ volatile(
    // Set video mode
    "mov $0x00, %%ah\n"
    "mov $0x03, %%al\n"
    "int $0x10"
    ::: "ax"
  );

  main();

  _exit();
}

