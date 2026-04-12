#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <memory.h>

typedef struct vector_storage_t {
    size_t element_size;
    size_t element_count;
    void* data;
} vector_storage_t;

typedef vector_storage_t* vector_t;

vector_storage_t vector_new(const size_t element_size, const size_t element_count);
bool vector_delete(vector_t vector);
bool vector_resize(vector_t vector, size_t element_count);
size_t vector_size(const vector_t vector);
bool vector_push(vector_t vector, const void* element);
void* vector_at(const vector_t vector, size_t index);
size_t vector_find(const vector_t vector, const void* element);