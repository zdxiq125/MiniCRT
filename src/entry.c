#include "minicrt.h"
#ifdef WIN32
#include <Windows.h>
#endif

extern int main(int argc, char* argv[]);
void exit(int);

static void crt_fatal_error(const char* msg) {
  exit(1);
}

void mini_crt_entry(void) {
  int ret;
#ifdef WIN32
  int flag = 0;
  int argc = 0;
  char* argv[16];
  char* cl = GetCommandLineA();

  argv[0] = cl;
  argc++;
  while(*cl) {
    if(*cl == '\"') {
      flag = flag == 0 ? 1 : 0;
    } else if(*cl == ' ' && flag == 0) {
      if(*(cl+1)) {
        argv[argc] = cl + 1;
        argc++;
      }
      *cl = '\0';
    }
    cl++;
  }
#else
  int argc;
  char** argv;
  char* ebp_reg = 0;
  asm("movl %%ebp,%0 \n":"=r"(ebp_reg));
  argc = *(int*)(ebp_reg + 4);
  argv = (char**)(ebp_reg + 8);
#endif
  if(!mini_crt_heap_init())
    crt_fatal_error("heap initialize failed.");
  if(!min_crt_io_init())
    crt_fatal_error("io initialize failed.");
  ret = main(argc, argv);
  exit(ret);
}

void exit(int exitCode) { }
