//
// Created by Gregorio Benatti on 13/11/2018.
//

#include "Converter.h"
#include "BMPFile.h"
#include "DDSFile.h"
#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <iterator>
#include <sstream>
#include <fstream>


Converter::Converter()
{
    dataBuffer = nullptr;
    pixels = nullptr;
}

Converter::~Converter()
{
    if (dataBuffer != nullptr) { delete[] dataBuffer; }
    if (pixels != nullptr)     { delete[] pixels; }
}


int Converter::convertDDSToBMP(DDS_HEADER header)
{
    std::string fileName = "output.bmp";
    std::ofstream outputFile;
    outputFile.open(fileName, std::ofstream::out | std::ofstream::binary | std::ofstream::app);

    if (!isFileSizeValid(header.dwWidth, header.dwHeight))
    {
        std::cout << "File size not valid" << std::endl;
        return 0;
    }

    if (outputFile.is_open())
    {
        /*
         *
         *
         * DDS_PIXELFORMAT pixelFormat;
    pixelFormat.dwSize        = INFO_SIZE;
    pixelFormat.dwFlags       = DDPF_FOURCC;
    pixelFormat.dwFourCC      = DXT1_FOURCC;
    pixelFormat.dwRGBBitCount = 0;
    pixelFormat.dwRBitMask    = 0;
    pixelFormat.dwGBitMask    = 0;
    pixelFormat.dwBBitMask    = 0;
    pixelFormat.dwABitMask    = 0;

    ddsHeader->dwSize              = HEADER_SIZE;
    ddsHeader->dwFlags             = FLAGS;
    ddsHeader->dwHeight            = height;
    ddsHeader->dwWidth             = width;
    ddsHeader->dwPitchOrLinearSize = std::max(1, (int)(ddsHeader->dwWidth + 3) / 4) * std::max(1, (int)(ddsHeader->dwHeight + 3) / 4) * 8;
    ddsHeader->dwDepth             = 0;
    ddsHeader->dwMipMapCount       = 0;
    ddsHeader->dwReserved1[11];
    ddsHeader->ddspf               = pixelFormat;
    ddsHeader->dwCaps              = DDSCAPS_TEXTURE;
    ddsHeader->dwCaps2             = 0;
    ddsHeader->dwCaps3             = 0;
    ddsHeader->dwCaps4             = 0;
    ddsHeader->dwReserved2         = 0;
        */

        outputFile.write((const char *)&fileHeader->bfType, 2);      // bfType
        outputFile.write((const char *)&fileHeader->bfSize, 4);      // bfSize
        outputFile.write((const char *)&fileHeader->bfReserved1, 2); // bfReserved1
        outputFile.write((const char *)&fileHeader->bfReserved2, 2); // bfReserved2
        outputFile.write((const char *)&fileHeader->bfOffBits, 4);   // bfOffBits,

        outputFile.write((const char *)&infoHeader->biSize, 4);          // biSize
        outputFile.write((const char *)&infoHeader->biWidth, 4);         // biWidth
        outputFile.write((const char *)&infoHeader->biHeight, 4);        // biHeight
        outputFile.write((const char *)&infoHeader->biPlanes, 2);        // biPlanes
        outputFile.write((const char *)&infoHeader->biBitCount, 2);      // biBitCount
        outputFile.write((const char *)&infoHeader->biCompression, 4);   // biCompression
        outputFile.write((const char *)&infoHeader->biSizeImage, 4);     // biSizeImage
        outputFile.write((const char *)&infoHeader->biXPelsPerMeter, 4); // biXPelsPerMeter
        outputFile.write((const char *)&infoHeader->biYPelsPerMeter, 4); // biYPelsPerMeter
        outputFile.write((const char *)&infoHeader->biClrUsed, 4);       // biClrUsed
        outputFile.write((const char *)&infoHeader->biClrImportant, 4);  // biClrImportant

        // pixel =

        unsigned int imageSize = infoHeader->biSizeImage;
        uint8_t* pCopy = new uint8_t[imageSize];
        memcpy(pCopy, pixels, imageSize);

        for (unsigned long i = 0; i < imageSize; i += 3) {
            uint8_t tmpRGB = 0;
            tmpRGB = pCopy[i];
            pCopy[i] = pCopy[i + 2];
            pCopy[i + 2] = tmpRGB;
        }

        unsigned int arraySize = infoHeader->biSizeImage;
        for (unsigned int i = 0; i < arraySize; ++i) {
            outputFile.write((const char *)&pCopy[i], 1);
        }
        outputFile.close();

        delete[] pCopy;
    }

    return 0;
}


int Converter::convertBMPToDDS(BITMAPFILEHEADER* fileHeader, BITMAPINFOHEADER* inforHeader)
{
    std::string fileName = "output.dds";
    std::ofstream outputFile;
    outputFile.open(fileName, std::ofstream::out | std::ofstream::binary | std::ofstream::app);

    if (outputFile.is_open())
    {
        outputFile.write((const char *)"DDS ", 4);
        outputFile.write((const char *)&header->dwSize, 4);              // dwSize
        outputFile.write((const char *)&header->dwFlags, 4);             // dwFlags
        outputFile.write((const char *)&header->dwHeight, 4);            // dwHeight
        outputFile.write((const char *)&header->dwWidth, 4);             // dwWidth
        outputFile.write((const char *)&header->dwPitchOrLinearSize, 4); // dwPitchOrLinearSize
        outputFile.write((const char *)&header->dwDepth, 4);             // dwDepth
        outputFile.write((const char *)&header->dwMipMapCount, 4);       // dwMipMapCount
        outputFile.write((const char *)&header->dwReserved1, 44);        // dwReserved1

        outputFile.write((const char *)&header->ddspf.dwSize, 4);        // ddspf.dwSize
        outputFile.write((const char *)&header->ddspf.dwFlags, 4);       // ddspf.dwFlags
        outputFile.write((const char *)&header->ddspf.dwFourCC, 4);      // ddspf.dwFourCC
        outputFile.write((const char *)&header->ddspf.dwRGBBitCount, 4); // ddspf.dwRGBBitCount
        outputFile.write((const char *)&header->ddspf.dwRBitMask, 4);    // ddspf.dwRBitMask
        outputFile.write((const char *)&header->ddspf.dwGBitMask, 4);    // ddspf.dwGBitMask
        outputFile.write((const char *)&header->ddspf.dwBBitMask, 4);    // ddspf.dwBBitMask
        outputFile.write((const char *)&header->ddspf.dwABitMask, 4);    // ddspf.dwABitMask

        outputFile.write((const char *)&header->dwCaps, 4);      // dwCaps
        outputFile.write((const char *)&header->dwCaps2, 4);     // dwCaps2
        outputFile.write((const char *)&header->dwDepth, 4);     // dwDepth
        outputFile.write((const char *)&header->dwCaps3, 4);     // dwCaps3
        outputFile.write((const char *)&header->dwReserved2, 4); // dwReserved2

        // dataBuffer =

        unsigned int arraySize = header->dwPitchOrLinearSize;
        for (unsigned int i = 0; i < arraySize; ++i) {
            outputFile.write((const char *)&dataBuffer[i], 1);
        }

        outputFile.close();
    }

    return 0;
}


bool Converter::isFileSizeValid(int width, int height)
{
    return ((width%4 == 0) && (height%4 == 0)) ? true : false;
}
