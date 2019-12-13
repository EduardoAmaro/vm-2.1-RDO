/*
 * CodecType.hh
 *
 * Author: Pedro Garcia Freitas
 *         <pedro.gf@samsung.com>
 */

#ifndef SOURCE_CODECTYPE_HH_
#define SOURCE_CODECTYPE_HH_

enum class CodecType
  : int {MuLE,
  WaSP
};


std::istream &operator>>(std::istream &in, CodecType &level);
std::ostream &operator<<(std::ostream &in, const CodecType &level);

#endif /* SOURCE_CODECTYPE_HH_ */
