#pragma once

#include <cstddef>

class ArenaAllocator {
public:
    ArenaAllocator(size_t bytes);
    ~ArenaAllocator();

    template<typename T> T* alloc()
    {
        void* offset = m_offset;
        m_offset += sizeof(T);
        return static_cast<T*>(offset);
    }
private:

    size_t m_size;
    std::byte* m_buffer;
    std::byte* m_offset;
};