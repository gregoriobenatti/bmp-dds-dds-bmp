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


#ifndef MAKEFOURCC
#define MAKEFOURCC(c0,c1,c2,c3) ((unsigned int)(char)(c0)|((unsigned int)(char)(c1)<<8)|((unsigned int)(char)(c2)<<16)|((unsigned int)(char)(c3)<<24))
#endif

#define DDPF_FOURCC 0x4
#define DXT1_FOURCC (MAKEFOURCC('D', 'X', 'T', '1'))
#define FLAGS 0x81007
#define HEADER_SIZE 124
#define INFO_SIZE 32
#define DDSCAPS_TEXTURE 0x1000


class DDSFile {
public:
    DDSFile();
    virtual ~DDSFile();

    void DDSInit(std::string fileName);
    void createDDSFile(unsigned long height, unsigned long width);
    void writeFile(DDS_HEADER* header);
    bool isFileSizeValid(int width, int height);


private:
    DDS_HEADER* ddsHeader;
    uint8_t* dataBuffer;
};


#endif //BMP_DDS_DDS_BMP_DDSFILE_H
