//
// Created by Gregorio Benatti on 06/11/2018.
//

#ifndef BMP_DDS_DDS_BMP_DDSFILE_H
#define BMP_DDS_DDS_BMP_DDSFILE_H

#include <string>
#include <stdint.h>


typedef unsigned long DWORD;

struct DDS_PIXELFORMAT {
    DWORD dwSize;
    DWORD dwFlags;
    DWORD dwFourCC;
    DWORD dwRGBBitCount;
    DWORD dwRBitMask;
    DWORD dwGBitMask;
    DWORD dwBBitMask;
    DWORD dwABitMask;
};

typedef struct {
    DWORD           dwSize;
    DWORD           dwFlags;
    DWORD           dwHeight;
    DWORD           dwWidth;
    DWORD           dwPitchOrLinearSize;
    DWORD           dwDepth;
    DWORD           dwMipMapCount;
    DWORD           dwReserved1[11];
    DDS_PIXELFORMAT ddspf;
    DWORD           dwCaps;
    DWORD           dwCaps2;
    DWORD           dwCaps3;
    DWORD           dwCaps4;
    DWORD           dwReserved2;
} DDS_HEADER;

typedef struct {
    DDS_HEADER* header;
    uint8_t* dataBuffer;
} DDSSTRUCT;


class DDSFile {
public:
    DDSFile();
    virtual ~DDSFile();

    DDSSTRUCT DDSInit(std::string fileName);


private:
    DDS_HEADER* ddsHeader;
    uint8_t* dataBuffer;
};


#endif //BMP_DDS_DDS_BMP_DDSFILE_H
