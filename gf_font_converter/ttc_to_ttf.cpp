
#include <fstream>
#include <sys/stat.h>
#include <algorithm>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <vector>

#include <CoreFoundation/CFByteOrder.h>

#include "ttc_to_ttf.hpp"


#ifdef __cplusplus
extern "C" {
#endif
    #include "ttf_to_ttf.h"

#ifdef __cplusplus
}
#endif
template <class T>
void endswap(T *objp)
{
    unsigned char *memp = reinterpret_cast<unsigned char*>(objp);
    std::reverse(memp, memp + sizeof(T));
}

int32_t ttcTottfConvert(std::string sourceFile, std::string sourcePath, std::string destinationPath, int index, std::vector<std::string> &fontsList)
{
    char filename[260];
    sprintf(filename, "%s/%s", sourcePath.c_str(), sourceFile.c_str());
    
    uint32_t ttf_count = 0;
    
    FILE* in_file = fopen(filename, "rt");
    if (in_file == NULL) {
        return -2;
    }
    
    struct    stat    info;
    fstat(fileno(in_file), &info);
    char*            buffer = (char*) malloc(info.st_size);
    fread(buffer, sizeof(char), info.st_size, in_file);
    fclose(in_file);
    
    if (strlen(filename) > 4) {
        char    tmp[5];
        strcpy(tmp, &filename[strlen(filename) - 4]);
        for (long i = 0; i < 4; i++)
            tmp[i] = toupper(tmp[i]);
        if (strcmp(tmp, ".TTC") == 0)
            filename[strlen(filename) - 4] = 0;
    }
  
    if (*(int32_t*)buffer != *(int32_t*)"ttcf")
    {
        return -1;
    }
    else
    {
        ttf_count = *(uint32_t*)&buffer[0x08];
        endswap(&ttf_count);
    
        std::vector<uint32_t> ttf_offset_array;
    
        for (uint32_t i = 0; i < ttf_count; i++)
        {
            uint32_t offset = *(uint32_t*)&buffer[0x0C + i*0x04];
            endswap(&offset);
            ttf_offset_array.push_back(offset);
        }
    
        for (uint32_t i = 0; i < ttf_count; i++)
        {
            uint32_t        table_header_offset = ttf_offset_array[i];
            uint16_t        table_count = *(uint16_t*)&buffer[table_header_offset + 0x04];
            endswap(&table_count);
    
            uint32_t        header_length = 0x0C + table_count * 0x10;
            uint32_t        table_length = 0;
    
            for (uint16_t j = 0; j < table_count; j++)
            {
                uint32_t    length = *(uint32_t*)&buffer[table_header_offset + 0x0C + 0x0C + j * 0x10];
                endswap(&length);
                table_length += (length + 3) & ~3;
            }
    
            uint32_t    total_length = header_length + table_length;
    
            char* new_buffer = (char*)malloc(total_length);
            uint32_t    pad = 0;
            memcpy(new_buffer, &buffer[table_header_offset], header_length);
            uint32_t    current_offset = header_length;
    
            for (uint16_t j = 0; j < table_count; j++)
            {
                uint32_t    offset = *(uint32_t*)&buffer[table_header_offset + 0x0C + 0x08 + j * 0x10];
                endswap(&offset);
                uint32_t    length = *(uint32_t*)&buffer[table_header_offset + 0x0C + 0x0C + j * 0x10];
                endswap(&length);
    
                uint32_t tmp_offset = current_offset;
                endswap(&tmp_offset);
                *(uint32_t*)&new_buffer[0x0C + 0x08 + j * 0x10] = tmp_offset;
    
                memcpy(&new_buffer[current_offset], &buffer[offset], length);
                memcpy(&new_buffer[current_offset + length], &pad, ((length + 3) & ~3) - length);
    
                *(uint32_t*)&new_buffer[0x0C + 0x04 + j * 0x10] = CalcTableChecksum((uint32_t*)&new_buffer[current_offset], length);
                current_offset += (length + 3) & ~3;
            }


            char    out_filename_tmp[260];
            char    out_filename[260];
            sprintf(out_filename_tmp, "%s/temp.tff", destinationPath.c_str());
            FILE*    out_file = fopen(out_filename_tmp, "wt");
            fwrite(new_buffer, sizeof(char), total_length, out_file);
            fclose(out_file);
            free(new_buffer);
            
            sprintf(out_filename, "%s/%d_%d%s.ttf", destinationPath.c_str(), index, i, sourceFile.c_str());
            convert_font(out_filename_tmp, out_filename);
            fontsList.push_back(out_filename);
        }
    }

    free(buffer);
    return ttf_count;
}

uint32_t CalcTableChecksum(uint32_t* table, uint32_t length)
{
    uint32_t    sum = 0L;
    uint32_t*    end_ptr = table + ((length + 3) & ~3) / sizeof(uint32_t);
    uint32_t tmp;
    while (table < end_ptr)
    {
        tmp = *table++;
        endswap(&tmp);
        sum += tmp;
    }

    endswap(&sum);
    return sum;
}

