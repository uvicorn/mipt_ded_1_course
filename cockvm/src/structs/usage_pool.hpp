#ifndef USAGEPOOL_H
#define USAGEPOOL_H

#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <type_traits>

enum UsagePoolError : unsigned char {
    NO_ERROR                = 0,
    ELEMENT_NOT_INITIALIZED = 1 << 0,
    IS_ALREADY_USED         = 1 << 1,
    POOL_IS_FULL            = 1 << 2
};

// ЭТО ГОВНО БЛЯТЬ
// НЕ ИСПОЛЬЗОВАТЬ ВЕДЕТСЯ АССЕНИЗАЦИЯ
//
/*
 * Владение объектами внутри пула полностью передается ему, то есть его нельзя искусственно удалить, только через пулл
 * Пока это не сделано и нужно ручками херачить внутрь уже сконстру
*/

template <typename T, const bool ElementRewriteBlock=1>
class UsagePool {
    private:
        size_t capacity;
        bool* used;
        T** elements;

        // TODO: обдумать можно ли выдавать нулевой элемент если ElementBlocking=0 и пул заполнен
        UsagePoolError update_top_index(){
            while (this->used[this->top_index] && this->top_index < this->capacity)
                this->top_index++;
            if (this->top_index == this->capacity)
                return POOL_IS_FULL;
            return NO_ERROR;
        }

    public:
        size_t top_index = 0;
        UsagePool(size_t capacity){
            this->capacity = capacity;
            this->elements = malloc(capacity * sizeof(T* ));
            this->used = calloc(capacity, sizeof(T* ));
        }

        // работает за O(n), надо переписать на хранение всех задействованных ячеек
        ~UsagePool(){
            for (size_t index = 0; index < this->capacity; index++){
                if (this->used[index])
                    delete this->elements[index];
            }
        }

        UsagePoolError set_element(size_t index, T* element){
            assert(index < this->capacity);

            if (this->used[index]){
                if (ElementRewriteBlock)
                    return IS_ALREADY_USED;
                else
                    delete elements[index];
            }
            this->elements[index] = element;
            return NO_ERROR;
        }

        UsagePoolError get_element(size_t index, T** output_element){
            assert(index < this->capacity);

            if (!this->used[index]){
                return ELEMENT_NOT_INITIALIZED;
            }
            *output_element = this->elements[index];
            return NO_ERROR;
        }

        UsagePoolError remove_element(size_t index){
            assert(index < this->capacity);
            if (this->used[index]){
                delete this->elements[index];
                this->used[index] = 0;
            } else {
                return ELEMENT_NOT_INITIALIZED;
            }
            return NO_ERROR;
        }

        UsagePoolError append_element(T* element){
            if (update_top_index() == POOL_IS_FULL) 
                return POOL_IS_FULL;
            return set(this->top_index, element);
        }
};

#endif
