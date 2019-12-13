/*
 * CodecType.cpp
 *
 * Author: Pedro Garcia Freitas
 *         <pedro.gf@samsung.com>
 */

#include <sstream>

#include "CodecType.hh"

using namespace std;


std::istream &operator>>(std::istream &in, CodecType &level) {
  int i;
  in >> i;
  level = static_cast<CodecType>(i);
  return in;
}

std::ostream &operator<<(std::ostream &in, const CodecType &level) {
  return in << static_cast<int>(level);
}
