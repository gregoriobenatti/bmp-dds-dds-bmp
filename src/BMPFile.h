//
// Created by Gregorio Benatti on 06/11/2018.
//

#ifndef BMP_DDS_DDS_BMP_BMPFILE_H
#define BMP_DDS_DDS_BMP_BMPFILE_H


#include <string>
#include <stdint.h>


typedef struct {
    unsigned short bfType;
    unsigned int   bfSize;
    unsigned short bfReserved1;
    unsigned short bfReserved2;
    unsigned int   bfOffBits;
} BITMAPFILEHEADER;


typedef struct {
    unsigned int   biSize;
    int            biWidth;
    int            biHeight;
    unsigned short biPlanes;
    unsigned short biBitCount;
    unsigned int   biCompression;
    unsigned int   biSizeImage;
    int            biXPelsPerMeter;
    int            biYPelsPerMeter;
    unsigned int   biClrUsed;
    unsigned int   biClrImportant;
} BITMAPINFOHEADER;


typedef struct {
    BITMAPFILEHEADER* fileHeader;
    BITMAPINFOHEADER* infoHeader;
    uint8_t* pixels;
} BMPSTRUCT;


class BMPFile {
public:
    BMPFile();
    virtual ~BMPFile();

    BMPSTRUCT BMPInit(std::string fileName);


private:
    BITMAPFILEHEADER* bmpHeader;
    BITMAPINFOHEADER* bmpInfoHeader;
    uint8_t* pixels;
};


#endif //BMP_DDS_DDS_BMP_BMPFILE_H
