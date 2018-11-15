//
// Created by Gregorio Benatti on 06/11/2018.
//

#include "BMPFile.h"
#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <vector>
#include <iterator>


BMPFile::BMPFile()
{
    bmpHeader = nullptr;
    bmpInfoHeader = nullptr;
}


BMPFile::~BMPFile()
{
    delete[] bmpHeader;
    delete[] bmpInfoHeader;
}


BMPSTRUCT BMPFile::BMPInit(std::string fileName)
{
    BMPSTRUCT bmp;

    std::cout << "[BMPFile] BMPInit" << std::endl;

    uint8_t* dataBuffer[2] = { nullptr, nullptr };
    bmpHeader = nullptr;
    bmpInfoHeader = nullptr;

    std::ifstream file(fileName, std::ios::binary);
    if (!file)
    {
        std::cout << "No file found..." << std::endl;
    }

    dataBuffer[0] = new uint8_t[sizeof(BITMAPFILEHEADER)];
    dataBuffer[1] = new uint8_t[sizeof(BITMAPINFOHEADER)];

    static constexpr size_t HEADER_SIZE = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    std::array<char, HEADER_SIZE> header;
    file.read(header.data(), header.size());

    bmpHeader = (BITMAPFILEHEADER*)dataBuffer[0];
    bmpHeader->bfSize      = *reinterpret_cast<uint32_t *>(&header[0]);  // The header field used to identify the BMP and DIB file is 0x42 0x4D in hexadecimal, same as BM in ASCII (BM, BA, CI, CP, IC, PT)
    bmpHeader->bfType      = *reinterpret_cast<uint32_t *>(&header[2]);  // The size of the BMP file in bytes
    bmpHeader->bfReserved1 = *reinterpret_cast<uint32_t *>(&header[6]);  // Reserved; actual value depends on the application that creates the image
    bmpHeader->bfReserved2 = *reinterpret_cast<uint32_t *>(&header[8]);  // Reserved; actual value depends on the application that creates the image
    bmpHeader->bfOffBits   = *reinterpret_cast<uint32_t *>(&header[10]); // The offset, i.e. starting address, of the byte where the bitmap image data (pixel array) can be found.

    bmpInfoHeader = (BITMAPINFOHEADER*)dataBuffer[1];
    bmpInfoHeader->biSize          = *reinterpret_cast<uint32_t *>(&header[14]); // the size of this header (40 bytes)
    bmpInfoHeader->biWidth         = *reinterpret_cast<uint32_t *>(&header[18]); // the bitmap width in pixels (signed integer)
    bmpInfoHeader->biHeight        = *reinterpret_cast<uint32_t *>(&header[22]); // the bitmap height in pixels (signed integer)
    bmpInfoHeader->biPlanes        = *reinterpret_cast<uint32_t *>(&header[26]); // the number of color planes (must be 1)
    bmpInfoHeader->biBitCount      = *reinterpret_cast<uint32_t *>(&header[28]); // the number of bits per pixel, which is the color depth of the image. Typical values are 1, 4, 8, 16, 24 and 32.
    bmpInfoHeader->biCompression   = *reinterpret_cast<uint32_t *>(&header[30]); // the compression method being used. See the next table for a list of possible values
    bmpInfoHeader->biSizeImage     = *reinterpret_cast<uint32_t *>(&header[34]); // the image size. This is the size of the raw bitmap data; a dummy 0 can be given for BI_RGB bitmaps.
    bmpInfoHeader->biXPelsPerMeter = *reinterpret_cast<uint32_t *>(&header[38]); // the horizontal resolution of the image. (pixel per metre, signed integer)
    bmpInfoHeader->biYPelsPerMeter = *reinterpret_cast<uint32_t *>(&header[42]); // the vertical resolution of the image. (pixel per metre, signed integer)
    bmpInfoHeader->biClrUsed       = *reinterpret_cast<uint32_t *>(&header[46]); // the number of colors in the color palette, or 0 to default to 2n
    bmpInfoHeader->biClrImportant  = *reinterpret_cast<uint32_t *>(&header[50]); // the number of important colors used, or 0 when every color is important; generally ignored

    bmp.fileHeader = bmpHeader;
    bmp.infoHeader = bmpInfoHeader;

    return bmp;
}

