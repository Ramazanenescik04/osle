#include "../sdk/osle.h"

static file_t file;
static handle_t handle;

static void done(void) {
  putln("Press any key to continue...", 29);
  (void)getc();
  exit();
}

static void usage(void) {
  putln("Usage: touch [FILE]", 20);
  done();
}

int main(int argc, char** argv) {
  if (argc != 1) {
    putln("Missing file name.", 19);
    usage();
  }

  char* filename = argv[0];

  if(!create(filename, &handle, &file)) {
    puts("File ", 5);
    puts(filename, 0xFF);
    putln(" created successfully.", 23);
  } else {
    putln("Unable to create file.", 23);
  }

  done();
}
