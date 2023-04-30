#pragma once
#include <cstddef>
#include <iostream>

//================//
// SOLUTION BELOW //
//================//

// As given by examples, this is a smart pointer to an array(!)
template <class T>
class smart_ptr {
    static_assert(!std::is_same_v<T, void>, "pointer to void is malformed");

    struct Block {
        T* begin;
        size_t count;

        Block(T* _) : begin(_), count(1) {}
        ~Block() {
            delete[] begin;
        }
    };

    Block* block;
    mutable bool is_checked;


    void clear_block() {
        if (block && --(block->count) == 0) {
            delete block;
        }
    }

public:
    smart_ptr() : block(nullptr), is_checked(false) {};
    smart_ptr(T* _) : block(new Block(_)), is_checked(false) {};
    smart_ptr(const smart_ptr& other) : block(other.block), is_checked(other.is_checked) {
        if (block) {
            block->count++;
        }
    }
    smart_ptr(smart_ptr&& other) noexcept : block(other.block), is_checked(other.is_checked) {
        other.block = nullptr;
    };
    smart_ptr& operator=(const smart_ptr& other) {
        if (this != &other) {
            clear_block();
            block = other.block;
            if (block) {
                block->count++;
            }
            is_checked = other.is_checked;
        }
        return *this;
    }
    smart_ptr& operator=(smart_ptr&& other) noexcept {
        if (&other != this) {
            clear_block();
            block = other.block;
            is_checked = other.is_checked;
            other.block = nullptr;
        } 
        return *this;
    }
    ~smart_ptr() {
        clear_block();
    }
    T& operator[](size_t i) {
        if (block && !is_checked) {
            is_checked = true;
            fprintf(stderr, "Warning: pointer is used without checking on null\n");
        }
        return block->begin[i];
    }
    const T& operator[](size_t i) const {
        if (block && !is_checked) {
            is_checked = true;
            fprintf(stderr, "Warning: pointer is used without checking on null\n");
        }
        return block->begin[i];
    }
    operator bool() const {
        is_checked = true;
        if (block) {
            return block->begin;
        }
        return false;
    }
};