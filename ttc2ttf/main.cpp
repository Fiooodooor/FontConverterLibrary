
#include <fstream>
#include "gf_font_convert.hpp"

int main(int argc, char* argv[])
{
    std::string sourceFile = "Helvetica.ttc";
    std::string sourcePath = "/Users/ccc/Documents/ttc2ttf";
    std::string destinationPath = "/Users/ccc/Documents";
    std::vector<std::string> fontsList;
    
    int result = gf_font_converter::convert(sourceFile, sourcePath, destinationPath, 3, fontsList);
    
    for(auto strit : fontsList)
    {
        printf("\nconverted %s ", strit.c_str());
    }
    printf("\n total: %d", result);
    
    return 0;
}

