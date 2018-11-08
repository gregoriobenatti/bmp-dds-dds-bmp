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
    pixels = nullptr;
}


BMPFile::~BMPFile()
{
    delete[] bmpHeader;
    delete[] bmpInfoHeader;

    if (pixels != nullptr)
    {
        delete[] pixels;
    }
}


void BMPFile::BMPInit(std::string fileName)
{
    std::cout << "[BMPFile] BMPInit" << std::endl;

    uint8_t* dataBuffer[2] = { nullptr, nullptr };
    pixels = nullptr;
    bmpHeader = nullptr;
    bmpInfoHeader = nullptr;

    std::ifstream file(fileName, std::ios::binary);
    if (!file)
    {
        std::cout << "No file found..." << std::endl;
    }

    dataBuffer[0] = new uint8_t[sizeof(BITMAPFILEHEADER)];
    dataBuffer[1] = new uint8_t[sizeof(BITMAPINFOHEADER)];
    bmpHeader = (BITMAPFILEHEADER*)dataBuffer[0];
    bmpInfoHeader = (BITMAPINFOHEADER*)dataBuffer[1];

    static constexpr size_t HEADER_SIZE = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    std::array<char, HEADER_SIZE> header;
    file.read(header.data(), header.size());

    bmpHeader->bfSize      = *reinterpret_cast<uint32_t *>(&header[0]);  // The header field used to identify the BMP and DIB file is 0x42 0x4D in hexadecimal, same as BM in ASCII (BM, BA, CI, CP, IC, PT)
    bmpHeader->bfType      = *reinterpret_cast<uint32_t *>(&header[2]);  // The size of the BMP file in bytes
    bmpHeader->bfReserved1 = *reinterpret_cast<uint32_t *>(&header[6]);  // Reserved; actual value depends on the application that creates the image
    bmpHeader->bfReserved2 = *reinterpret_cast<uint32_t *>(&header[8]);  // Reserved; actual value depends on the application that creates the image
    bmpHeader->bfOffBits   = *reinterpret_cast<uint32_t *>(&header[10]); // The offset, i.e. starting address, of the byte where the bitmap image data (pixel array) can be found.

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
}


void BMPFile::writeFile(BITMAPFILEHEADER* fileHeader, BITMAPINFOHEADER* infoHeader)
{
    std::string fileName = "output.bmp";
    std::ofstream outputFile;
    outputFile.open(fileName, std::ofstream::out | std::ofstream::binary | std::ofstream::app);

    if (!isFileSizeValid(infoHeader->biWidth, infoHeader->biHeight))
    {
        std::cout << "File size not valid" << std::endl;
    }

    if (outputFile.is_open())
    {
        outputFile.write((const char *)&fileHeader->bfType, 2);
        outputFile.write((const char *)&fileHeader->bfSize, 4);
        outputFile.write((const char *)&fileHeader->bfReserved1, 2);
        outputFile.write((const char *)&fileHeader->bfReserved2, 2);
        outputFile.write((const char *)&fileHeader->bfOffBits, 4);

        outputFile.write((const char *)&infoHeader->biSize, 4);
        outputFile.write((const char *)&infoHeader->biWidth, 4);
        outputFile.write((const char *)&infoHeader->biHeight, 4);
        outputFile.write((const char *)&infoHeader->biPlanes, 2);
        outputFile.write((const char *)&infoHeader->biBitCount, 2);
        outputFile.write((const char *)&infoHeader->biCompression, 4);
        outputFile.write((const char *)&infoHeader->biSizeImage, 4);
        outputFile.write((const char *)&infoHeader->biXPelsPerMeter, 4);
        outputFile.write((const char *)&infoHeader->biYPelsPerMeter, 4);
        outputFile.write((const char *)&infoHeader->biClrUsed, 4);
        outputFile.write((const char *)&infoHeader->biClrImportant, 4);

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
}


void BMPFile::createBMPFile()
{
    writeFile(bmpHeader, bmpInfoHeader);
}


bool BMPFile::isFileSizeValid(int width, int height)
{
    return ((width%4 == 0) && (height%4 == 0)) ? true : false;
}