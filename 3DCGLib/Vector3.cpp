#include "Vector3.h"

namespace Lib
{
    // �f�t�H���g�R���X�g���N�^
    Vector3::Vector3()
    {
        *this = Vector3::ZERO;
    }

    // �R�s�[�R���X�g���N�^
    Vector3::Vector3(const Vector3 &other)
    {
        *this = other;
    }

    // �f�X�g���N�^
    Vector3::~Vector3()
    {
    }
    // �R���X�g���N�^
    Vector3::Vector3(
        const float _x,
        const float _y,
        const float _z
    )
    {
        x = _x;
        y = _y;
        z = _z;
    }

    // �w����W�Ɉړ�
    void Vector3::move(const float _x, const float _y, const float _z)
    {
        x = _x;
        y = _y;
        z = _z;
    }
    void Vector3::move(const Vector3 &other)
    {
        x = other.x;
        y = other.y;
        z = other.z;
    }
    // ���W�����������炷
    void Vector3::translate(const float _x, const float _y, const float _z)
    {
        x += _x;
        y += _y;
        z += _z;
    }
    void Vector3::translate(const Vector3 &vec)
    {
        x += vec.x;
        y += vec.y;
        z += vec.z;
    }
    // ���ς����߂�
    float Vector3::dot(const Vector3 &other) const
    {
        return (x * other.x) + (y * other.y) + (z * other.z);
    }
    // �O�ς����߂�
    Vector3 Vector3::cross(const Vector3 &other) const
    {
        return Vector3(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
    }
    // ���������߂�
    float Vector3::length() const
    {
        return std::sqrt(dot(*this));
    }
    // ���������߂�
    float Vector3::distance(const Vector3 &other) const
    {
        return (*this - other).length();
    }
    // ���K������
    Vector3 Vector3::normalize() const
    {
        const float len = this->length();
        if (len < FLT_EPSILON) {
            return Vector3::ZERO;
        }
        return *this / len;
    }

    const Vector3 Vector3::ZERO   ( 0.0f,  0.0f,  0.0f);
    const Vector3 Vector3::UP     ( 0.0f,  1.0f,  0.0f);
    const Vector3 Vector3::DOWN   ( 0.0f, -1.0f,  0.0f);
    const Vector3 Vector3::LEFT   (-1.0f,  0.0f,  0.0f);
    const Vector3 Vector3::RIGHT  ( 1.0f,  0.0f,  0.0f);
    const Vector3 Vector3::FORWARD( 0.0f,  0.0f,  1.0f);
    const Vector3 Vector3::BACK   ( 0.0f,  0.0f, -1.0f);
}