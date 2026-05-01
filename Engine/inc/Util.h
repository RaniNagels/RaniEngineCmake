#pragma once
#include <type_traits>
#include <utility>

// contains C++23 standarrd library functions, that the emscripten library has yet to implement
namespace Util
{
    template<typename T>
    concept Enum = std::is_enum_v<T>;

    template<Enum E>
    constexpr uint8_t to_underlying(E e) noexcept
    {
#if defined(_WIN32)
        return std::to_underlying(e);
#else
        return static_cast<uint8_t>(e);
#endif
    }
}