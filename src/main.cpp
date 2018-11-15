
#include "BMPFile.h"
#include "DDSFile.h"
#include "Converter.h"
#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <iterator>
#include <sstream>
#include <fstream>


const std::string BMP  = "bmp";
const std::string DDS  = "dds";
const std::string HELP = "help";
const std::string MAN  = "man";


void help()
{
    std::cout << "\n\n--- BMP-DDS-DDS-BMP ---\n" << std::endl;
    std::cout << "Arguments --> INPUT.FILE\n"
              << "Need to be a file in the same directory as the executable\n"
              << "./converter input.file\n\n\n"
              << std::endl;
}


std::string toLowerCase(std::string s)
{
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}


std::string getFileExtension(std::string inputFile)
{
    std::string fileExtension = toLowerCase(inputFile.substr(inputFile.find_last_of(".") + 1));
    std::cout << "\nInput file type: " << fileExtension << std::endl;
    std::cout << "Convert to: " << ((fileExtension == BMP) ? DDS : BMP) << std::endl;
    return fileExtension;
}


void convertTo(std::string fileExtension, std::string fileName)
{
    std::cout << "[Converting] Geting file information." <<std::endl;
    std::cout << "[Converting] Start convert file." <<std::endl;

    std::shared_ptr<BMPFile> pBMPFile(new BMPFile);
    std::shared_ptr<DDSFile> pDDSFile(new DDSFile);
    std::shared_ptr<Converter> converter(new Converter);

    if (fileExtension == BMP)
    {
        BMPSTRUCT bmp = pBMPFile->BMPInit(fileName);
        converter->convertBMPToDDS(bmp.fileHeader, bmp.infoHeader, bmp.pixels);
    }
    else if (fileExtension == DDS)
    {
        DDSSTRUCT dds = pDDSFile->DDSInit(fileName);
        converter->convertDDSToBMP(dds.header, dds.dataBuffer);
    }
    else
    {
        std::cout << "File not supported." << std::endl;
    }

    std::cout << "[Converting] Finish convert file." <<std::endl;
}


int main(int argc, char* argv[])
{
    try
    {
        std::string arg1 = toLowerCase(argv[1]);
        if (arg1 == HELP || arg1 == MAN)
        {
            help();
        }
        else
        {
            std::string fileExtension = getFileExtension(argv[1]);
            std::cout << "\n[Start] bmp-dds-dds-bmp." <<std::endl;

            if ((fileExtension != BMP) && (fileExtension != DDS))
            {
                std::cout << "Error! File not supported --> " << argv[1] << std::endl;
                return 1;
            }

            convertTo(fileExtension, argv[1]);

            std::cout << "[Finish] bmp-dds-dds-bmp." <<std::endl;
        }
    }
    catch (...)
    {
        std::cout << "Error! " << std::endl;
        return 1;
    }

    return 0;
}
