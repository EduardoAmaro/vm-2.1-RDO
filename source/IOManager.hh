/*
 * IOManager.hh
 *
 * Author: Pedro Garcia Freitas
 *         <pedro.gf@samsung.com>
 */

#ifndef IOMANAGER_HH_
#define IOMANAGER_HH_

#include <cstdio>
#include <cstring>
#include <string>
#include <experimental/filesystem>

#ifdef _MSC_VER
//#define TARPATH "C:/Local/astolap/Programs/tar-1.13-1-bin/bin/tar"
#define TARPATH "C:/Local/astolap/Programs/GnuWin32/bin/bsdtar"
#endif

#ifndef _MSC_VER
#define TARPATH "tar"
#endif

namespace fs = std::experimental::filesystem;

using namespace std;

class IOManager {
 private:
  static IOManager& getInstance();
  IOManager();
  virtual ~IOManager();
  static FILE* read_only_fopen(const char *filename, const char *mode);
  static FILE* writeable_fopen(const char *filename, const char *mode);
  static bool is_writable(const char*);

 public:
  IOManager(IOManager const&) = delete;
  void operator=(IOManager const&) = delete;
  static FILE* fopen(const char *filename, const char *mode);
  static FILE* fopen(string filename, const char *mode);
  static void ensure_directory(string filename);
  static void ensure_directory(const char* filename);
  static fs::directory_iterator list_directory(const char *dir);
  static fs::directory_iterator list_directory(string dir);
  static void mkdir(string path);
  static void mkdir(const char *dir);
  static void rm(const char *dir);
  static void rm(string dir);
  static void copy(string, string);
  static bool exists(string path);
  static bool exists(const char* path);
};

#endif /* IOMANAGER_HH_ */
