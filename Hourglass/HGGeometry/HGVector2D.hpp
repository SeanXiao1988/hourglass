/**
 *  @file    HGVector2D.hpp
 *  @brief   2D Vector template
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2012/11/23
 *  Company:  SNSTEAM.inc
 *  (C) Copyright 2012 SNSTEAM.inc All rights reserved.
 * 
 * This file is a part of Hourglass Engine Project.
 *
 * The copyright to the contents herein is the property of SNSTEAM.inc
 * The contents may be used and/or copied only with the written permission of
 * SNSTEAM.inc or in accordance with the terms and conditions stipulated in
 * the agreement/contract under which the contents have been supplied.
 * =====================================================================================
 */

#ifndef HGVECTOR2D_HPP_
#define HGVECTOR2D_HPP_

#include "HGSystem.h"

HGNAMESPACE_START
    
template<typename Type>
class Vector2D
{
public:
    
    // default constructor
    Vector2D()
    {
        setValue(0, 0);
    }
    
    // constructor with initial values from array
    Vector2D(const Type v[2])
    {
        setValue(v);
    }
    
    // constructor with initial values
    Vector2D(Type _x, Type _y)
    {
        setValue(_x, _y);
    }
    
    // copy constructor
    Vector2D(const Vector2D<Type>& vec)
    {
        setValue(vec[0], vec[1]);
    }
    
    // destructor
    virtual ~Vector2D() {}
    
    // set new x, y with values from array
    Vector2D<Type>& setValue(const Type v[2])
    {
        x = v[0];
        y = v[1];
        
        return *this;
    }
    
    // set new x, y with values
    Vector2D<Type>& setValue(Type _x, Type _y)
    {
        x = _x;
        y = _y;
        
        return *this;
    }
    
    // returns pointer to array
    const Type* getValue() const
    {
        return &x;
    }
    
    Type dot(const Vector2D<Type>& vec) const
    {
        return (x * vec[0] + y * vec[1]);
    }
    
    Type length() const
    {
        return (Type)std::sqrt(sqrLength());
    }
    
    Type sqrLength() const
    {
        return (x * x + y * y);
    }
    
    Type normalize()
    {
        Type magnitude = length();
        
        if (magnitude != 0.0)
            (*this) *= (Type)(1.0 / magnitude);
        else
            setValue(0.0, 0.0);
        
        return magnitude;
    }
    
    Type cross(const Vector2D<Type>& vec)
    {
        return (x * vec[1] - y * vec[0]);
    }
    
    void negate()
    {
        setValue(-x, -y);
    }
    
    Type& operator [](int i)
    {
        return (i == 0) ? x : y;
    }
    
    const Type& operator [](int i) const
    {
        return (i == 0) ? x : y;
    }
    
    Vector2D<Type>& operator *=(const Type d)
    {
        x *= d;
        y *= d;
        
        return *this;
    }
    
    Vector2D<Type>& operator /=(const Type d)
    {
        Type inv = 1.0/d;
        
        x *= inv;
        y *= inv;
        
        return *this;
    }
    
    Vector2D<Type>& operator *=(const Vector2D<Type>& vec)
    {
        x *= vec.x;
        y *= vec.y;
        
        return *this;
    }
    
    Vector2D<Type>& operator +=(const Vector2D<Type>& vec)
    {
        x += vec.x;
        y += vec.y;
        
        return *this;
    }
    
    Vector2D<Type>& operator -=(const Vector2D<Type>& vec)
    {
        x -= vec.x;
        y -= vec.y;
        
        return *this;
    }
    
    Vector2D<Type> operator -() const
    {
        return Vector2D<Type>(-x, -y);
    }
    
    friend Vector2D<Type> operator *(const Vector2D<Type>& vec, const Type d)
    {
        return Vector2D<Type>(vec.x * d, vec.y * d);
    }
    
    friend Vector2D<Type> operator *(const Type d, const Vector2D<Type>& vec)
    {
        return vec * d;
    }
    
    friend Vector2D<Type> operator /(const Vector2D<Type>& vec, const Type d)
    {
        return Vector2D<Type>(vec.x/d, vec.y/d);
    }
    
    friend Vector2D<Type> operator *(const Vector2D<Type>& vec1, const Vector2D<Type>& vec2)
    {
        return Vector2D<Type>(vec1.x * vec2.x, vec1.y * vec2.y);
    }
    
    friend Vector2D<Type> operator +(const Vector2D<Type>& vec1, const Vector2D<Type>& vec2)
    {
        return Vector2D<Type>(vec1.x + vec2.x, vec1.y + vec2.y);
    }
    
    friend Vector2D<Type> operator -(const Vector2D<Type>& vec1, const Vector2D<Type>& vec2)
    {
        return Vector2D<Type>(vec1.x - vec2.x, vec1.y - vec2.y);
    }
    
    friend bool operator ==(const Vector2D<Type>& vec1, const Vector2D<Type>& vec2)
    {
        return (vec1.x == vec2.x && vec1.y == vec2.y);
    }
    
    friend bool operator !=(const Vector2D<Type>& vec1, const Vector2D<Type>& vec2)
    {
        return !(vec1 == vec2);
    }
    
    bool equals(const Vector2D<Type> vec, const Type tolerance=1E-2) const
    {
        return ( (*this - vec).sqrLength() <= tolerance * tolerance );
    }
    
    static Vector2D<Type> max()
    {
        return Vector2D<Type>(std::numeric_limits<Type>::max(), std::numeric_limits<Type>::max());
    }
    
    static Vector2D<Type> min()
    {
        return Vector2D<Type>(std::numeric_limits<Type>::min(), std::numeric_limits<Type>::min());
    }
    
    
    Type x;
    Type y;
};

typedef Vector2D<int>       Vector2Di;
typedef Vector2D<float>     Vector2Df;
typedef Vector2D<double>    Vector2Dd;
typedef Vector2D<int32_t>   Vector2Di32;
typedef Vector2D<uint32_t>  Vector2Du32;
    
typedef Vector2Di           Point2i;
typedef Vector2Df           Point2f;
typedef Vector2Dd           Point2d;
typedef Vector2Di32         Point2i32;
typedef Vector2Du32         Point2u32;
    
HGNAMESPACE_END

#endif // HGVECTOR2D_HPP_
