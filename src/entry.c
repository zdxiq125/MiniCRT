#include "minicrt.h"
#ifdef WIN32
#include <Windows.h>
#endif

extern int main(int argc, char* argv[]);
void exit(int);

static void crt_fatal_error(const char* msg) {

}

void mini_crt_entry(void) {

}

void exit(int exitCode) { }
