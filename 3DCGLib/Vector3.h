#pragma once
#ifndef VECTOR3_H
#define VECTOR3_H

namespace Lib
{
    template <class T>
    class Vector3
    {
    public:
        T x;
        T y;
        T z;

        // �f�t�H���g�R���X�g���N�^
        Vector3() : x(T()), y(T()), z(T()), w(T())
        {
        }

        // �R�s�[�R���X�g���N�^
        Vector3(const Vector3 &other)
        {
            x = other.x;
            y = other.y;
            z = other.z;
        }

        // �f�X�g���N�^
        ~Vector3()
        {
        }
        // �R���X�g���N�^
        Vector3(
            const T _x = static_cast<T>(0),
            const T _y = static_cast<T>(0),
            const T _z = static_cast<T>(0)
        )
        {
            x = _x;
            y = _y;
            z = _z;
        }

        // �w����W�Ɉړ�
        void move(const T _x, const T _y, const T _z = static_cast<T>(0))
        {
            x = _x;
            y = _y;
            z = _z;
        }
        void move(const Vector3 &other)
        {
            x = other.x;
            y = other.y;
            z = other.z;
        }
        // ���W�����������炷
        void translate(const T _x, const T _y, const T _z)
        {
            x += _x;
            y += _y;
            z += _z;
        }
        void translate(const T &vec)
        {
            x += vec.x;
            y += vec.y;
            z += vec.z;
        }
        // ���ς����߂�
        T dot(const Vector3 &other) const
        {
            return (x * other.x) + (y * other.y) + (z * other.z);
        }
        // �O�ς����߂�
        Vector3<T> cross(const Vector3 &other) const
        {
            return Vector3(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
        }
        // ���������߂�
        T length() const
        {
            return std::sqrt(dot(*this));
        }
        // ���������߂�
        T distance(const Vector3 &other) const
        {
            return (*this - other).length();
        }
        // ���K������
        Vector3 normalize() const
        {
            const float len = this->length();
            if (len < FLT_EPSILON) {
                return Vector3::ZERO;
            }
            return *this / len;
        }
        // ���Z�q�I�[�o�[���[�h
        Vector3 & operator+=(const Vector3 &other)
        {
            return *this = *this + other;
        }
        Vector3 & operator-=(const Vector3 &other)
        {
            return *this = *this - other;
        }
        Vector3 & operator*=(const T scalar)
        {
            return *this = *this / other;
        }
        Vector3 & operator/=(const T scalar)
        {
            return *this = *this / other;
        }
        const Vector3 operator+(const Vector3 &other) const
        {
            return Vector3(x + other.x, y + other.y, z + other.z);
        }
        const Vector3 operator-(const Vector3 &other) const
        {
            return Vector3(x - other.x, y - other.y, z - other.z);
        }
        const Vector3 operator*(const T scalar) const
        {
            return Vector3(x * scalar, y * scalar, z * scalar);
        }
        const Vector3 operator/(const T scalar) const
        {
            return Vector3(x / scalar, y / scalar, z / scalar);
        }
        const Vector3 operator-() const
        {
            return Vector3(-x, -y, -z);
        }
        bool operator==(const Vector3 &other) const
        {
            return (x == oher.x) && (y == other.y) && (z == other.z);
        }
        bool operator!=(const Vector3 &other) const
        {
            return !(*this == other);
        }
        bool operator<(const Vector3 &other) const
        {
            if (x < other.x) return true;
            if (y < other.y) return true;
            if (z < other.z) return true;
            return false;
        }

        // �萔
        static const Vector3 ZERO;
        static const Vector3 UP;
        static const Vector3 DOWN;
        static const Vector3 LEFT;
        static const Vector3 RIGHT;
        static const Vector3 FORWARD;
        static const Vector3 BACK;
    };
    template<class T>
    const Vector3<T> Vector3<T>::ZERO(static_cast<T>(0), static_cast<T>(0), static_cast<T>(0));
    template<class T>
    const Vector3<T> Vector3<T>::UP(static_cast<T>(0), static_cast<T>(1), static_cast<T>(0));
    template<class T>
    const Vector3<T> Vector3<T>::DOWN(static_cast<T>(0), static_cast<T>(-1), static_cast<T>(0));
    template<class T>
    const Vector3<T> Vector3<T>::LEFT(static_cast<T>(-1), static_cast<T>(0), static_cast<T>(0));
    template<class T>
    const Vector3<T> Vector3<T>::RIGHT(static_cast<T>(1), static_cast<T>(0), static_cast<T>(0));
    template<class T>
    const Vector3<T> Vector3<T>::FORWARD(static_cast<T>(0), static_cast<T>(0), static_cast<T>(1));
    template<class T>
    const Vector3<T> Vector3<T>::BACK(static_cast<T>(0), static_cast<T>(0), static_cast<T>(-1));
}


#endif