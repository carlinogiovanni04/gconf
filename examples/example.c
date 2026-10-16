#define GCONF_IMPLEMENTATION
#include "gconf.h"

int main(void) {
    GConfEntry entry1 = gconf_entry_create("Fullscreen", "true");
    GConfEntry entry2 = gconf_entry_create("DebugMode", "false");

    GConfEntry entries[2] = {entry1, entry2};

    if (gconf_file_write(entries, 2, "test.gconf")) {
        printf("Successfully wrote configuration file!\n");
    }

    GConfFile *config = gconf_file_read("test.gconf");
    if (config) {
        printf("\nSuccessfully read back %u entries:\n", config->header.numEntries);
        for (uint32_t i = 0; i < config->header.numEntries; ++i) {
            printf("  [%s] = [%s]\n", config->entries[i].key, config->entries[i].value);
        }
        
        gconf_file_free(config);
    }

    return 0;
}
