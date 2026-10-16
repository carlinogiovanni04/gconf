#ifndef GCONF_H
#define GCONF_H

#include <stdint.h>

typedef struct{
    uint32_t magic;      // GCF\0 - 0x47434600
    uint32_t numEntries; // up to 4 billion
    uint32_t version;    // 0.1 current
    uint32_t padding;    // Unused for now
} GConfHeader;

typedef struct {
    const char *key;
    const char *value;
} GConfEntry;


#ifndef GCONF_IMPLEMENTATION




#endif
#endif
