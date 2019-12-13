#include "Encoder.hh"
#include "WaSPEncoder.hh"
#include "MuLEEncoder.hh"

#include "CodecType.hh"

Encoder::Encoder(CoderSetup coderSetup)
    : setup(coderSetup) {
}

//TODO: It can be better whether rewritten using polimorphic OO resources
void Encoder::run(void) {
  if (setup.type == CodecType::WaSP)
    runWaspEncoder();
  else if (setup.type == CodecType::MuLE)
    runMuleEncoder();
}


Encoder::~Encoder() {
  // TODO Auto-generated destructor stub
}

void Encoder::runWaspEncoder() {
  WaSPEncoder encoder(setup);
  encoder.run();
}

void Encoder::runMuleEncoder() {
  MuLEEncoder encoder(setup);
  encoder.run();
}
