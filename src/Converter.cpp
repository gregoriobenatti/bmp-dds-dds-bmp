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


int Converter::convertDDSToBMP(DDS_HEADER header) // change to pointer??
{
    // create BMP structure, refactor, put in other function
    BITMAPFILEHEADER* bmpHeader = new BITMAPFILEHEADER();
    BITMAPINFOHEADER* bmpInfoHeader = new BITMAPINFOHEADER();

    bmpHeader->bfType = BF_TYPE_MB;
    bmpHeader->bfSize = (header.dwWidth * header.dwHeight * 3) + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    bmpHeader->bfReserved1 = 0;
    bmpHeader->bfReserved2 = 0;
    bmpHeader->bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    bmpInfoHeader->biSize = sizeof(BITMAPINFOHEADER);
    bmpInfoHeader->biWidth = header.dwWidth;
    bmpInfoHeader->biHeight = header.dwHeight;
    bmpInfoHeader->biPlanes = NUM_OF_PLANES;
    bmpInfoHeader->biBitCount = BIT_COUNT_24;
    bmpInfoHeader->biCompression = BI_RGB;
    bmpInfoHeader->biSizeImage = (header.dwWidth * header.dwHeight * 3);
    bmpInfoHeader->biXPelsPerMeter = PIXELS_PER_METER;
    bmpInfoHeader->biYPelsPerMeter = PIXELS_PER_METER;
    bmpInfoHeader->biClrUsed = 0;
    bmpInfoHeader->biClrImportant = 0;

    pixels = BMPUncompressedImageData(bmpInfoHeader->biSizeImage);


    // write file BMP
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
        outputFile.write((const char *)&bmpHeader->bfType, 2);
        outputFile.write((const char *)&bmpHeader->bfSize, 4);
        outputFile.write((const char *)&bmpHeader->bfReserved1, 2);
        outputFile.write((const char *)&bmpHeader->bfReserved2, 2);
        outputFile.write((const char *)&bmpHeader->bfOffBits, 4);

        outputFile.write((const char *)&bmpInfoHeader->biSize, 4);
        outputFile.write((const char *)&bmpInfoHeader->biWidth, 4);
        outputFile.write((const char *)&bmpInfoHeader->biHeight, 4);
        outputFile.write((const char *)&bmpInfoHeader->biPlanes, 2);
        outputFile.write((const char *)&bmpInfoHeader->biBitCount, 2);
        outputFile.write((const char *)&bmpInfoHeader->biCompression, 4);
        outputFile.write((const char *)&bmpInfoHeader->biSizeImage, 4);
        outputFile.write((const char *)&bmpInfoHeader->biXPelsPerMeter, 4);
        outputFile.write((const char *)&bmpInfoHeader->biYPelsPerMeter, 4);
        outputFile.write((const char *)&bmpInfoHeader->biClrUsed, 4);
        outputFile.write((const char *)&bmpInfoHeader->biClrImportant, 4); 

        unsigned int imageSize = bmpInfoHeader->biSizeImage;
        uint8_t* pCopy = new uint8_t[imageSize];
        memcpy(pCopy, pixels, imageSize);

        for (unsigned long i = 0; i < imageSize; i += 3) {
            uint8_t tmpRGB = 0;
            tmpRGB = pCopy[i];
            pCopy[i] = pCopy[i + 2];
            pCopy[i + 2] = tmpRGB;
        }

        unsigned int arraySize = bmpInfoHeader->biSizeImage;
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
    // create BMP structure, refactor, put in other function
    //Create header
    DDS_HEADER* ddsHeader = new DDS_HEADER();

    ddsHeader->dwSize = HEADER_SIZE;
    ddsHeader->dwFlags = FLAGS;
    ddsHeader->dwHeight = inforHeader->biWidth;
    ddsHeader->dwWidth = inforHeader->biHeight;
    ddsHeader->dwPitchOrLinearSize = std::max((unsigned int)1, (unsigned int)(ddsHeader->dwWidth + 3) / 4) * std::max((unsigned int)1, (unsigned int)(ddsHeader->dwHeight + 3) / 4) * 8;
    ddsHeader->dwDepth = 0;
    ddsHeader->dwMipMapCount = 0;
    ddsHeader->dwReserved1[11];

    //Create pixel format
    DDS_PIXELFORMAT pixelFormat;
    pixelFormat.dwSize = INFO_SIZE;
    pixelFormat.dwFlags = DDPF_FOURCC;
    pixelFormat.dwFourCC = DXT1_FOURCC;
    pixelFormat.dwRGBBitCount = 0;
    pixelFormat.dwRBitMask = 0;
    pixelFormat.dwGBitMask = 0;
    pixelFormat.dwBBitMask = 0;
    pixelFormat.dwABitMask = 0;

    ddsHeader->ddspf = pixelFormat;
    ddsHeader->dwCaps = DDSCAPS_TEXTURE;
    ddsHeader->dwCaps2 = 0;
    ddsHeader->dwCaps3 = 0;
    ddsHeader->dwCaps4 = 0;
    ddsHeader->dwReserved2 = 0;

    dataBuffer = DDSUncompressedImageData(inforHeader->biWidth, inforHeader->biHeight); //DXT1Compress(uncompressedImageData, m_pDdsHeader->dwPitchOrLinearSize, imageSize, width, height);


    // write file DDS
    std::string fileName = "output.dds";
    std::ofstream outputFile;
    outputFile.open(fileName, std::ofstream::out | std::ofstream::binary | std::ofstream::app);

    if (outputFile.is_open())
    {
        outputFile.write((const char *)"DDS ", 4);
        outputFile.write((const char *)&ddsHeader->dwSize, 4);              // dwSize
        outputFile.write((const char *)&ddsHeader->dwFlags, 4);             // dwFlags
        outputFile.write((const char *)&ddsHeader->dwHeight, 4);            // dwHeight
        outputFile.write((const char *)&ddsHeader->dwWidth, 4);             // dwWidth
        outputFile.write((const char *)&ddsHeader->dwPitchOrLinearSize, 4); // dwPitchOrLinearSize
        outputFile.write((const char *)&ddsHeader->dwDepth, 4);             // dwDepth
        outputFile.write((const char *)&ddsHeader->dwMipMapCount, 4);       // dwMipMapCount
        outputFile.write((const char *)&ddsHeader->dwReserved1, 44);        // dwReserved1

        outputFile.write((const char *)&ddsHeader->ddspf.dwSize, 4);        // ddspf.dwSize
        outputFile.write((const char *)&ddsHeader->ddspf.dwFlags, 4);       // ddspf.dwFlags
        outputFile.write((const char *)&ddsHeader->ddspf.dwFourCC, 4);      // ddspf.dwFourCC
        outputFile.write((const char *)&ddsHeader->ddspf.dwRGBBitCount, 4); // ddspf.dwRGBBitCount
        outputFile.write((const char *)&ddsHeader->ddspf.dwRBitMask, 4);    // ddspf.dwRBitMask
        outputFile.write((const char *)&ddsHeader->ddspf.dwGBitMask, 4);    // ddspf.dwGBitMask
        outputFile.write((const char *)&ddsHeader->ddspf.dwBBitMask, 4);    // ddspf.dwBBitMask
        outputFile.write((const char *)&ddsHeader->ddspf.dwABitMask, 4);    // ddspf.dwABitMask

        outputFile.write((const char *)&ddsHeader->dwCaps, 4);      // dwCaps
        outputFile.write((const char *)&ddsHeader->dwCaps2, 4);     // dwCaps2
        outputFile.write((const char *)&ddsHeader->dwDepth, 4);     // dwDepth
        outputFile.write((const char *)&ddsHeader->dwCaps3, 4);     // dwCaps3
        outputFile.write((const char *)&ddsHeader->dwReserved2, 4); // dwReserved2
        
        unsigned int arraySize = ddsHeader->dwPitchOrLinearSize;
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



uint8_t* Converter::BMPUncompressedImageData(int imageSize) const
{
    if (pixels == nullptr)
        return nullptr;

    //Copy the image data to avoid awkward accidental deleting
    uint8_t* pCopy = new uint8_t[imageSize];
    memcpy(pCopy, pixels, imageSize);

    return pCopy;
}


uint8_t* Converter::DDSUncompressedImageData(int dwWidth, int dwHeight) const
{
    if (dataBuffer == nullptr)
        return nullptr;

    //const unsigned int width = dwWidth;
    //const unsigned int height = dwHeight;

    //Allocate memory for uncompressed image data, width * height * 3 color bytes per pixel
    //uint8_t* bytesBuffer = new uint8_t[width * height * 3];
    uint8_t* bytesBuffer = new uint8_t[dwWidth * dwHeight * 3];

    return bytesBuffer;
}