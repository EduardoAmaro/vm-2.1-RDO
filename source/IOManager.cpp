/*
 * IOManager.cpp
 *
 * Author: Pedro Garcia Freitas
 *         <pedro.gf@samsung.com>
 */
#include <experimental/filesystem>
#include <iostream>
#include <vector>
#include <string>
#include "IOManager.hh"
#include "exceptions.hh"

namespace fs = std::experimental::filesystem;

IOManager::IOManager() {

}

IOManager& IOManager::getInstance() {
  static IOManager instance;
  return instance;
}

IOManager::~IOManager() {
  // TODO Auto-generated destructor stub
}

FILE* IOManager::read_only_fopen(const char* filename, const char* mode) {
  return std::fopen(filename, mode);
}

FILE* IOManager::writeable_fopen(const char* filename, const char* mode) {
  IOManager::ensure_directory(filename);
  return std::fopen(filename, mode);
}

FILE* IOManager::fopen(const char* filename, const char* mode) {
  if (is_writable(mode))
    return IOManager::writeable_fopen(filename, mode);
  else
    return IOManager::read_only_fopen(filename, mode);
}

FILE* IOManager::fopen(string filename, const char* mode) {
  return IOManager::fopen(filename.c_str(), mode);
}

void IOManager::ensure_directory(string filename) {
  fs::path file_path(filename);
  fs::path parent_path = file_path.parent_path();
  if (!exists(parent_path.u8string()))
    fs::create_directories(parent_path);
}

void IOManager::ensure_directory(const char* filename) {
  string fname(filename);
  IOManager::ensure_directory(fname);
}

bool IOManager::is_writable(const char* mode) {
  string smode(mode);
  return smode.find('w') != std::string::npos
      || smode.find('a') != std::string::npos;
}

fs::directory_iterator IOManager::list_directory(const char* dir) {
  return fs::directory_iterator(dir);
}

fs::directory_iterator IOManager::list_directory(string dir) {
  return list_directory(dir.c_str());
}

void IOManager::copy(string from, string to) {
  if (fs::exists(from)) {
    IOManager::ensure_directory(to);
    fs::copy_file(from, to, fs::copy_options::overwrite_existing);
  } else {
    throw FileDoesNotExistsException(from);
  }
}

bool IOManager::exists(const char* filename) {
  fs::path file_path(filename);
  return fs::exists(file_path);
}

bool IOManager::exists(string filename) {
  return exists(filename.c_str());
}

void IOManager::mkdir(const char *path) {
  if (!fs::exists(path))
    fs::create_directories(path);
}

void IOManager::mkdir(string path) {
  mkdir(path.c_str());
}

void IOManager::rm(const char* dir) {
  fs::remove_all(dir);
}

void IOManager::rm(string dir) {
  rm(dir.c_str());
}
