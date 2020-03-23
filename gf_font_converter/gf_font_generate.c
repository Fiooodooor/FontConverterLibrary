//#include <stdio.h>
#include <stdlib.h>
#include <fontforge/fontforge.h>
#include "gf_font_generate.h"

static EncMap *createMapCopy(EncMap *map);

#if defined(_WIN32) || defined(_WIN64)
	extern int			doinitFontForgeMain(void);
	extern SplineFont  *LoadSplineFont(const char *, enum openflags);
	extern int			GenerateScript(SplineFont *, char *, const char *, int, int, char *, struct sflist *, EncMap *, NameList *, int);
	extern void			SFFlatten(SplineFont **cidmaster);
	extern char**		GetFontNames(const char*);
#else

    #include <dlfcn.h>
#endif

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

int convert_font(const char* src_file, const char* output_file, const char* library_path)
{
    void *ffH = dlopen(library_path, RTLD_LOCAL | RTLD_LAZY);
    if(!ffH)
        return -10;
    
   int(*doinitFontForgeMain)(void) = dlsym(ffH, "doinitFontForgeMain");
   if(doinitFontForgeMain)
       doinitFontForgeMain();
   else {
       dlclose(ffH);
       ffH = NULL;
       return -11;
   }   
        
    SplineFont *(*LoadSplineFont)(const char *, enum openflags) = dlsym(ffH, "LoadSplineFont");
    int(*GenerateScript)(SplineFont *, char *, const char *, int, int, char *, struct sflist *, EncMap *, NameList *, int) = dlsym(ffH, "GenerateScript");;
    void(*SFFlatten)(SplineFont **cidmaster) = dlsym(ffH, "SFFlatten");
    
    if(!LoadSplineFont || !GenerateScript || !SFFlatten) {
        dlclose(ffH);
        ffH = NULL;
        return -12;
    }
    SplineFont* font = (SplineFont*)LoadSplineFont(src_file, 1);
       
    if (font != NULL)
    {
        if(font->subfontcnt > 0)
        {
            EncMap *tmpMapCpy = createMapCopy(font->map);
            if(!tmpMapCpy)
                return -13;
            SFFlatten(&font);
            if(font->map == NULL)
                font->map = tmpMapCpy;
            else
                free(tmpMapCpy);
        }
        char mutable_output_file[300];
        strcpy(mutable_output_file, output_file);
        if(GenerateScript(font, mutable_output_file, NULL, 0, -1, NULL, NULL, font->map, NULL, 1)) {
            dlclose(ffH);
            ffH = NULL;
            return 1;
        }
        else {
            dlclose(ffH);
            ffH = NULL;
            return -16;
        }
    }
    dlclose(ffH);
    ffH = NULL;
    return -14;
}


