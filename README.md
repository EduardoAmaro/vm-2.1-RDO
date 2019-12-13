# JPEG Pleno Verification Model 2.0

---------------------------------------


## Introduction

Unzip **wasp-jpl-vm2.0.zip** to a directory. Probably, the extracted directory will be **wasp-jpl-vm2.0**. We will assume that the default directory is **wasp-jpl-vm2.0** in this documentation.

The JPEG Pleno (JPL) Verification Model 2.0 (VM20) is composed by four executables:

1. jpl-vm20-encoder-bin
2. jpl-vm20-decoder-bin
3. mule-encoder-bin
4. mule-decoder-bin


In order to build these exectuables, please run the following commands:

1. `make clean` (to remove the object and executable files)
2. `make all` (to compile VM 2.0)

After running the `make all` command, the following executable files should be generated: **mule-encoder-bin**, **mule-decoder-bin**, **jpl-vm20-encoder-bin** and **jpl-vm20-decoder-bin**

Directory **MuLE_CFGs/** contains sample MuLE configuration files

The software has a help message that explains its command line parameters.

---------------------------------------


## JPL Encoder help message

 ./jpl-vm20-encoder-bin --help  

JPL-VM-20 Encoder  

Usage: ./jpl-vm20-encoder-bin [OPTIONS]  

Options:  
-h,--help                   Print this help message and exit  
-i,--input TEXT REQUIRED    Input directory containing a set of uncompressed light field images (xxx_yyy.ppm).  
-o,--output TEXT REQUIRED   Output directory containing temporary light field data and the compressed bitstream.  
-c,--config TEXT REQUIRED   Path to config file (.conf for WaSP or .cfg for MuLE)  
-t,--type enum/CodecType in {MuLE=0, WaSP=1} REQUIRED  
Codec type  
-k,--kakadu TEXT            Directory containing the Kakadu codec (kdu_compress and kdu_expand).  
-m,--mule TEXT              Directory containing the MuLE codec (default directory).  
System command: /mule-encoder-bin -lf / -o /output.LF -cf bikes_example_MuLE.cfg


## JPL Decoder help message

./jpl-vm20-decoder-bin --help  
JPL-VM-20 Decoder  
Usage: ./jpl-vm20-decoder-bin [OPTIONS]  

Options:  
-h,--help                   Print this help message and exit  
-i,--input TEXT REQUIRED    Input coded light field bitstream (output.LF)  
-o,--output TEXT REQUIRED   Output directory the decoded light field in format xxx_yyy.ppm  
-t,--type enum/CodecType in {MuLE=0, WaSP=1} REQUIRED  
Codec type  
-k,--kakadu TEXT            Directory containing the Kakadu codec (kdu_compress and kdu_expand)  
-m,--mule TEXT              Directory containing the MuLE codec


---------------------------------------


## Command line parameters --- WaSP

### encode with WaSP

```
./jpl-vm20-encoder-bin --type 1 \
    --input <Input directory containing a set of uncompressed light field images (SSS_TTT.ppm)> \
    --output <Output directory containing temporary light field data and the compressed bitstream> \
    --kakadu <Directory containing the Kakadu codec (kdu_compress)> \
    --config <Path to config file (.conf for WaSP)>
```

#### example (encode with WaSP)

```
./jpl-vm20-encoder-bin --type 1 \
    --input /home/LF/datasets/greek \
    --output /home/LF/wasp-encoded-greek/ \
    --kakadu /home/LF/kakadu-linux \
    --config /home/LF/WASP_CONFs/greek-81_46.2945_0.11016.conf
```


### decode with WaSP

```
./jpl-vm20-decoder-bin --type 1 \
    --input <Input coded light field bitstream (output.LF)> \
    --output <Output directory the decoded light field in format SSS_TTT.ppm> \
    --kakadu /home/LF/kakadu-linux
```


#### example (decode with WaSP)

```
./jpl-vm20-decoder-bin --type 1 \
    --input /home/LF/wasp-encoded-greek/output.LF \
    --output /home/LF/wasp-decoded-greek/ \
    --kakadu /home/LF/kakadu-linux
```

---------------------------------------


## Command line parameters --- MuLE

### encode with MuLE

```
./jpl-vm20-encoder-bin --type 0  \
    --input <Input directory containing a set of uncompressed light field images (SSS_TTT.ppm)> \
    --output Output directory containing temporary light field data and the compressed bitstream> \
    --config <Path to config file (.cfg for MuLE)> \
    --mule <Directory containing the MuLE codec (by default, it's the same directory of jpl-vm20-encoder-bin)>```
```

#### example (encode with MuLE)

```
./jpl-vm20-encoder-bin --type 0 \
    --input /home/LF/datasets/bikes \
    --output /home/LF/mule-encoded-bikes/ \
    --config /home/LF/MULE_CFGs/bikes_example_MuLE.cfg \
    --mule .
```


### decode with MuLE

```
./jpl-vm20-decoder-bin --type 0 \
    --input <Input coded light field bitstream (output.LF)> \
    --output  <Output directory the decoded light field in format SSS_TTT.ppm> \
    --mule . \
    --config <Path to config file (.cfg for MuLE)> \
```

#### example (decode with MuLE)

```
./jpl-vm20-decoder-bin --type 0 \
    --input /home/LF/mule-encoded-bikes/output.LF \
    --output /home/LF/mule-decoded-bikes/ \
    --mule . \
    --config /home/LF/MULE_CFGs/bikes_example_MuLE.cfg
```


