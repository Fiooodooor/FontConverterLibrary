
#ifndef TTF_TO_TTF_H
#define TTF_TO_TTF_H

#include <sys/types.h>
#include <sys/stat.h>

char** get_font_names(const char* src_file);
int convert_font(const char* src_file, char* output_file);

#endif // TTF_TO_TTF_H

