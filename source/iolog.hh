#ifndef IOLOG_HH
#define IOLOG_HH

#include <cstdlib>
#include <cstdio>
#include <cstring>

extern long io_operations;

void print_io_log(const char*, const char*, const char*, int, const char*);

void print_codec_JP2_call(const char*, const char*, const char*, const char*,
                          const char*, int);

#endif

