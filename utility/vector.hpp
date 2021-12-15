#ifndef AOC_UTILITY_ARRAY2_HPP
#define AOC_UTILITY_ARRAY2_HPP

#include <cstdint>

namespace aoc
{
    template<typename T>
    struct Vector2
    {
        T x = 0;
        T y = 0;

        Vector2 operator +(const Vector2& rhs) const
        {
            return Vector2{ x + rhs.x, y + rhs.y };
        }

        Vector2 operator -(const Vector2& rhs) const
        {
            return Vector2{ x - rhs.x, y - rhs.y };
        }

        Vector2 operator *(const T& scalar) const
        {
            return Vector2{ x * scalar, y * scalar };
        }

        bool operator ==(const Vector2& rhs) const
        {
            return x == rhs.x && y == rhs.y;
        }

        bool operator !=(const Vector2& rhs) const
        {
            return x != rhs.x || y != rhs.y;
        }
    };

    using Vector2s= Vector2<size_t>;
    using Vector2i32 = Vector2<int32_t>;
}

#endif