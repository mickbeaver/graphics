#ifndef ARRAY_COUNT_H
#define ARRAY_COUNT_H

#include <cstddef>

// Basically the same thing that is proposed in this paper:
//   https://isocpp.org/files/papers/n4280.pdf

template <typename ArrayType, size_t NumArrayElements>
constexpr size_t ArrayCount(const ArrayType (&)[NumArrayElements]) noexcept
{
    return NumArrayElements;
}


#endif // ARRAY_COUNT_H
