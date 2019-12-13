/* fileaux.cpp */
/* Author: Pekka Astola */
/* <pekka.astola@tuni.fi>*/

#include "fileaux.hh"
#include <sys/stat.h>

#include <string>

using namespace std;

#define SYSTEM_VERBOSE_QUIET true

int system_1(char *str) {
  string sys_call_str(str);
if (!SYSTEM_VERBOSE_QUIET)
  printf("System command: %s\n", str);
#ifdef _WIN32
  sys_call_str.append(" > nul");
#endif
#ifdef __unix__
  sys_call_str.append(" > /dev/null");
#endif
  return system(sys_call_str.c_str());
}


long aux_GetFileSize(string filename) {
  struct stat stat_buf;
  int rc = stat(filename.c_str(), &stat_buf);
  return rc == 0 ? stat_buf.st_size : -1;
}


long aux_GetFileSize(const char* filename) {
  string sbuffer(filename);
  return aux_GetFileSize(sbuffer);
}


long aux_GetFileSize(char* filename) {
  string sbuffer(filename);
  return aux_GetFileSize(sbuffer);
}

