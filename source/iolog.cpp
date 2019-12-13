#include "iolog.hh"

long io_operations = 0;
long codec_operations = 0;

void print_io_log(const char* type, const char* src, const char* function,
                  int line, const char* dst) {
  char fmt[] =
      "{io}[\x1B[31m%d\x1B[0m] (\x1B[32m%s\x1B[0m) %s -> %s -> %d => \x1B[33m%s\x1B[0m\n";
  printf(fmt, io_operations, type, src, function, line, dst);
  io_operations++;
}

void print_codec_JP2_call(const char* type, const char* input,
                          const char* output, const char* src,
                          const char* function, int line) {
  char fmt[] =
      "{codec}[\e[95m%d\e[39m] <<\e[31m%s\e[39m>> [\e[93m%s\e[39m -> \e[92m%s\e[39m] (%s | %s | %d)\n";
  printf(fmt, codec_operations, type, input, output, src, function, line);
  codec_operations++;
}
