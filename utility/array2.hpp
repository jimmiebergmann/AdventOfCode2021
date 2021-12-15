#ifndef AOC_UTILITY_VECTOR_HPP
#define AOC_UTILITY_VECTOR_HPP

#include <memory>
#include <iostream>
#include <functional>
#include "vector.hpp"

namespace aoc
{

    template<typename T>
    struct Array2
    {
        using Type = T;
        using DataRow = std::unique_ptr<T[]>;
        using Data = std::unique_ptr<DataRow[]>;

    	Vector2s size = {};
        Data data = nullptr;

        Array2() = default;

        Array2(const Vector2s size, Data&& data) :
            size(size),
            data(std::move(data))
        {}

        Array2(Array2&& rhs) noexcept :
            size(rhs.size),
            data(std::move(rhs.data))
        { }

        Array2& operator = (Array2&& rhs)
        {
            size = rhs.size;
            data = std::move(rhs.data);
            return *this;
        }

        bool is_outside(const Vector2i32 position) const
        {
            return position.x < 0 || position.y < 0 || position.x >= size.x || position.y >= size.y;
        }
        
    };

    template<typename T>
    Array2<T> make_array2(Vector2s size)
    {
        Array2<T> array2d{ size, std::make_unique<typename Array2<T>::DataRow[]>(size.y) };
        for (size_t i = 0; i < size.y; i++)
        {
            array2d.data[i] = std::make_unique<T[]>(size.x);
        }
        return array2d;
    }

    template<typename TPrint, typename T>
    void print_array2(const Array2<T>& array2)
    {
        for (size_t y = 0; y < array2.size.y; y++)
        {
            for (size_t x = 0; x < array2.size.x; x++)
            {
                std::cout << static_cast<TPrint>(array2.data[y][x]);
            }
            std::cout << "\n";
        }
    }

    template<typename TPrint, typename T, typename TReturn>
    void print_array2_2(const Array2<T>& array2, const std::function<TReturn(const T&)> & print_func)
    {
        for (size_t y = 0; y < array2.size.y; y++)
        {
            for (size_t x = 0; x < array2.size.x; x++)
            {
                std::cout << static_cast<TPrint>(print_func(array2.data[y][x]));
            }
            std::cout << "\n";
        }
    }

    using Array2ui8 = Array2<uint8_t>;

}

#endif
