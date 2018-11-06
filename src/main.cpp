
#include <iostream>
#include "BMPFile.h"
#include "DDSFile.h"


const std::string BMP  = "bmp";
const std::string DDS  = "dds";
const std::string HELP = "help";
const std::string MAN  = "man";


void help()
{
    std::cout << "\n\n--- BMP-DDS-DDS-BMP ---\n" << std::endl;
    std::cout << "Arguments --> INPUT.FILE and OUTPUT.FILE or only INPUT.FILE \n"
              << "Support to FILE.NAME or /PATH/TO/FILE.NAME \n"
              << "./converter input.file output.file \n"
              << "OR \n"
              << "./converter input.file \n\n\n"
              << std::endl;

}


std::string toLowerCase(std::string s)
{
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}


std::string getFileExtension(std::string inputFile)
{
    std::string fileExtension = inputFile.substr(inputFile.find_last_of(".") + 1);
    std::cout << "Input file type: " << fileExtension << std::endl;
    return toLowerCase(fileExtension);
}


void BMPFileInit() { std::cout << "[Converting] to BMP." <<std::endl; }
void DDSFileInit() { std::cout << "[Converting] to DDS." <<std::endl; }


int main(int argc, char* argv[])
{
    try
    {
        // Initial version
        // >> just accept 1 argument, input.file
        // >> Check size multiple by 4
        // >> Check extension if is valid.
        // >> Based on extension, convert to the other, eg. input.BMP > input.DDS or input.DDS > input.BMP
        // TODO: Better string output, step by step
        // TODO: Update the HELPER function
        // TODO: Create a Utils.h file for string helper functions?!

        std::string arg1 = toLowerCase(argv[1]);
        if (arg1 == HELP || arg1 == MAN)
        {
            help();
        }
        else
        {
            std::string fileExtension = getFileExtension(argv[1]);

            if ((fileExtension != BMP) && (fileExtension != DDS))
            {
                std::cout << "Error! File not supported --> " << argv[1] << std::endl;
                return 1;
            }

            std::cout << "[Start] Convert from BMP to DDS" <<std::endl;
            std::cout << "[Init] Check image size" <<std::endl;

            (fileExtension == BMP) ? BMPFileInit() : DDSFileInit();

            std::cout << "[Finish] Done." <<std::endl;
        }
    }
    catch (...)
    {
        std::cout << "Error! " << std::endl;
        return 1;
    }

    return 0;
}
