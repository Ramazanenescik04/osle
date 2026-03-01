extern int main(void);

__attribute__((naked, section(".text.epilogue"), noreturn)) 
void __epilogue(void) {
  __asm__ volatile("int $0x20" :::);
}

__attribute__((section(".text.prologue"), noreturn)) 
void __prologue(void) {
  __asm__ volatile(
    "mov $0x0003, %%ax\n"
    "int $0x10"
    ::: "ax"
  );

  main();

  __epilogue();
}

