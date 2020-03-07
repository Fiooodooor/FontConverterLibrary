//#include <stdio.h>
#include <stdlib.h>
#include <fontforge/fontforge.h>
#include "gf_font_generate.h"

static EncMap *createMapCopy(EncMap *map);

typedef struct externPtrs {
	void* ffH;
	int(*doinitFontForgeMain)(void);
	SplineFont *(*LoadSplineFont)(const char *, enum openflags);
	int(*GenerateScript)(SplineFont *, char *, const char *, int, int, char *, struct sflist *, EncMap *, NameList *, int);
	void(*SFFlatten)(SplineFont **cidmaster);
	char** (*GetFontNames)(const char*);
	int isInitialized;
};

#if defined(_WIN32) || defined(_WIN64)
	extern int			doinitFontForgeMain(void);
	extern SplineFont  *LoadSplineFont(const char *, enum openflags);
	extern int			GenerateScript(SplineFont *, char *, const char *, int, int, char *, struct sflist *, EncMap *, NameList *, int);
	extern void			SFFlatten(SplineFont **cidmaster);
	extern char**		GetFontNames(const char*);
#else
    #define LIBRARY_FILE_NAME "libfontforge.4.dylib"
    #include <dlfcn.h>
#endif

static struct externPtrs libH = { NULL, NULL, NULL, NULL, NULL, NULL, 0 };

void libraryInitialize()
{
	if (libH.isInitialized <= 0)
	{
		libH.isInitialized = -1;

#if defined(_WIN32) || defined(_WIN64)
		libH.doinitFontForgeMain = &doinitFontForgeMain;
		libH.LoadSplineFont		 = &LoadSplineFont;
		libH.GenerateScript		 = &GenerateScript;
		libH.SFFlatten			 = &SFFlatten;
		libH.GetFontNames		 = &GetFontNames;
		libH.isInitialized = 7; // (4 + 2 + 1)
#else
		libH.ffH = dlopen(LIBRARY_FILE_NAME, RTLD_LOCAL | RTLD_LAZY);
		if (!libH.ffH)
			return;
		libH.doinitFontForgeMain = dlsym(libH.ffH, "doinitFontForgeMain");
		libH.LoadSplineFont		= dlsym(libH.ffH, "LoadSplineFont");
		libH.GenerateScript		= dlsym(libH.ffH, "GenerateScript");

		if (!libH.doinitFontForgeMain || !libH.LoadSplineFont || !libH.GenerateScript) 
		{
			dlclose(libH.ffH);
			return;
		}
		libH.isInitialized = 4;

		libH.SFFlatten			= dlsym(libH.ffH, "SFFlatten");
		if (libH.SFFlatten)
			libH.isInitialized += 2;

		libH.GetFontNames = dlsym(libH.ffH, "GetFontNames");
		if(libH.GetFontNames)
			libH.isInitialized += 1;
#endif
		libH.doinitFontForgeMain();
	}
}
static void libraryUninitialize()
{
	if (libH.isInitialized >= 4) {
		libH.doinitFontForgeMain = NULL;
		libH.LoadSplineFont = NULL;
		libH.GenerateScript = NULL;
		libH.isInitialized -= 4;
	}
	if (libH.isInitialized >= 2) {
		libH.SFFlatten = NULL;
		libH.isInitialized -= 2;
	}
	if (libH.isInitialized >= 1) {
		libH.GetFontNames = NULL;
		libH.isInitialized -= 1;
	}
#ifndef _WIN32
	#ifndef _WIN66
	dlclose(libH.ffH);
	#endif
#endif
	libH.ffH = NULL;
	libH.isInitialized = 0;
}

EncMap *createMapCopy(EncMap *map)
{
	EncMap *new;
	int n;

	if ((new = chunkalloc(sizeof(EncMap))) != NULL)
	{
		*new = *map;
		if ((new->map = malloc(map->enccount * sizeof(int32))) != NULL)
		{
			if ((new->backmap = malloc(map->backmax * sizeof(int32))) != NULL)
			{
				memcpy(new->map, map->map, map->enccount * sizeof(int32));
				memcpy(new->backmap, map->backmap, map->backmax * sizeof(int32));
				/* NOTE: This new->enc 'also' points to same map->enc. */
				if (map->remap == NULL)
					return(new);
				for (n = 0; map->remap[n].infont != -1; ++n);
				if ((new->remap = malloc(n * sizeof(struct remap))) != NULL)
				{
					memcpy(new->remap, map->remap, n * sizeof(struct remap));
					return(new);
				}
				free(new->backmap);
			}
			free(new->map);
		}
		free(new);
	}
	return(NULL);
}

char** get_font_names(const char* src_file)
{   
	if (libH.isInitialized % 2 != 1)
		return NULL;

    char* src_file_copy = strdup(src_file);
    char** font_names = GetFontNames(src_file_copy);
    free(src_file_copy);
    return font_names;
}

int convert_font(const char* src_file, const char* output_file)
{
	if (libH.isInitialized < 4)
		return -1;

    SplineFont* font = (SplineFont*)libH.LoadSplineFont(src_file, 1);
       
    if (font != NULL)
    {
        if(font->subfontcnt > 0 && libH.isInitialized >= 6)
        {
            EncMap *tmpMapCpy = createMapCopy(font->map);
            SFFlatten(&font);
            if(font->map == NULL)
                font->map = tmpMapCpy;
            else
                free(tmpMapCpy);
        }        
		if(GenerateScript(font, output_file, NULL, 0, -1, NULL, NULL, font->map, NULL, 1));
            return 1;
    }
    return -1;
}


