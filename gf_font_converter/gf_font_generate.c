//#include <stdio.h>
#include <stdlib.h>
#include "/Users/ccc/Downloads/fontforge-master/builds/inc/fontforge-config.h"
#include <fontforge/fontforge.h>
#include "gf_font_generate.h"

static EncMap *createMapCopy(EncMap *map);

extern int			doinitFontForgeMain(void);
extern SplineFont  *LoadSplineFont(const char *, enum openflags);
extern int			GenerateScript(SplineFont *, char *, const char *, int, int, char *, struct sflist *, EncMap *, NameList *, int);
extern void			SFFlatten(SplineFont **cidmaster);
extern char**		GetFontNames(const char*);

#include <dlfcn.h>


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

int convert_font(const char* src_file, const char* output_file)
{
    doinitFontForgeMain();
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
            return 1;
        }
        else {
            return -16;
        }
    }
    return -14;
}


