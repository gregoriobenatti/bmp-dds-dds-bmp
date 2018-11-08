//
// Created by Gregorio Benatti on 06/11/2018.
//

#include "DDSFile.h"
#include <math.h>
#include <string>
#include <array>
#include <iterator>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <vector>
#include <deque>


DDSFile::DDSFile()
{
    ddsHeader = nullptr;
    dataBuffer = nullptr;
}

DDSFile::~DDSFile()
{
    delete[] ddsHeader;
    delete[] dataBuffer;
}


void DDSFile::DDSInit(std::string fileName)
{
    std::cout << "[DDSFile] DDSInit" << std::endl;

    dataBuffer = nullptr;
    ddsHeader = nullptr;

    std::ifstream file(fileName, std::ios::binary);
    if (!file)
    {
        std::cout << "No file found..." << std::endl;
    }

    dataBuffer = new uint8_t[sizeof(DDS_HEADER)];
    file.read((char*)dataBuffer, sizeof(DDS_HEADER));

    // Construct the values from the buffers
    ddsHeader = (DDS_HEADER*)dataBuffer;

    unsigned int mainImageSize = std::max(1, (int)(ddsHeader->dwWidth + 3) / 4) * std::max(1, (int)(ddsHeader->dwHeight + 3) / 4) * 8;
    dataBuffer = new uint8_t[mainImageSize];
    file.read((char*)dataBuffer, mainImageSize);

    std::cout << "[DDSFile] ." << std::endl;
}


void DDSFile::createDDSFile(unsigned long height, unsigned long width)
{
    if (!isFileSizeValid(width, height))
    {
        std::cout << "File size not valid" << std::endl;
    }

    dataBuffer = new uint8_t[sizeof(DDS_HEADER)];
    ddsHeader = (DDS_HEADER*)dataBuffer;

    DDS_PIXELFORMAT pixelFormat;
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

    writeFile(ddsHeader);
}


void DDSFile::writeFile(DDS_HEADER* header)
{
    std::string fileName = "output.dds";
    std::ofstream outputFile;
    outputFile.open(fileName, std::ofstream::out | std::ofstream::binary | std::ofstream::app);

    if (outputFile.is_open())
    {
        outputFile.write((const char *)"DDS ", 4);
        outputFile.write((const char *)&header->dwSize, 4);
        outputFile.write((const char *)&header->dwFlags, 4);
        outputFile.write((const char *)&header->dwHeight, 4);
        outputFile.write((const char *)&header->dwWidth, 4);
        outputFile.write((const char *)&header->dwPitchOrLinearSize, 4);
        outputFile.write((const char *)&header->dwDepth, 4);
        outputFile.write((const char *)&header->dwMipMapCount, 4);
        outputFile.write((const char *)&header->dwReserved1, 44);

        outputFile.write((const char *)&header->ddspf.dwSize, 4);
        outputFile.write((const char *)&header->ddspf.dwFlags, 4);
        outputFile.write((const char *)&header->ddspf.dwFourCC, 4);
        outputFile.write((const char *)&header->ddspf.dwRGBBitCount, 4);
        outputFile.write((const char *)&header->ddspf.dwRBitMask, 4);
        outputFile.write((const char *)&header->ddspf.dwGBitMask, 4);
        outputFile.write((const char *)&header->ddspf.dwBBitMask, 4);
        outputFile.write((const char *)&header->ddspf.dwABitMask, 4);

        outputFile.write((const char *)&header->dwCaps, 4);
        outputFile.write((const char *)&header->dwCaps2, 4);
        outputFile.write((const char *)&header->dwDepth, 4);
        outputFile.write((const char *)&header->dwCaps3, 4);
        outputFile.write((const char *)&header->dwReserved2, 4);

        unsigned int arraySize = header->dwPitchOrLinearSize;
        for (unsigned int i = 0; i < arraySize; ++i) {
            outputFile.write((const char *)&dataBuffer[i], 1);
        }
    }

    outputFile.close();
}


bool DDSFile::isFileSizeValid(int width, int height)
{
    return ((width%4 == 0) && (height%4 == 0)) ? true : false;
}