//
//  gf_font_converter.hpp
//  gf_font_converter
//
//  Created by CzornyCzfanyCzop on 2/28/20.
//  Copyright © 2020 CzornyCzfanyCzop. All rights reserved.
//

#ifndef gf_font_converter_
#define gf_font_converter_

/* The classes below are exported */
#pragma GCC visibility push(default)

class gf_font_converter
{
    public:
    static int convert(std::string sourceFile, std::string sourcePath, std::string destinationPath, int index, std::vector<std::string> &fontsList);
};

#pragma GCC visibility pop
#endif
