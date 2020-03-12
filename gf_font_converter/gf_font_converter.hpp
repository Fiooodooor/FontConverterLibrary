//
//  gf_font_converter.hpp
//  gf_font_converter
//
//  Created by CzornyCzfanyCzop on 2/28/20.
//  Copyright © 2020 CzornyCzfanyCzop. All rights reserved.
//

#ifndef GF_FONTCONVERTER_PUBLIC
#define GF_FONTCONVERTER_PUBLIC

/* The classes below are exported */
//#pragma GCC visibility push(default)


extern "C" int copyConvertFont(std::string sourceFile, std::string sourcePath, std::string destinationPath, int index, std::vector<std::string> &fontsList, std::string libraryPath);
typedef int copyConvertFont_creator(std::string sourceFile, std::string sourcePath, std::string destinationPath, int index, std::vector<std::string> &fontsList, std::string libraryPath);

//#pragma GCC visibility pop
#endif // GF_FONTCONVERTER_PUBLIC
