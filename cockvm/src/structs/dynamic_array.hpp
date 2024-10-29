#ifndef DYNAMIC_ARRAY
#define DYNAMIC_ARRAY

#include <assert.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#define ARRAY_CAPACITY 16

template <typename T>
class DynamicArray {
    public:
        ~DynamicArray(){
            /* for (int index = 0; index < this->size; index++){ */
            /*     delete this->elements[index]; */
            /* } */
            free(elements);
        }

        void append(T* element){
            assert(element != NULL);

            if (this->size == this->capacity){
                this->capacity <<= 1;
                this->elements = realloc(elements, capacity);
                assert(this->elements != NULL && !"realloc failed");
            }
            memcpy(&this->elements[size * sizeof(T)], element, sizeof(T));
            this->size++;
        }

        T get(size_t index){
            assert(index < this->size);
            return this->elements[index * sizeof(T)];
        }

        DynamicArray(size_t capacity = ARRAY_CAPACITY, T* default_elements = NULL){
            assert(capacity != 0);
            
            this->capacity = capacity;
            this->elements = malloc(capacity * sizeof(T));
            if (default_elements != NULL){
                memcpy(elements, default_elements, capacity * sizeof(T));
            }
        }

    private:
        size_t size = 0;
        size_t capacity = 0;
        T* elements;

        T* allocate_elements(size_t capacity){
            assert(capacity != 0);

        }
};


#endif

