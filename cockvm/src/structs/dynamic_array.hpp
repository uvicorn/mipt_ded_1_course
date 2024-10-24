#ifndef DYNAMIC_ARRAY
#define DYNAMIC_ARRAY

#include <assert.h>
#include <cstdlib>
#include <stddef.h>
#include <string.h>

#define ARRAY_CAPACITY 16

template <typename T>
class DynamicArray {
    public:
        virtual ~DynamicArray(){
            free(elements);
            /* delete this; */
        }

        void append(T* element){
            assert(element != NULL);

            if (this->size == this->capacity){
                this->capacity <<= 1;
                this->elements = realloc(elements, capacity);
            }
            memcpy(&this->elements[size], element, sizeof(T));
            this->size += sizeof(T);
        }

        T get(size_t index){
            assert(index * sizeof(T) < size);
            return elements[index * sizeof(T)];
        }
        DynamicArray(size_t capacity){
            this->elements = allocate_elements(capacity);
        }

    private:
        size_t size = 0;
        size_t capacity = 0;
        T* elements;

        T* allocate_elements(size_t capacity){
            assert(capacity != 0);
            elements = malloc(capacity * sizeof(T));
            this->capacity = capacity * sizeof(T);
            return elements;
        }
};
template <typename T>
class UsagePool {
    public:
        
};

#endif

