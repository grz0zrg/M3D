#include "hash_table.h"

#define M3D_HASH_TABLE_INITIAL_SIZE 31

int primes[27] = {31, 61, 127, 251, 509, 1021, 2039, 4093, 8191, 16381,
                  32749, 65521, 131071, 262139, 524287, 1048573, 2097143,
                  4194301, 8388593, 16777213, 33554393, 67108859, 134217689,
                  268435399, 536870909, 1073741789, 2147483647};

inline unsigned int m3d_HashTableCode(m3d_HashTable *hash_table,
                                      const char *str) {
    if (!hash_table || !str) return -1;

    unsigned int hash = 0;

    do {
        hash = (9 * hash + *str) % hash_table->table_size;
    } while (*str++ != '\0');

    if (hash_table->lg_table_size < 26) {
        hash = hash % primes[hash_table->lg_table_size+5];
        return hash % hash_table->table_size;
    }

    return hash;
}

m3d_HashTable *m3d_HashTableInit() {
	m3d_HashTable *hash_table = (m3d_HashTable *)malloc(sizeof(m3d_HashTable));

	if (!hash_table) return NULL;

    memset(hash_table, 0, sizeof(m3d_HashTable));

    hash_table->table_size = M3D_HASH_TABLE_INITIAL_SIZE;
    hash_table->lg_table_size = log10(hash_table->table_size);
    hash_table->keys = (char **)malloc(sizeof(char *) * hash_table->table_size);
    hash_table->vals = (void **)malloc(sizeof(void *) * hash_table->table_size);

    memset(hash_table->keys, 0, sizeof(char *) * hash_table->table_size);
    memset(hash_table->vals, 0, sizeof(char *) * hash_table->table_size);

	return hash_table;
}

m3d_HashTable *m3d_HashTableInitExt(unsigned int size) {
	m3d_HashTable *hash_table = (m3d_HashTable *)malloc(sizeof(m3d_HashTable));

	if (!hash_table) return NULL;

    memset(hash_table, 0, sizeof(m3d_HashTable));

    hash_table->table_size = size;
    hash_table->lg_table_size = log10(size);
    hash_table->keys = (char **)malloc(sizeof(char *) * size);
    hash_table->vals = (void **)malloc(sizeof(void *) * size);

    memset(hash_table->keys, 0, sizeof(char *) * size);
    memset(hash_table->vals, 0, sizeof(char *) * size);

	return hash_table;
}

int m3d_HashTableResize(m3d_HashTable *hash_table, unsigned int new_size) {
    if (!hash_table) return 0;

    m3d_HashTable *ht_tmp = m3d_HashTableInitExt(new_size);

    char **keys = hash_table->keys;
    void **vals = hash_table->vals;

    unsigned int i = 0;
    for (i = 0; i < hash_table->table_size; i++) {
        m3d_HashTableSet(ht_tmp, keys[i], vals[i]);
        free(keys[i]);
    }

    free(hash_table->keys);
    free(hash_table->vals);

    hash_table->table_size    = ht_tmp->table_size;
    hash_table->entry_size    = ht_tmp->entry_size;
    hash_table->lg_table_size = ht_tmp->lg_table_size;
    hash_table->keys          = ht_tmp->keys;
    hash_table->vals          = ht_tmp->vals;

    free(ht_tmp);

    return 1;
}

int m3d_HashTableSet(m3d_HashTable *hash_table,
                                const char *key,
                                void *value) {
    if (!hash_table || !key || !value) return 0;

    if (hash_table->entry_size >= hash_table->table_size / 2) {
        if (!m3d_HashTableResize(hash_table, 2 * hash_table->table_size)) {
            return 0;
        }
    }

    unsigned int hash = m3d_HashTableCode(hash_table, key);
    if (hash == (unsigned int)-1) return 0;

    int i = 0;
    for (i = hash; hash_table->keys[i] != NULL;
                                        i = (i + 1) % hash_table->table_size) {
        if (strcmp(hash_table->keys[i], key) == 0) {
            hash_table->vals[i] = value;
            return 1;
        }
    }

    size_t key_length = strlen(key)+1;
    char *key_cpy = (char *)malloc(sizeof(char) * key_length);
    if (!key_cpy) return 0;

    strncpy(key_cpy, key, key_length);

    hash_table->keys[i] = key_cpy;
    hash_table->vals[i] = value;

    hash_table->entry_size++;

    return 1;
}

void *m3d_HashTableGet(m3d_HashTable *hash_table, const char *key) {

    if (!hash_table || !key) return NULL;

    unsigned int hash = m3d_HashTableCode(hash_table, key);

    int i = 0;
    for (i = hash; hash_table->keys[i] != NULL;
                                        i = (i + 1) % hash_table->table_size) {
        if (strcmp(hash_table->keys[i], key) == 0) {
            return hash_table->vals[i];
        }
    }

    return NULL;
}

void m3d_HashTableDelete(m3d_HashTable *hash_table, const char *key) {
    if (!hash_table || !key) return;
    if (!m3d_HashTableGet(hash_table, key)) return;

    int i = m3d_HashTableCode(hash_table, key);
    while (strcmp(key, hash_table->keys[i]) != 0) {
        i = (i + 1) % hash_table->table_size;
    }

    free(hash_table->keys[i]);

    hash_table->keys[i] = NULL;
    hash_table->vals[i] = NULL;

    i = (i + 1) % hash_table->table_size;

    while (hash_table->keys[i] != NULL) {
        char *rkey = hash_table->keys[i];
        char *rval = hash_table->vals[i];
        hash_table->keys[i] = NULL;
        hash_table->vals[i] = NULL;

        hash_table->entry_size--;

        m3d_HashTableSet(hash_table, rkey, rval);
        i = (i + 1) % hash_table->table_size;
    }

    hash_table->entry_size--;
    if (hash_table->entry_size > 0 ||
        hash_table->entry_size == hash_table->table_size/8) {
        m3d_HashTableResize(hash_table, hash_table->table_size/2);
    }
}

void m3d_HashTableDebug(m3d_HashTable *hash_table) {
    if (!hash_table) return;

    printf(">------- HashTable occupancy pattern <-------\n");
    int wrap = 96;
    unsigned int i = 0;
    for (i = 0; i < hash_table->table_size; i++) {
        if (i%wrap == 0) {
            printf("\n");
        }

        if (hash_table->keys[i]) {
            printf("*");
        } else {
            printf("-");
        }
    }

    printf("\n\n");
    printf(">------- HashTable content <-------\n\n");
    for (i = 0; i < hash_table->table_size; i++) {
        printf("key[%i] = %s\n", i, hash_table->keys[i]);
    }
}

const char *m3d_HashTableGetKeyAt(m3d_HashTable *hash_table,
                                  unsigned int index) {
    if (!hash_table) return NULL;
    if (hash_table->table_size == 0) return NULL;
    if (index >= hash_table->table_size) return NULL;

    return hash_table->keys[index];
}

void *m3d_HashTableGetValueAt(m3d_HashTable *hash_table, unsigned int index) {
    if (!hash_table) return NULL;
    if (hash_table->table_size == 0) return NULL;
    if (index >= hash_table->table_size) return NULL;

    return hash_table->vals[index];
}

void m3d_HashTableFree(m3d_HashTable *hash_table) {
    if (!hash_table) return;

    unsigned int i = 0;
    for (i = 0; i < hash_table->table_size; i++) {
        free(hash_table->keys[i]);
    }

    free(hash_table->keys);
    free(hash_table->vals);
    free(hash_table);
}
