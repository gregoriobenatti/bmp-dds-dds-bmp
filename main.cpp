
#include <iostream>


int main(int argc, char* argv[])
{
	try 
	{
		std::cout << "--- BMP-DDS-DDS-BMP ---" << std::endl;
		std::cout << "EXECUTE: ./converter input.file outuput.file" << std::endl;
		std::cerr << "1st: " << argv[1] << " 2nd: " << argv[2] << std::endl;

	}
	catch (...) 
	{
		std::cout << "Something went horribly wrong!" << std::endl;
		return 1;
	}

	return 0;
}
