#pragma once

#include <string>

#define MANAGER_DECLARATION(Class) \
public: \
    static Class& Instance() \
    { \
        static Class instance; \
        return instance; \
    } \
    Class() = default; \
    void Init(); \
    ~Class(); \
    Class(const Class&) = delete; \
    Class& operator = (const Class&) = delete; \
