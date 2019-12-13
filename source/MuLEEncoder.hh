/*
 * MuLEEncoder.hh
 *
 * Author: Pedro Garcia Freitas
 *         <pedro.gf@samsung.com>
 */

#include "CoderSetup.hh"

#ifndef SOURCE_MULEENCODER_HH_
#define SOURCE_MULEENCODER_HH_

class MuLEEncoder {
 private:
  CoderSetup setup;
 public:
  MuLEEncoder(CoderSetup coderSetup);
  virtual ~MuLEEncoder();
  void run(void);
};

#endif /* SOURCE_MULEENCODER_HH_ */
