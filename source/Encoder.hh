/*
 * Encoder.hh
 *
 * Author: Pedro Garcia Freitas
 *         <pedro.gf@samsung.com>
 */

#ifndef ENCODER_HH
#define ENCODER_HH


#include "CoderSetup.hh"

using namespace std;

class Encoder {
 private:
  CoderSetup setup;
  void runWaspEncoder();
  void runMuleEncoder();
 public:
  Encoder(CoderSetup);
  virtual ~Encoder();
  void run(void);

};

#endif
