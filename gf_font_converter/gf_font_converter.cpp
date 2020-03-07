//
//  gf_font_converter.cpp
//  gf_font_converter
//
//  Created by Milosz Linkiewicz on 2/28/20.
//  Copyright © 2020 GarageFarm.net. All rights reserved.
//

#include <string>
#include <vector>
#include "gf_font_converter.hpp"
#include "gf_font_converterPriv.hpp"

extern "C" {
	#include "gf_font_generate.h"
}

#if defined(_WIN32) || defined(_WIN64)
	#define FOPEN(FILEPTR,FILEPATH,FILEMODE) fopen_s(& FILEPTR, FILEPATH, FILEMODE)
	#define EXPORT	
	#define MAX_PATH 260
#else
	#define FOPEN(FILEPTR,FILEPATH,FILEMODE) FILEPTR = fopen(FILEPATH, FILEMODE)
	#define EXPORT __attribute__((visibility("default")))
	#define MAX_PATH 260
#endif
//#define EXPORT __attribute__((visibility("default")))

int gf_font_converter::convert(std::string sourceFile, std::string sourcePath, std::string destinationPath, int index, std::vector<std::string> &fontsList)
{
    int fontsConverted = 0;
	char fontExtension[4];
    try {
        if(sourceFile.length() < 5)
            return -1;
        if(destinationPath.length() < 5)
            return -2;        
        
		libraryInitialize();

        strcpy(fontExtension, &sourceFile.c_str()[sourceFile.length() - 4]);
        for (long i = 0; i < 4; i++)
			fontExtension[i] = toupper(fontExtension[i]);
        
        if (strcmp(fontExtension, ".TTC") == 0 || strcmp(fontExtension, ".OTC") == 0)
            fontsConverted = gf_font_converter::ttcToTtfConvert(sourceFile, sourcePath, destinationPath, index, fontsList);
        else
        {			
			sourcePath += ('/' + sourceFile);
			destinationPath += ('/' + std::to_string(index) + '_' + sourceFile);

            fontsConverted = convert_font(sourcePath.c_str(), destinationPath.c_str());
            if(fontsConverted > 0)
                fontsList.push_back(destinationPath.c_str());
        }
    }
    catch(...) {
        /* error handling */
        return -3;
    }
    return fontsConverted;
}

int32_t gf_font_converter::ttcToTtfConvert(std::string sourceFile, std::string sourcePath, std::string destinationPath, int index, std::vector<std::string> &fontsList)
{
	char* ttcBuffer = NULL;
	uint32_t ttcSize = 0, ttcConverted = 0, ttcResult;
	FILE* ttcFilePtr = NULL;
	std::string source = sourcePath + '/' + sourceFile;

	FOPEN(ttcFilePtr, source.c_str(), "rt");

	if (ttcFilePtr == NULL)
		return -2;

	ttcBuffer = (char*)malloc(100 * sizeof(char));
	fread(ttcBuffer, sizeof(char), 99, ttcFilePtr);
	fclose(ttcFilePtr);

	if (*(int32_t*)ttcBuffer != *(int32_t*)"ttcf")
		return -1;
	else
	{
		ttcSize = *(uint32_t*)&ttcBuffer[0x08];
		unsigned char *memp = reinterpret_cast<unsigned char*>(ttcSize);
		std::reverse(memp, memp + sizeof(uint32_t));

		char subFontSource[MAX_PATH], subFontDestin[MAX_PATH];
		for (uint32_t i = 0; i < ttcSize; i++)
		{
			sprintf_s(subFontSource, MAX_PATH, "%s(%d)", source.c_str(), i);
			sprintf_s(subFontDestin, MAX_PATH, "%s/%d_%d_%s.otf", destinationPath.c_str(), index, i, sourceFile.c_str());
			ttcResult = convert_font(subFontSource, subFontDestin);
			if (ttcResult > 0) {
				++ttcConverted;
				fontsList.push_back(subFontDestin);
			}
		}
	}

	free(ttcBuffer);
	return ttcConverted;
}

EXPORT
int copyConvertFont(std::string sourceFile, std::string sourcePath, std::string destinationPath, int index, std::vector<std::string> &fontsList)
{
    return gf_font_converter::convert(sourceFile, sourcePath, destinationPath, index, fontsList);
}

// add handling of font uninitialize 

