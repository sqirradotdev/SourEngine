#pragma once

#include <string>

#include "util/Error.h"

template <class T>
class Manager
{
protected:
    static T* m_instance;
    static std::string m_name;

    Manager() = default;
    virtual Error InternalInit() = 0;
    virtual ~Manager() = 0;
public:
    inline static T* GetInstance() { return m_instance; }
    inline static std::string& GetName() { return m_name; }

    static Error Init();
    static void Shutdown();
};

#include "Manager.tpp"

#define MANAGER_DECLARATION(Class) \
protected: \
    template <class T> \
    friend class Manager; \
    Class() = default; \
    virtual Error InternalInit(); \
public: \
    virtual ~Class();

#define MANAGER_DEFINITION(Class) \
Class* Class::m_instance = nullptr; \
std::string Class::m_name = #Class; \
