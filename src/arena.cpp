#include "arena.hpp"

#include <cstdlib>

ArenaAllocator::ArenaAllocator(size_t bytes)
    :m_size(bytes)
{
    m_buffer = static_cast<std::byte*>(malloc(m_size));
    m_offset = m_buffer;
}

ArenaAllocator::~ArenaAllocator()
{
    free(m_buffer);
}