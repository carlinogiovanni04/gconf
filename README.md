# GConf - Simple binary configuration file format

GConf is a lightweight, single header binary configuration file format written in C. It is designed to be fast and dependency-free for embedding configuration management into games, tools, or small applications.

---

## File Format Specification

The `.gconf` binary format is structured sequentially:

| Offset | Size (Bytes) | Field | Description |
| :--- | :--- | :--- | :--- |
| 0 | 4 | Magic Bytes | File identifier: ASCII `"GCF\0"` (`0x47, 0x43, 0x46, 0x00`) |
| 4 | 4 | Version | Format version (currently `1`) |
| 8 | 4 | Entry Count | Number of configuration key-value pairs stored |
| 12 | 4 | Padding | Reserved/unused (`0`) |
| 16+ | Variable | Entries | Sequence of key-value pairs |

### Entry Structure
Each entry is stored one after another, using length-prefixed strings:
1. **Key Length** (`uint32_t`)
2. **Key String** (`char[]` - not null-terminated on disk)
3. **Value Length** (`uint32_t`)
4. **Value String** (`char[]` - not null-terminated on disk)

---

## Quick Start

To use GConf, include `"gconf.h"` in your project. In **one** of your C source files, define the implementation macro before including the header:

```c
#define GCONF_IMPLEMENTATION
#include "gconf.h"

```

### Writing a Config File

```c
#define GCONF_IMPLEMENTATION
#include "gconf.h"

int main(void) {
    GConfEntry entries[] = {
        gconf_entry_create("Fullscreen", "true"),
        gconf_entry_create("Resolution", "1920x1080"),
        gconf_entry_create("Volume", "80")
    };

    gconf_file_write(entries, 3, "settings.gconf");
    return 0;
}

```

### Reading a Config File

```c
#define GCONF_IMPLEMENTATION
#include "gconf.h"

int main(void) {
    GConfFile *config = gconf_file_read("settings.gconf");
    if (!config) return 1;

    for (uint32_t i = 0; i < config->header.numEntries; ++i) {
        printf("%s = %s\n", config->entries[i].key, config->entries[i].value);
    }

    gconf_file_free(config);
    return 0;
}

```

---

## API Reference

* `GConfEntry gconf_entry_create(char *key, char *value)` - Create an in-memory entry.
* `int gconf_file_write(const GConfEntry *entries, uint32_t numEntries, const char *fileName)` - Serializes and writes entries to disk. Returns `1` on success, `0` on failure.
* `GConfFile *gconf_file_read(const char *fileName)` - Reads, validates and parses a `.gconf` file from disk. Returns a pointer to a `GConfFile`, or `NULL` in case of an error.
* `void gconf_file_free(GConfFile *file)` - Frees all dynamically allocated memory associated with a `GConfFile`.
