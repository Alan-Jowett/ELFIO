// Copyright (C) 2024 ELFIO contributors
// SPDX-License-Identifier: MIT

#pragma once

#include <cstdint>

// Safe integer class that is a drop-in replacement for the standard integer types, but with checks for overflow.
// The class is used to prevent integer overflow in ELFIO library.
// The class is not intended to be used in other projects.
template <typename T>
struct elf_integer {
    T value;

    constexpr elf_integer() = default;
    constexpr elf_integer( T value_ ) : value( value_ ) {}

    constexpr elf_integer( const elf_integer& other ) = default;
    constexpr elf_integer& operator=( const elf_integer& other ) = default;

    constexpr elf_integer( elf_integer&& other ) = default;
    constexpr elf_integer& operator=( elf_integer&& other ) = default;

    template <typename U>
    constexpr elf_integer( const elf_integer<U>& other ) : value( other.value ) {
        if ( value > std::numeric_limits<T>::max() ) {
            value = std::numeric_limits<T>::max();
        } else if ( value < std::numeric_limits<T>::min() ) {
            value = std::numeric_limits<T>::min();
        }
    }

    elf_integer& operator+=( const elf_integer& other ) {
        if ( other.value > 0 && value > std::numeric_limits<T>::max() - other.value ) {
            value = std::numeric_limits<T>::max();
        } else if ( other.value < 0 && value < std::numeric_limits<T>::min() - other.value ) {
            value = std::numeric_limits<T>::min();
        } else {
            value += other.value;
        }
        return *this;
    }

    elf_integer& operator-=( const elf_integer& other ) {
        if ( other.value > 0 && value < std::numeric_limits<T>::min() + other.value ) {
            value = std::numeric_limits<T>::min();
        } else if ( other.value < 0 && value > std::numeric_limits<T>::max() + other.value ) {
            value = std::numeric_limits<T>::max();
        } else {
            value -= other.value;
        }
        return *this;
    }

    elf_integer& operator*=( const elf_integer& other ) {
        if ( value == 0 || other.value == 0 ) {
            value = 0;
        } else if ( value > 0 && other.value > 0 && value > std::numeric_limits<T>::max() / other.value ) {
            value = std::numeric_limits<T>::max();
        } else if ( value > 0 && other.value < 0 && value > std::numeric_limits<T>::min() / other.value ) {
            value = std::numeric_limits<T>::min();
        } else if ( value < 0 && other.value > 0 && value < std::numeric_limits<T>::min() / other.value ) {
            value = std::numeric_limits<T>::min();
        } else if ( value < 0 && other.value < 0 && value < std::numeric_limits<T>::max() / other.value ) {
            value = std::numeric_limits<T>::max();
        } else {
            value *= other.value;
        }
        return *this;
    }

    elf_integer& operator/=( const elf_integer& other ) {
        if ( other.value == 0 ) {
            value = std::numeric_limits<T>::max();
        } else if ( value == std::numeric_limits<T>::min() && other.value == -1 ) {
            value = std::numeric_limits<T>::max();
        } else {
            value /= other.value;
        }
        return *this;
    }

    elf_integer& operator%=( const elf_integer& other ) {
        if ( other.value == 0 ) {
            value = std::numeric_limits<T>::max();
        } else if ( value == std::numeric_limits<T>::min() && other.value == -1 ) {
            value = 0;
        } else {
            value %= other.value;
        }
        return *this;
    }

    elf_integer& operator++() {
        if ( value < std::numeric_limits<T>::max() ) {
            ++value;
        }
        return *this;
    }

    elf_integer operator++( int ) {
        elf_integer tmp = *this;
        if ( value < std::numeric_limits<T>::max() ) {
            ++value;
        }
        return tmp;
    }

    elf_integer& operator--() {
        if ( value > std::numeric_limits<T>::min() ) {
            --value;
        }
        return *this;
    }

    elf_integer operator--( int ) {
        elf_integer tmp = *this;
        if ( value > std::numeric_limits<T>::min() ) {
            --value;
        }
        return tmp;
    }

    elf_integer operator+( const elf_integer& other ) const {
        elf_integer tmp = *this;
        tmp += other;
        return tmp;
    }

    elf_integer operator-( const elf_integer& other ) const {
        elf_integer tmp = *this;
        tmp -= other;
        return tmp;
    }

    elf_integer operator*( const elf_integer& other ) const {
        elf_integer tmp = *this;
        tmp *= other;
        return tmp;
    }

    elf_integer operator/( const elf_integer& other ) const {
        elf_integer tmp = *this;
        tmp /= other;
        return tmp;
    }

    elf_integer operator%( const elf_integer& other ) const {
        elf_integer tmp = *this;
        tmp %= other;
        return tmp;
    }

    elf_integer operator-() const {
        elf_integer tmp = *this;
        if ( tmp.value != std::numeric_limits<T>::min() ) {
            tmp.value = -tmp.value;
        }
        return tmp;
    }

    bool operator==( const elf_integer& other ) const { return value == other.value; }
    bool operator!=( const elf_integer& other ) const { return value != other.value; }
    bool operator<( const elf_integer& other ) const { return value < other.value; }
    bool operator>( const elf_integer& other ) const { return value > other.value; }
    bool operator<=( const elf_integer& other ) const { return value <= other.value; }
    bool operator>=( const elf_integer& other ) const { return value >= other.value; }

    static elf_integer min() { return std::numeric_limits<T>::min(); }
    static elf_integer max() { return std::numeric_limits<T>::max(); }


};