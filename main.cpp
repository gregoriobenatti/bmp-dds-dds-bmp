
#include <iostream>
//#include <BMPFile.h>
//#include <DDSFile.h>

void help()
{
    std::cout << "\n\n--- BMP-DDS-DDS-BMP ---\n" << std::endl;
    std::cout << "Arguments --> INPUT.FILE and OUTPUT.FILE or only INPUT.FILE \n"
            << "Support to FILE.NAME or /PATH/TO/FILE.NAME \n"
            << "./converter input.file output.file \n"
            << "OR \n"
            << "./converter input.file \n"
            << std::endl;

}

void parseInputStrign() {  }

void checkInputFile() {  }


int main(int argc, char* argv[])
{
	try 
	{
	    // parse arguments to get only file name.
	    // parse extension to check conversion.
	    // validate image size, w and h multiple of 4
	    // check if arg1 and arg2 are differente
	    // posibility to use only arg1 to input and output using the same name in same directory

	    std::string arg1 = argv[1];
		if (arg1 == "help" || arg1 == "man")
		    help();
		else
		    std::cerr << "Converting " << argv[1] << " to " << argv[2] << std::endl;

	}
	catch (...) 
	{
		std::cout << "Something went horribly wrong!" << std::endl;
		return 1;
	}

	return 0;
}
