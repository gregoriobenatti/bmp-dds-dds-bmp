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


DDSSTRUCT DDSFile::DDSInit(std::string fileName)
{
    DDSSTRUCT dds;

    dataBuffer = nullptr;
    ddsHeader = nullptr;

    std::ifstream file(fileName, std::ios::binary);
    if (!file)
    {
        std::cout << "No file found..." << std::endl;
    }

    dataBuffer = new uint8_t[sizeof(DDS_HEADER)];
    file.read((char*)dataBuffer, sizeof(DDS_HEADER));
    ddsHeader = (DDS_HEADER*)dataBuffer;

    unsigned int mainImageSize = std::max((unsigned int)1, (unsigned int)(ddsHeader->dwWidth + 3) / 4) * std::max((unsigned int)1, (unsigned int)(ddsHeader->dwHeight + 3) / 4) * 8;
    if (mainImageSize != ddsHeader->dwPitchOrLinearSize)
    {
        std::cout << "Something is weird, check DDS file." << std::endl;
    }
    dataBuffer = new uint8_t[mainImageSize];
    dds.header = ddsHeader;
    dds.dataBuffer = new uint8_t[mainImageSize]; //DX1Compress probably here

    return dds;
}
