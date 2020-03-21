//
//  gf_font_converterPriv.hppß
//  gf_font_converter
//
//  Created by CzornyCzfanyCzop on 2/28/20.
//  Copyright © 2020 CzornyCzfanyCzop. All rights reserved.
//

/* The classes below are not exported */
#ifndef GF_FONTCONVERTER_PRIVATE
#define GF_FONTCONVERTER_PRIVATE
#pragma GCC visibility push(hidden)

static int font_convert_start(const char* sourceFile, const char* sourcePath, const char* destinationPath, int index, FontsListS *fontsList, const char* libraryPath);

class gf_font_converter
{
public:
	static int convert(std::string &sourceFile, std::string &sourcePath, std::string &destinationPath, int index, FontsListS *fontsList, std::string &libraryPath);
	static int32_t ttcToTtfConvert(std::string &sourceFile, std::string &sourcePath, std::string &destinationPath, int index, FontsListS *fontsList, std::string &libraryPath);
};

#pragma GCC visibility pop
#endif
