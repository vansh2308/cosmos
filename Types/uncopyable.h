#ifndef TYPES_UNCOPYABLE_H
#define TYPES_UNCOPYABLE_H
#pragma once

namespace cosmos::Types{
    struct Uncopyable{
        explicit Uncopyable() = default;
        virtual ~Uncopyable() = default;
        Uncopyable(const Uncopyable&) = delete;
        auto operator=(const Uncopyable&) -> Uncopyable& = delete;
        Uncopyable(Uncopyable&&) = delete;
        auto operator=(Uncopyable&&) -> Uncopyable& = delete;
    };
}


#endif