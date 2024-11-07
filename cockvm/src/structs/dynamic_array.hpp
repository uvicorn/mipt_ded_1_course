#ifndef DYNAMIC_ARRAY_HPP
#define DYNAMIC_ARRAY_HPP

#include <algorithm>
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#define ARRAY_CAPACITY 16

template <typename T>
class DynamicArray {
    public:

        DynamicArray(size_t capacity = ARRAY_CAPACITY, const T& default_elements = T()){
            /* assert(capacity != 0); */
            this->size     = capacity;
            this->capacity = capacity;

            this->elements = new T[capacity];
            std::fill(this->elements, this->elements + capacity, default_elements);

            assert(this->elements);
        }

        ~DynamicArray(){
            delete[] this->elements;
        }

        T& get(size_t index){
            assert(index < this->size);
            return this->elements[index];
        }

        void push_back(const T& element){
            if (this->size == this->capacity) {
                this->capacity <<= 1;

                T* new_elements = new T[this->capacity];
                assert(new_elements);

                for (size_t index = 0; index < this->size; index++)
                    new_elements[index] = std::move_if_noexcept(this->elements[index]);

                delete[] this->elements;
                this->elements = new_elements;
            }
            // ПРОИСХОДИТ КОПИРОВАНИЕ
            this->elements[this->size++] = element;
        }
        T& pop_back(){
            assert(0 < this->size);
            return this->elements[this->size--];
        }

    private:
        size_t size = 0;
        size_t capacity = 0;
        T* elements;
};


#endif

