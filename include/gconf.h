#ifndef GCONF_H
#define GCONF_H

#include <stdint.h>

typedef struct{
    uint32_t magic;      // GCF\0 - 0x47434600
    uint32_t numEntries; // up to 4 billion
} GCONF_HEADER;

typedef struct {
} GCONF_ENTRY;


#ifndef GCONF_IMPLEMENTATION




#endif
#endif
