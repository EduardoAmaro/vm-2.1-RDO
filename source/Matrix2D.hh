/*
 * Matrix2D.h
 *
 * Author: Pedro Garcia Freitas
 *         <pedro.gf@samsung.com>
 */

#include <vector>
#include <stdexcept>
#include <algorithm>
#include <iostream>

using namespace std;

#ifndef MATRIX2D_H_
#define MATRIX2D_H_

template<typename T>
class Matrix2D {
 private:
  vector<std::vector<T>> data;
  unsigned int sizeX, sizeY;
  unsigned int offsetX, offsetY;

 public:
  ~Matrix2D() {
  }

  Matrix2D(unsigned int left, unsigned int right, unsigned int top,
           unsigned int down, T v)
      : sizeX(right - left + 1),
        sizeY(down - top + 1),
        offsetX(left),
        offsetY(top) {
    if (this->data.empty()) {
      for (uint i = 0; i < sizeY; ++i) {
        vector<T> tmp;
        for (uint j = 0; j < sizeX; j++)
          tmp.push_back(0);
        this->data.push_back(std::move(tmp));
      }
    }
  }

  T get(unsigned int i, unsigned int j) {
    unsigned int y = i - offsetX;
    unsigned x = j - offsetY;
    if (x >= sizeX || y >= sizeY)
      throw std::out_of_range("index is out of range");
    return this->data[y][x];
  }

  void set(unsigned int i, unsigned int j, T value) {
    unsigned int y = i - offsetX;
    unsigned x = j - offsetY;
    if (x >= sizeX || y >= sizeY)
      throw std::out_of_range("index is out of range");
    this->data[y][x] = value;
  }

//    ostream& operator<<(ostream &strm, const Matrix2D<T> &a) {
//      for (unsigned int i = offsetX; i < sizeX; ++i)
//        for (unsigned int j = offsetY; j < sizeY; ++j)
//          strm << data[sizeX * j + i] << (i < sizeX - 1) ? " " : "\n";
//      return strm;
//    }
};

#endif /* MATRIX2D_H_ */
