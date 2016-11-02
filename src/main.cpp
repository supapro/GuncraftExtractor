#include <cstdlib>
#include <algorithm>
#include "../include/converters.h"

#if defined(_WIN32)
#include <windows.h>
#undef min
#undef max
#endif



void setConsoleTitle(const char *title)
{
#if defined(_WIN32)
	SetConsoleTitleA(title);
#else
	// set title terminal command.
	printf("%c[0;%s%c", '\033', title, '\007');
#endif
}

int main(int argc, char *argv[]) 
{
	setConsoleTitle("GunCraft Extractor - v1.1");

	if (argc < 2) {
		std::cout << "You need to start this program by dropping a file(s) on the executable." << std::endl;
		std::cin.ignore();
		exit(0);
	}

	std::string fileName;
	std::string ext;
	for (int i = 1; i < argc; ++i) {
		fileName = argv[i];
		ext = fileName.substr(fileName.length() - 4, 4);

		if (ext.compare(".xnb") == 0) {
			std::cout << "Converting xnb file " << argv[i] << std::endl;
			convertXnb(fileName);
		} 
		else if (ext.compare(".png") == 0) {
			std::cout << "Converting png file " << argv[i] << " to xnb" << std::endl;
			PngToXnb(fileName);
		}
		else if (ext.compare(".wav") == 0) {
			std::cout << "Converting wav file " << argv[i] << " to xnb" << std::endl;
			WavToXnb(fileName);
		}
		else {
			std::cout << "Unsupported file " << fileName << std::endl;
		}

		std::cout << std::endl;
	}

	std::cin.ignore();
	
	return 0;
}
