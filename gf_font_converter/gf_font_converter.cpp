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

#include "ttc_to_ttf.hpp"

#ifdef __cplusplus
extern "C" {
#endif
    #include "ttf_to_ttf.h"
#ifdef __cplusplus
}
#endif

#define EXPORT __attribute__((visibility("default")))

int gf_font_converter::convert(std::string sourceFile, std::string sourcePath, std::string destinationPath, int index, std::vector<std::string> &fontsList)
{
    int fontsConverted = 0;
    try {
        if (sourceFile.length() < 4)
            return -1;
        if(destinationPath.length() < 3)
            return -2;
         
        char tmp[5];
        strcpy(tmp, &sourceFile.c_str()[sourceFile.length() - 4]);
        for (long i = 0; i < 4; i++)
            tmp[i] = toupper(tmp[i]);
        
        if (strcmp(tmp, ".TTC") == 0)
        {
            fontsConverted = ttcTottfConvert(sourceFile, sourcePath, destinationPath, index, fontsList);
        }
        else
        {
            char tempSourceFile[260], tempDestFile[260];
            sprintf(tempSourceFile, "%s/%s", sourcePath.c_str(), sourceFile.c_str());
            sprintf(tempDestFile, "%s/%s", destinationPath.c_str(), sourceFile.c_str());
            fontsConverted = convert_font(tempSourceFile, tempDestFile);
            if(fontsConverted > 0)
            {
                fontsList.push_back(tempDestFile);
            }
        }
    }
    catch(...) {
        /* error handling */
        return -3;
    }
    return fontsConverted;
}

EXPORT
int copyConvertFont(std::string sourceFile, std::string sourcePath, std::string destinationPath, int index, std::vector<std::string> &fontsList)
{
    return gf_font_converter::convert(sourceFile, sourcePath, destinationPath, index, fontsList);
}
