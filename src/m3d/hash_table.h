#ifndef __M3D_HASH_TABLE_H__
#define __M3D_HASH_TABLE_H__

    /**
     * Hash table implemented using a simple linear probing algorithm.
     *
     * Note: It store void pointers so it is a sort of "generic storage",
     *       DISADVANTAGE IS: YOU NEED TO FREE THE DATA YOU PUT INTO IT!
     */

    #include <stdlib.h>
    #include <string.h>
    #include <stdio.h>
    #include <math.h>

    typedef struct {
        unsigned int table_size;
        unsigned int entry_size;

        int lg_table_size;

        char **keys;
        void **vals;
    } m3d_HashTable;

    m3d_HashTable *m3d_HashTableInit();
    int m3d_HashTableSet(m3d_HashTable *hash_table,
                              const char *key,
                              void *value);
    void *m3d_HashTableGet(m3d_HashTable *hash_table, const char *key);
    void m3d_HashTableDelete(m3d_HashTable *hash_table, const char *key);
    void m3d_HashTableDebug(m3d_HashTable *hash_table);
    const char *m3d_HashTableGetKeyAt(m3d_HashTable *hash_table,
                                  unsigned int index);
    void *m3d_HashTableGetValueAt(m3d_HashTable *hash_table,
                                  unsigned int index);
    void m3d_HashTableFree(m3d_HashTable *hash_table);

#endif // __M3D_HASH_TABLE_H__
