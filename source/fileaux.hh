/* fileaux.hh */
/* Author: Pekka Astola */
/* <pekka.astola@tuni.fi>*/

#ifndef FILEAUX_HH
#define FILEAUX_HH

#ifdef __unix__
#define _popen popen
#define _pclose pclose
#endif

#include <string>

using namespace std;


int system_1(char *str);

long aux_GetFileSize(const char* filename);
long aux_GetFileSize(char* filename);
long aux_GetFileSize(string filename);

#endif
