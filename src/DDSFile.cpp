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


DDS_HEADER DDSFile::DDSInit(std::string fileName)
{
    std::cout << "[DDSFile] DDSInit" << std::endl;
    DDS_HEADER dds;

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

    return dds;
}
