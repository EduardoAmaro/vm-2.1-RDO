CC=g++

CFLAGS=-I. -std=c++11 -O2 -g -lstdc++fs -fPIC -fopenmp

HH_DEPS = source/IOManager.hh \
  source/Decoder.hh \
  source/Encoder.hh \
  source/bitdepth.hh \
  source/clip.hh \
  source/fastols.hh \
  source/fileaux.hh \
  source/inpainting.hh \
  source/medianfilter.hh \
  source/merging.hh \
  source/minconf.hh \
  source/ppm.hh \
  source/predictdepth.hh \
  source/psnr.hh \
  source/residualjp2.hh \
  source/sparsefilter.hh \
  source/warping.hh \
  source/view.hh \
  source/ycbcr.hh \
  source/CoderSetup.hh \
  source/iolog.hh \
  source/exceptions.hh \
  source/SparseToDenseLFSynthesizer.hh \
  source/Matrix2D.hh \
  source/MuLEncoderWrapper.hh \
  source/MuLEDecoderWrapper.hh \
  source/Unshifter.hh \
  source/thirdpartlibs/CLI11.hpp \
  source/thirdpartlibs/json.hh \
  source/thirdpartlibs/split.h \
  source/thirdpartlibs/join.hh \
  source/codestream.hh \
  source/WaSPEncoder.hh \
  source/WaSPDecoder.hh \
  source/CodecType.hh \
  source/MuLEEncoder.hh \
  source/MuLEDecoder.hh

OBJ_ENCODER = source/jpl-vm20-encoder.o \
  source/Encoder.cpp \
  source/iolog.cpp \
  source/fastols.cpp \
  source/fileaux.cpp \
  source/merging.cpp \
  source/minconf.cpp \
  source/ppm.cpp \
  source/predictdepth.cpp \
  source/psnr.cpp \
  source/residualjp2.cpp \
  source/sparsefilter.cpp \
  source/MuLEEncoder.cpp \
  source/warping.cpp \
  source/view.cpp \
  source/ycbcr.cpp \
  source/IOManager.cpp \
  source/CoderSetup.cpp \
  source/MuLEncoderWrapper.cpp \
  source/SparseToDenseLFSynthesizer.cpp \
  source/codestream.cpp \
  source/WaSPEncoder.cpp \
  source/CodecType.cpp
  

OBJ_DECODER = source/jpl-vm20-decoder.o \
  source/Decoder.cpp \
  source/iolog.cpp \
  source/fastols.cpp \
  source/fileaux.cpp \
  source/merging.cpp \
  source/minconf.cpp \
  source/ppm.cpp \
  source/predictdepth.cpp \
  source/psnr.cpp \
  source/residualjp2.cpp \
  source/sparsefilter.cpp \
  source/warping.cpp \
  source/view.cpp \
  source/ycbcr.cpp \
  source/IOManager.cpp \
  source/DecoderSetup.cpp \
  source/MuLEDecoderWrapper.cpp \
  source/Unshifter.cpp \
  source/codestream.cpp \
  source/WaSPDecoder.cpp \
  source/CodecType.cpp \
  source/MuLEDecoder.cpp
 

MULE_HH = source/mule/Aritmetico.h source/mule/Hierarchical4DEncoder.h \
  source/mule/MultiscaleTransform.h source/mule/View.h source/mule/Block4D.h \
  source/mule/LightField.h source/mule/PartitionDecoder.h \
  source/mule/Hierarchical4DDecoder.h source/mule/Matrix.h \
  source/mule/TransformPartition.h source/mule/Prediction.h source/mule/Statistics.h

MULE_ENCODER = source/mule/Matrix.cpp source/mule/Block4D.cpp \
  source/mule/MultiscaleTransform.cpp source/mule/View.cpp \
  source/mule/LightField.cpp source/mule/Aritmetico.cpp \
  source/mule/TransformPartition.cpp source/mule/Hierarchical4DEncoder.cpp \
  source/mule/encoder.cpp source/mule/Prediction.cpp source/mule/Statistics.cpp

MULE_DECODER = source/mule/Matrix.cpp source/mule/Block4D.cpp \
  source/mule/MultiscaleTransform.cpp source/mule/View.cpp \
  source/mule/LightField.cpp source/mule/Aritmetico.cpp \
  source/mule/PartitionDecoder.cpp source/mule/Hierarchical4DDecoder.cpp \
  source/mule/decoder.cpp source/mule/Prediction.cpp source/mule/Statistics.cpp

%.o: %.cpp $(HH_DEPS) $(MULE_HH)
	$(CC) -c -o $@ $< $(CFLAGS)

jpl-vm20-encoder-bin: $(OBJ_ENCODER)
	$(CC) -o $@ $^ $(CFLAGS)

jpl-vm20-decoder-bin: $(OBJ_DECODER)
	$(CC) -o $@ $^ $(CFLAGS)

mule-encoder-bin: $(MULE_ENCODER)
	$(CC) -o $@ $^ $(CFLAGS)

mule-decoder-bin: $(MULE_DECODER)
	$(CC) -o $@ $^ $(CFLAGS)


all: jpl-vm20-encoder-bin jpl-vm20-decoder-bin mule-encoder-bin mule-decoder-bin

clean:
	rm source/*.o jpl-vm20-encoder-bin jpl-vm20-decoder-bin mule-encoder-bin mule-decoder-bin

