#pragma once
#include <cstddef> // size_t

using namespace std;

class Allocator {
public  :
    size_t m_TotalSize;
    size_t m_used;
    size_t m_peak;
public :
    Allocator(const size_t totalSize) : m_TotalSize(totalSize), m_used(0), m_peak(0) {};
    virtual ~Allocator(){m_TotalSize = 0;}
    virtual void* Allocate(const std::size_t allocationSize, const std::size_t alignment) = 0;
    virtual void Init() = 0;
    virtual void Free(void* ptr) = 0;
};

/*
class Allocator {
protected:
    std::size_t m_totalSize;
    std::size_t m_used;
    std::size_t m_peak;
public:

    Allocator(const std::size_t totalSize) : m_totalSize{ totalSize }, m_used{ 0 }, m_peak{ 0 } { }

    virtual ~Allocator() { m_totalSize = 0; }

    virtual void* Allocate(const std::size_t size, const std::size_t alignment = 0) = 0;

    virtual void Free(void* ptr) = 0;

    virtual void Init() = 0;

    friend class Benchmark;
};

*/