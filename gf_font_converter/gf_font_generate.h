
#ifndef GF_FONT_GENERATE_H
#define GF_FONT_GENERATE_H

static void libraryInitialize();
static void libraryUninitialize();
static char** get_font_names(const char* src_file);
static int convert_font(const char* src_file, const char* output_file);

#endif // GF_FONT_GENERATE_H

