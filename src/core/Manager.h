#pragma once

#include <string>

#define MANAGER_DECLARATION(Class) \
public: \
    static Class& Instance() \
    { \
        static Class instance; \
        return instance; \
    } \
    Class(); \
    ~Class(); \
    Class(const Class&) = delete; \
    Class& operator = (const Class&) = delete; \
