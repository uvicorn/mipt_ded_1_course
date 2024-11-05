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
        void push_back(const T& element){
            if (this->size == this->capacity) {
                this->capacity <<= 1;
                for (size_t index = 0 )
            }
            m_capacity = m_size;
            Elem* new_data = new Elem[m_capacity];
              assert(new_data);

              // Перемещаем элементы если можем,
              // если они не желают перемещаться без возможного кидания ошибок
              // копируем их.
              for (size_t index = 0; i < m_size; ++i)
                new_data[i] = std::move_if_noexcept(m_data[i]);

              delete [] m_data;
              m_data = new_data;
            }

            // Тут происходит копирование.
            // Если бы мы memcpy-или какой-нибудь сложный тип
            // (вроде std::string) можно было
            // бы получить две строки с общим буффером,
            // но раздельными размерами, что не есть хорошо.
            m_data[m_size] = elem;

            m_size++;
        }
        void append(T* element){
            assert(element != NULL);

            if (this->size == this->capacity){
                this->capacity <<= 1;
                this->elements = realloc(elements, capacity);
                assert(this->elements != NULL && !"realloc failed");
            }
            memcpy(&this->elements[size], element, sizeof(T));
            this->size++;
        }

        T get(size_t index){
            assert(index < this->size);
            return this->elements[index];
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
};


#endif

