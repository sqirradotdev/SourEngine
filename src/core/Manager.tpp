#include "Manager.h"

template <class T>
Manager<T>::~Manager() = default;

template <class T>
Error Manager<T>::Init()
{
    if (m_instance != nullptr)
        return FAILED;

    m_instance = new T();
    if (m_instance->InternalInit() != OK)
    {
        delete m_instance;
        return FAILED;
    }

    return OK;
}

template <class T>
void Manager<T>::Shutdown()
{
    delete m_instance;
}
