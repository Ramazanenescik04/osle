#include "../sdk/osle.h"

static file_t file;
static handle_t handle;

static void done(void) {
  putln("Press any key to continue...", 29);
  (void)getc();
  exit();
}

static void error(const char *msg, int msglen) {
  putln(msg, msglen);
  done();
}

static void usage(void) {
  putln("Usage: touch [FILE]", 20);
  done();
}

int main(int argc, char **argv) {
  if (argc != 1) {
    error("Missing file name.", 19);
  }

  char *filename = argv[0];

  if (0 != create(filename, &handle, &file)) {
    error("Unable to create file.", 23);
  }
  
  puts("File ", 5);
  puts(filename, 0xFF);
  putln(" created successfully.", 23);

  done();
}
