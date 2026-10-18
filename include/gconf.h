#ifndef GCONF_H
#define GCONF_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char magic[4];       // "GCF\0"
  uint32_t version;    // 0x1 current
  uint32_t numEntries; // Number of entries
  uint32_t padding;    // Unused for now
} GConfHeader;

typedef struct {
  char *key;
  char *value;
} GConfEntry;

typedef struct {
  GConfHeader header;
  GConfEntry *entries;
} GConfFile;

// Public API Declarations
GConfEntry gconf_entry_create(char *key, char *value);
int gconf_file_write(const GConfEntry *entries, uint32_t numEntries,
                     const char *fileName);
GConfFile *gconf_file_read(const char *fileName);
void gconf_file_free(GConfFile *file);

#ifdef GCONF_IMPLEMENTATION

GConfEntry gconf_entry_create(char *key, char *value) {
  GConfEntry result = {.key = key, .value = value};
  return result;
}

int gconf_file_write(const GConfEntry *entries, uint32_t numEntries,
                     const char *fileName) {
  if (!entries || numEntries == 0 || !fileName) {
    printf("[GCONF] gconf_file_write - invalid arguments!\n");
    return 0;
  }

  FILE *file = fopen(fileName, "wb");
  if (!file) {
    printf("[GCONF] gconf_file_write - fopen failed!\n");
    return 0;
  }

  GConfHeader header = {.magic = {'G', 'C', 'F', '\0'},
                        .version = 1,
                        .numEntries = numEntries,
                        .padding = 0};

  fwrite(&header, sizeof(GConfHeader), 1, file);

  // Write entries with length prefixes
  for (uint32_t i = 0; i < numEntries; ++i) {
    uint32_t key_len = (uint32_t)strlen(entries[i].key);
    uint32_t val_len = (uint32_t)strlen(entries[i].value);

    fwrite(&key_len, sizeof(uint32_t), 1, file);
    fwrite(entries[i].key, 1, key_len, file);

    fwrite(&val_len, sizeof(uint32_t), 1, file);
    fwrite(entries[i].value, 1, val_len, file);
  }

  fclose(file);
  return 1;
}

GConfFile *gconf_file_read(const char *fileName) {
  FILE *file = fopen(fileName, "rb");
  if (!file) {
    printf("[GCONF] gconf_file_read - fopen failed!\n");
    return NULL;
  }

  GConfHeader header;
  if (fread(&header, sizeof(GConfHeader), 1, file) != 1) {
    printf("[GCONF] gconf_file_read - failed to read header!\n");
    fclose(file);
    return NULL;
  }

  if (memcmp(header.magic, "GCF", 4) != 0) {
    printf("[GCONF] gconf_file_read - invalid magic number!\n");
  }

  GConfFile *gconfFile = malloc(sizeof(GConfFile));
  if (!gconfFile) {
    fclose(file);
    return NULL;
  }

  gconfFile->header = header;
  gconfFile->entries = calloc(header.numEntries, sizeof(GConfEntry));
  if (!gconfFile->entries) {
    free(gconfFile);
    fclose(file);
    return NULL;
  }

  uint32_t successfully_read = 0;
  for (uint32_t i = 0; i < header.numEntries; ++i) {
    uint32_t key_len = 0;
    if (fread(&key_len, sizeof(uint32_t), 1, file) != 1)
      break;

    char *key = malloc(key_len + 1);
    if (!key)
      break;
    if (fread(key, 1, key_len, file) != key_len) {
      free(key);
      break;
    }
    key[key_len] = '\0';

    uint32_t val_len = 0;
    if (fread(&val_len, sizeof(uint32_t), 1, file) != 1) {
      free(key);
      break;
    }

    char *value = malloc(val_len + 1);
    if (!value) {
      free(key);
      break;
    }
    if (fread(value, 1, val_len, file) != val_len) {
      free(key);
      free(value);
      break;
    }
    value[val_len] = '\0';

    gconfFile->entries[i].key = key;
    gconfFile->entries[i].value = value;
    successfully_read++;
  }

  if (successfully_read < header.numEntries) {
    printf("[GCONF] gconf_file_read - file corrupted or read error!\n");
    gconfFile->header.numEntries = successfully_read;
    gconf_file_free(gconfFile);
    fclose(file);
    return NULL;
  }

  fclose(file);
  return gconfFile;
}

void gconf_file_free(GConfFile *file) {
  if (!file)
    return;
  for (uint32_t i = 0; i < file->header.numEntries; ++i) {
    free(file->entries[i].key);
    free(file->entries[i].value);
  }
  free(file->entries);
  free(file);
}

#endif // GCONF_IMPLEMENTATION
#endif // GCONF_H
