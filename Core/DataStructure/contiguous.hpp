#pragma once

#include <initializer_list>
#include <type_traits>

#include <Memory/memory.hpp>
#include <Common/common.hpp>

namespace DS {
    template <typename T>
    struct Vector {
        Vector() {
            this->m_count = 0;
            this->m_capacity = 0;
            this->m_data = nullptr;
        }

        Vector(std::initializer_list<T> list) {
            this->m_count = list.size();
            this->m_capacity = this->m_count * 2;

            this->m_data = (T*)Memory::alloc(this->m_capacity * sizeof(T));
            Memory::copy(this->m_data, this->m_capacity * sizeof(T), list.begin(), list.size() * sizeof(T));
        }

        Vector(u64 capacity) {
            this->m_count = 0;
            this->m_capacity = capacity;

            this->m_data = (T*)Memory::alloc(this->m_capacity * sizeof(T));
        }

        // Prevent copy
        Vector(const Vector& other) {
            // LOG_WARN("PERFORMING A VECTOR COPY!\n");

            RUNTIME_ASSERT(this->m_data == nullptr);

            this->m_count = other.m_count;
            this->m_capacity = other.m_capacity;

            this->m_data = (T*)Memory::alloc(this->m_capacity * sizeof(T));
            Memory::copy(this->m_data, this->m_capacity * sizeof(T), other.m_data,  this->m_capacity * sizeof(T));
        }

        Vector& operator=(const Vector& other) {
            // LOG_WARN("PERFORMING A VECTOR COPY!\n");

            // RUNTIME_ASSERT(this->m_data == nullptr);
            if (other.m_data == nullptr) return *this;

            this->m_count = other.m_count;
            this->m_capacity = other.m_capacity;
            this->m_data = (T*)Memory::alloc(this->m_capacity * sizeof(T));
            Memory::copy(this->m_data, this->m_capacity * sizeof(T), other.m_data,  this->m_capacity * sizeof(T));

            return *this;
        }

        // Prevent move
        Vector(Vector&&) = delete;
        Vector& operator=(Vector&& other) {
            this->destory();

            this->m_count = other.m_count;
            this->m_capacity = other.m_capacity;
            this->m_data = other.m_data;

            // Leave other in a invalid empty state
            other.m_count    = 0;
            other.m_capacity = 0;
            other.m_data     = nullptr;

            return *this;
        }

        ~Vector() {
            this->destory();
        }

        void reserve(u64 count) {
            const byte_t total_allocation_size = sizeof(T) * this->m_capacity;

            const byte_t old_allocation_size = sizeof(T) * this->m_count;
            const byte_t new_allocation_size = sizeof(T) * count;

            if (new_allocation_size > total_allocation_size) {
                this->m_data = Memory::realloc(this->m_data, old_allocation_size, new_allocation_size);
                this->m_capacity = count;
            }
        }

        void resize(u64 count) {
            const byte_t total_allocation_size = sizeof(T) * this->m_capacity;

            const byte_t old_allocation_size = sizeof(T) * this->m_count;
            const byte_t new_allocation_size = sizeof(T) * count;
            if (new_allocation_size > total_allocation_size) {
                this->m_data = Memory::realloc(this->m_data, old_allocation_size, new_allocation_size);
            }

            this->m_count = count;
            this->m_capacity = count;
        }

        void push(T value) {
            if (this->m_capacity < this->m_count + 1) {
                this->grow();
            }

            this->m_data[this->m_count] = value;
            this->m_count += 1;
        }

        void unstableSwapbackRemove(int i) {
            RUNTIME_ASSERT_MSG(this->m_count > 0, "You may not remove if the vector is empty!\n");
            RUNTIME_ASSERT_MSG((i >= 0) && (this->m_count - 1 >= i), "index is outside of bounds!\n");

            this->m_count -= 1;
            if (this->m_count == 0) {
                this->m_data[i] = this->m_data[this->m_count];
            }
        }

        T* begin() { 
            return m_data; 
        }
        T* end() { 
            return m_data + m_count; 
        }
        const T* begin() const { 
            return m_data; 
        }
        const T* end() const { 
            return m_data + m_count; 
        }

        u64 count() const {
            return this->m_count;
        }

        u64 capacity() const {
            return this->m_capacity;
        }

        T* data() const {
            return this->m_data;
        }
        
        T operator[](int i) const {
            RUNTIME_ASSERT_MSG((i >= 0) && (this->m_count - 1 >= i), "index is outside of bounds!\n");

            return this->m_data[i];
        }

        T& operator[](int i) {
            RUNTIME_ASSERT_MSG((i >= 0) && (this->m_count - 1 >= i), "index is outside of bounds!\n");

            return this->m_data[i];
        }
        
        void grow() {
            byte_t old_allocation_size = (this->m_capacity * sizeof(T));
            this->m_capacity *= 2;
            byte_t new_allocation_size = (this->m_capacity * sizeof(T));
            this->m_data = (T*)Memory::realloc(this->m_data, old_allocation_size, new_allocation_size);
        }

        void clear() {
            this->m_count = 0;
        }

        void shrink_to_fit() {
            this->capacity = this->m_count;
            byte_t shrunken_allocation_size = (this->m_capacity * sizeof(T));

            this->m_data = (T*)Memory::realloc(this->m_data, shrunken_allocation_size, shrunken_allocation_size);
        }
    private:
        T* m_data = nullptr;
        u64 m_count = 0;
        u64 m_capacity = 0;

        void destory() {
            if (this->m_data) {
                Memory::free(this->m_data);
            }

            this->m_count = 0;
            this->m_capacity = 0;
            this->m_data = nullptr;
        }
    };

    template <typename T>
    struct Stack {
        Stack(u64 capacity = 1) {
            this->m_count = 0;
            this->m_capacity = capacity;

            this->m_data = (T*)Memory::alloc(this->m_capacity * sizeof(T));
        }

        ~Stack() {
            this->free();
        }

        void free() {
            if (this->m_data) {
                for (byte_t i = 0; i < m_count; ++i) {
                    this->m_data[i].~T();
                }
            }

            Memory::free(this->m_data);

            this->m_data = nullptr;
            this->m_count = 0;
            this->m_capacity = 0;
        }

        T peek() const {
            RUNTIME_ASSERT_MSG(!this->empty(), "You may not peek if the stack is empty!\n");

            return this->m_data[this->count() - 1];
        }

        void push(T value) {
            if (this->m_capacity < this->m_count + 1) {
                this->grow();
            }

            this->m_data[this->m_count] = value;
            this->m_count += 1;
        }

        T pop() {
            RUNTIME_ASSERT_MSG(!this->empty(), "You may not pop if the stack is empty!\n");

            this->m_count -= 1;
            return this->m_data[this->m_count];
        }

        bool empty() const {
            return this->m_count == 0;
        }

        u64 count() const {
            return this->m_count;
        }

        u64 capacity() const {
            return this->m_capacity;
        }

        void grow() {
            if (this->m_capacity == 0 || this->m_data == nullptr) {
                // this->m_count = 0;
                // this->m_capacity = capacity;

                // this->m_data = (T*)Memory::alloc(this->m_capacity * sizeof(T));
                // return;
            }

            byte_t old_allocation_size = (this->m_capacity * sizeof(T));
            this->m_capacity *= 2;
            byte_t new_allocation_size = (this->m_capacity * sizeof(T));
            this->m_data = (T*)Memory::realloc(this->m_data, old_allocation_size, new_allocation_size);
        }
    private:
        T* m_data = nullptr;
        u64 m_count = 0;
        u64 m_capacity = 0;
    };
    
    template <typename T>
    struct RingQueue {
        RingQueue(byte_t capacity = 1) {
            RUNTIME_ASSERT(capacity > 0);
            
            this->m_count = 0;
            this->m_capacity = capacity;
            this->m_data = Memory::alloc(this->m_capacity * sizeof(T));
        }

        ~RingQueue() {
            Memory::free(this->m_data);

            this->m_data = nullptr;
            this->m_count = 0;
            this->m_capacity = 0;

        }

        bool enqueue(T value) {
            RUNTIME_ASSERT_MSG(!this->full(), "You may not enqueue if the ring queue is full!\n");

            this->data[this->m_write] = value;
            this->m_count += 1;
            this->write = (this->write + 1) % this->m_capacity;
        }
        
        T dequeue() {
            RUNTIME_ASSERT_MSG(!this->empty(), "You may not dequeue if the ring queue is empty!\n");

            T ret = this->data[this->m_read];
            this->m_count -= 1;
            this->m_read = (this->m_read + 1) % this->m_capacity;

            return ret;
        }

        bool empty() const {
            return this->m_count == 0;
        }

        bool full() const {
            return this->m_count == this->m_capacity;
        }

        u64 count() const {
            return this->m_count;
        }

        u64 capacity() const {
            return this->m_capacity;
        }

    private:
        T* m_data = nullptr;
        u64 m_count = 0;
        u64 m_capacity = 0;

        byte_t m_read = 0;
        byte_t m_write = 0;
    };
}