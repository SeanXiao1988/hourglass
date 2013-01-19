/**
 *  @file    HGRectangle.hpp
 *  @brief   Rectangle
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2012/11/25
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

#ifndef HGRECTANGLE_HPP_
#define HGRECTANGLE_HPP_

#include "HGVector2D.hpp"

HGNAMESPACE_START

template<typename Type>
class Rectangle
{
public:
    Rectangle()
    {
        makeEmpty();
    }

    Rectangle(const Vector2D<Type>& vec1, const Vector2D<Type>& vec2)
    {
        setBounds(vec1, vec2);
    }

    virtual ~Rectangle() {}

    void setBounds(const Vector2D<Type>& vec1, const Vector2D<Type>& vec2)
    {
        mMin = vec1;
        mMax = vec2;
    }

    bool isEmpty() const
    {
        return (mMax[0] < mMin[0] || mMax[1] < mMin[1]);
    }

    void makeEmpty()
    {
        mMin = Vector2D<Type>::max();
        mMax = Vector2D<Type>::min();
    }

    const Vector2D<Type>& getMin() const
    {
        return mMin;
    }

    const Vector2D<Type>& getMax() const
    {
        return mMax;
    }

    Vector2D<Type> getSize() const
    {
        return mMax - mMin;
    }

    Vector2D<Type> getCenter() const
    {
        return Vector2D<Type>( (mMax[0] + mMin[0]) * 0.5, (mMax[1] + mMin[1]) * 0.5 );
    }

    void extendTo(const Vector2D<Type>& dest)
    {
        if (isEmpty())
        {
            setBounds(dest, dest);
        }
        else
        {
            if (dest[0] < mMin[0])
                mMin[0] = dest[0];
            if (dest[1] < mMin[1])
                mMin[1] = dest[1];

            if (dest[0] > mMax[0])
                mMax[0] = dest[0];
            if (dest[1] > mMax[1])
                mMax[1] = dest[1];
        }
    }

    void extendTo(const Rectangle<Type>& rect)
    {
        if (isEmpty())
        {
            *this = rect;
        }
        else
        {
            extendTo(rect.getMin());
            extendTo(rect.getMax());
        }
    }
    
    void addOffset(const Vector2D<Type>& offset)
    {
        mMin += offset;
        mMax += offset;
    }
    
    Type getLeft()
    {
        return mMin.x;
    }
    
    Type getRight()
    {
        return mMax.x;
    }
    
    Type getTop()
    {
        return mMin.y;
    }
    
    Type getBottom()
    {
        return mMax.y;
    }
    
    Type getWidth()
    {
        return mMax.x - mMin.x;
    }
    
    Type getHeight()
    {
        return mMax.y - mMin.y;
    }

    Type getArea() const
    {
        Vector2D<Type> size = mMax - mMin;
        return (size[0] * size[1]);
    }

    bool intersect(const Vector2D<Type>& point) const
    {
        return !(point[0] < mMin[0] || point[0] > mMax[0] || point[1] < mMin[1] || point[1] > mMax[1]);
    }

    bool intersect(const Rectangle<Type>& rect) const
    {
        if ((mMax[0] < rect.mMin[0]) || (mMin[0] > rect.mMax[0]) || (mMax[1] < rect.mMin[1]) || (mMin[1] > rect.mMax[1]))
            return false;
        else
            return true;
    }

    friend bool operator ==(const Rectangle<Type>& r1, const Rectangle<Type>& r2)
    {
        return ( (r1.getMin() == r2.getMin()) && (r1.getMax() == r2.getMax()));
    }

    friend bool operator !=(const Rectangle<Type>& r1, const Vector2D<Type>& r2)
    {
        return !(r1 == r2);
    }

private:
    Vector2D<Type> mMin;
    Vector2D<Type> mMax;
};

typedef Rectangle<int>      Recti;
typedef Rectangle<float>    Rectf;
typedef Rectangle<double>   Rectd;
typedef Rectangle<int32_t>  Rect32i;
typedef Rectangle<uint32_t> Rect32u;

HGNAMESPACE_END

#endif // HGRECTANGLE_HPP_
