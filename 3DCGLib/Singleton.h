#pragma once
#ifndef SINGLETON_H
#define SINGLETON_H
#include <memory>

namespace Lib
{
    template<class T>
    class Singleton
    {
    public:
        static T& getInstance()
        {
            static typename T::singleton_pointer_type s_singleton(T::createInstance());
            return getReference(s_singleton);
        }

    private:
        typedef std::unique_ptr<T> singleton_pointer_type;
        // �C���X�^���X�̐���
        inline static T *createInstance() { return new T(); }
        // �C���X�^���X�̎擾
        inline static T &getReference(const singleton_pointer_type &ptr)
        {
            return *ptr;
        }

    protected:
        Singleton() {}

    private:
        // �R�s�[�̋֎~
        Singleton(const Singleton &) = delete;
        Singleton& operator=(const Singleton &) = delete;
        Singleton(Singleton&&) = delete;
        Singleton& operator=(Singleton &&) = delete;
    };
}

#endif