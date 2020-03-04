
#include <stdio.h>
#include <stdlib.h>
#include <fontforge/fontforge.h>
#include "ttf_to_ttf.h"


#if defined(_WIN32) || defined(_WIN64)

#else
    #define LIBRARY_FILE_NAME "libfontforge.4.dylib"
    #include <dlfcn.h>
#endif

char** get_font_names(const char* src_file)
{
    void* fontforge_handle = dlopen(LIBRARY_FILE_NAME, RTLD_LOCAL|RTLD_LAZY);
    if (!fontforge_handle)
        return NULL;
    
    char** (*GetFontNames)(const char*) = dlsym(fontforge_handle, "GetFontNames");
    
    if(!GetFontNames)
        return NULL;
    
    char* src_file_copy = strdup(src_file);
    char** font_names = GetFontNames(src_file_copy);
    free(src_file_copy);
    
    dlclose(fontforge_handle);
    return font_names;
}

int convert_font(const char* src_file, char* output_file)
{
    void* fontforge_handle = dlopen(LIBRARY_FILE_NAME, RTLD_LOCAL|RTLD_LAZY);
    if (!fontforge_handle)
    {
        return -2;
    }
    
    int (*doinitFontForgeMain)(void) = dlsym(fontforge_handle, "doinitFontForgeMain");
    SplineFont *(*LoadSplineFont)(const char *,enum openflags) = dlsym(fontforge_handle, "LoadSplineFont");
    int (*GenerateScript)(SplineFont *,char *,const char *, int, int, char *, struct sflist *,EncMap *, NameList *,int) = dlsym(fontforge_handle, "GenerateScript");
     void (*SFFlatten)(SplineFont **cidmaster) = dlsym(fontforge_handle, "SFFlatten");
     EncMap *(*EncMapCopy)(EncMap *map) = dlsym(fontforge_handle, "EncMapCopy");
    
    if(!doinitFontForgeMain || !LoadSplineFont || !GenerateScript)
    {
        if(dlclose(fontforge_handle) != 0)
            return -3;
        return -4;
    }
    
    SplineFont* font = NULL;
    doinitFontForgeMain();
    font = (SplineFont*)LoadSplineFont(src_file, 1);
       
    if (font != NULL)
    {
        if(font->subfontcnt > 0)
        {
            if(!SFFlatten || !EncMapCopy)
            {
                if(dlclose(fontforge_handle))
                    return -3;
                return -6;
            }
            EncMap *tmpMapCpy = EncMapCopy(font->map);
            SFFlatten(&font);
            if(font->map == NULL)
                font->map = tmpMapCpy;
            else
                free(tmpMapCpy);
        }
        int ret = GenerateScript(font, output_file, NULL, 0, -1, NULL, NULL, font->map, NULL, 1);
        dlclose(fontforge_handle);
        if (ret)
            return 0;
    }
    if(dlclose(fontforge_handle) != 0)
        return -3;
    return -1;
}


