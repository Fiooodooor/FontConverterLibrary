
//  Created by CzornyCzfanyCzop on 2/28/20.
//  Copyright © 2020 CzornyCzfanyCzop. All rights reserved.
//

#ifndef GF_FONTCONVERTER_PUBLIC
#define GF_FONTCONVERTER_PUBLIC

/* The classes below are exported */
#pragma GCC visibility push(default)
#define LIB_MAXPATH_SIZE 260

typedef struct FontsPathS {
    char fontPath[LIB_MAXPATH_SIZE];
} FontsPathS;

typedef struct FontsListS {
    int32_t structureSize;
    FontsPathS *pathsTable;
} FontsListS;

#ifdef __cplusplus
extern "C" { 
#endif
    #ifdef AE_OS_WIN
    __declspec(dllexport) int __cdecl copyConvertFont(const char* sourceFile, const char* sourcePath, const char* destinationPath, int index, FontsListS *fontsList, const char* libraryPath);
    #else
    int __cdecl copyConvertFont(const char* sourceFile, const char* sourcePath, const char* destinationPath, int index, FontsListS *fontsList, const char* libraryPath);
    #endif
#ifdef __cplusplus
}
#endif
typedef int(__cdecl *copyConvertFont_creator)(const char* sourceFile, const char* sourcePath, const char* destinationPath, int index, FontsListS *fontsList, const char* libraryPath);

#pragma GCC visibility pop
#endif // GF_FONTCONVERTER_PUBLIC
