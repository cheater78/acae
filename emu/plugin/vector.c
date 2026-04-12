#include "vector.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <memory.h>

vector_storage_t vector_new(const size_t element_size, const size_t element_count) {
    void* data = NULL;
    size_t count = 0;
    if (element_count > 0) {
        data = malloc(element_size * element_count);
        if (data) {
            count = element_count;
        }
    }
    return (vector_storage_t) {
        .element_size = element_size,
        .element_count = count,
        .data = data
    };
}

bool vector_delete(vector_t vector) {
    if (!vector)
        return false;
    free(vector->data);
    vector->data = NULL;
    vector->element_count = 0;
    return true;
}

bool vector_resize(vector_t vector, size_t element_count) {
    if (!vector)
        return false;
    if (element_count == vector->element_count)
        return true;
    if (element_count > SIZE_MAX / vector->element_size)
        return false;
    if (!vector->data && element_count > 0) {
        vector->data = malloc(vector->element_size * element_count);
        if (!vector->data)
            return false;
        vector->element_count = element_count;
        return true;
    }
    if (element_count == 0) {
        free(vector->data);
        vector->data = NULL;
        vector->element_count = 0;
        return true;
    }
    void* data = realloc(vector->data, vector->element_size * element_count);
    if (!data)
        return false;
    vector->data = data;
    vector->element_count = element_count;
    return true;
}

size_t vector_size(const vector_t vector) {
    if (!vector)
        return 0;
    return vector->element_count;
}

bool vector_push(vector_t vector, const void* element) {
    if (!vector || !element)
        return false;
    const size_t element_index = vector->element_count;
    if (!vector_resize(vector, vector->element_count + 1))
        return false;
    void* dest = (char*)vector->data + vector->element_size * (vector->element_count - 1);
    memcpy(dest, element, vector->element_size);
    return true;
}

void* vector_at(const vector_t vector, size_t index) {
    if (!vector || index >= vector->element_count)
        return NULL;
    return (char*)vector->data + vector->element_size * index;
}

size_t vector_find(const vector_t vector, const void* element) {
    if (!vector)
        return (size_t)-1;
    for (size_t i = 0; i < vector->element_count; i++) {
        void* current = (char*)vector->data + vector->element_size * i;
        if (memcmp(current, element, vector->element_size) == 0) {
            return i;
        }
    }
    return (size_t)-1;
}