//
// Created by Gregorio Benatti on 13/11/2018.
//

#ifndef BMP_DDS_DDS_BMP_CONVERTER_H
#define BMP_DDS_DDS_BMP_CONVERTER_H

#include "BMPFile.h"
#include "DDSFile.h"


#define BF_TYPE_MB 0x4D42
#define BIT_COUNT_24 24
#define BI_RGB 0x0000
#define PIXELS_PER_METER 2835
#define NUM_OF_PLANES 1

#ifndef MAKEFOURCC
#define MAKEFOURCC(c0,c1,c2,c3) ((unsigned int)(char)(c0)|((unsigned int)(char)(c1)<<8)|((unsigned int)(char)(c2)<<16)|((unsigned int)(char)(c3)<<24))
#endif

#define DDPF_FOURCC 0x4
#define DXT1_FOURCC (MAKEFOURCC('D', 'X', 'T', '1'))
#define FLAGS 0x81007
#define HEADER_SIZE 124
#define INFO_SIZE 32
#define DDSCAPS_TEXTURE 0x1000


class Converter {
public:
    Converter();
    virtual ~Converter();

    int convertDDSToBMP(DDS_HEADER header) ;
    int convertBMPToDDS(BITMAPFILEHEADER* fileHeader, BITMAPINFOHEADER* inforHeader);

    bool isFileSizeValid(int width, int height);

private:
    uint8_t* dataBuffer; //DDS
    uint8_t* pixels;     //BMP

};


#endif //BMP_DDS_DDS_BMP_CONVERTER_H
