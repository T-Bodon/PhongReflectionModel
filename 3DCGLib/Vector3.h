#pragma once
#ifndef VECTOR3_H
#define VECTOR3_H
#include <cmath>
#include <cfloat>

namespace Lib
{
    class Vector3
    {
    public:
        float x;
        float y;
        float z;

        // �f�t�H���g�R���X�g���N�^
        Vector3();

        // �R�s�[�R���X�g���N�^
        Vector3(const Vector3 &other);

        // �f�X�g���N�^
        ~Vector3();

        // �R���X�g���N�^
        Vector3(const float _x, const float _y, const float _z);

        // �w����W�Ɉړ�
        void move(const float _x, const float _y, const float _z);
        void move(const Vector3 &other);

        // ���W�����������炷
        void translate(const float _x, const float _y, const float _z);
        void translate(const Vector3 &vec);

        // ���ς����߂�
        float dot(const Vector3 &other) const;
        
        // �O�ς����߂�
        Vector3 cross(const Vector3 &other) const;

        // ���������߂�
        float length() const;

        // ���������߂�
        float distance(const Vector3 &other) const;

        // ���K������
        Vector3 normalize() const;

        // ���Z�q�I�[�o�[���[�h
        Vector3 & operator+=(const Vector3 &other)
        {
            return *this = *this + other;
        }
        Vector3 & operator-=(const Vector3 &other)
        {
            return *this = *this - other;
        }
        Vector3 & operator*=(const float scalar)
        {
            return *this = *this / scalar;
        }
        Vector3 & operator/=(const float scalar)
        {
            return *this = *this / scalar;
        }
        const Vector3 operator+(const Vector3 &other) const
        {
            return Vector3(x + other.x, y + other.y, z + other.z);
        }
        const Vector3 operator-(const Vector3 &other) const
        {
            return Vector3(x - other.x, y - other.y, z - other.z);
        }
        const Vector3 operator*(const float scalar) const
        {
            return Vector3(x * scalar, y * scalar, z * scalar);
        }
        const Vector3 operator/(const float scalar) const
        {
            return Vector3(x / scalar, y / scalar, z / scalar);
        }
        const Vector3 operator-() const
        {
            return Vector3(-x, -y, -z);
        }
        bool operator==(const Vector3 &other) const
        {
            return (x == other.x) && (y == other.y) && (z == other.z);
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

}


#endif