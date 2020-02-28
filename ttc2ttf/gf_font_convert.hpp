//
//  gf_font_convert.hpp
//  ttc2ttf
//
//  Created by CzornyCzfanyCzop on 2/27/20.
//  Copyright © 2020 CzornyCzfanyCzop. All rights reserved.
//

#ifndef gf_font_convert_hpp
#define gf_font_convert_hpp
#include <string>
#include <vector>

class gf_font_converter
{
public:
    static int convert(std::string sourceFile, std::string sourcePath, std::string destinationPath, int index, std::vector<std::string> &fontsList);
    
private:
};

#endif /* gf_font_convert_hpp */
