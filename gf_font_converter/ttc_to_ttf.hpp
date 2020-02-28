
#ifndef ttc_to_ttf_hpp
#define ttc_to_ttf_hpp

#include <stdint.h>

uint32_t CalcTableChecksum(uint32_t* table, uint32_t length);
int32_t ttcTottfConvert(std::string sourceFile, std::string sourcePath, std::string destinationPath, int index, std::vector<std::string> &fontsList);

#endif /* ttc_to_ttf_hpp */
