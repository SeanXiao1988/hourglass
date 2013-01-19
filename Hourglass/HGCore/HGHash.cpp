/**
 *  @file    HGHash.cpp
 *  @brief   
 *
 *  @author  Master.G (MG), mg@snsteam.com
 *
 *  @internal
 *  Created:  2012/06/15
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

#include "HGHash.h"

HGNAMESPACE_START

#define HASH_INIT		0x811C9DC5
#define HASH_PRIME		0x01000193
#define CONVERT_BACKSLASH
#define CASE_INSENSITIVE
    
HashStringMap Hash::mHashedStrings;

Hash::Hash()
	: mHashValue(INVALID_HASH)
{
}

Hash::Hash(uint32_t hashValue)
	: mHashValue(hashValue)
{
}

Hash::Hash(const Hash &rhs)
	:mHashValue(rhs.mHashValue)
{
}

Hash::Hash(const char *szString)
{
	mHashValue = makeHash(szString);
}

Hash::Hash(const std::string &str)
{
	mHashValue = makeHash(str.c_str());
}

Hash &Hash::operator=(const Hash &rhs)
{
	if (&rhs != this)
	{
		mHashValue = rhs.mHashValue;
	}

	return *this;
}

bool Hash::isValid() const
{
	return (mHashValue != INVALID_HASH);
}

uint32_t Hash::makeHash(const char *szString)
{
	if (!szString || !szString[0])
		return INVALID_HASH;

	const unsigned char* string = (const unsigned char*)szString;
	uint32_t hash = HASH_INIT;

	while (*string)
	{
		hash *= HASH_PRIME;

		char c = *string++;

#ifdef CONVERT_BACKSLASH
		if (c == '\\')
		{
			c = '/';
		}
#endif

#ifdef CASE_INSENSITIVE
		if ((c >= 'a') && (c <= 'z'))
		{
			c -= 'a' - 'A';
		}
#endif
		hash ^= (uint32_t)c;
	}

    std::string strToMap(szString);
	mHashedStrings.insert(HashStringMap::value_type(hash, strToMap));
    
    return hash;
}

void Hash::set(const uint32_t hash)
{
	mHashValue = hash;
}

const bool Hash::operator < (const Hash &rhs) const
{
	return mHashValue < rhs.mHashValue;
}

const bool Hash::operator > (const Hash &rhs) const
{
	return mHashValue > rhs.mHashValue;
}

const bool Hash::operator <= (const Hash &rhs) const
{
	return mHashValue <= rhs.mHashValue;
}

const bool Hash::operator >= (const Hash &rhs) const
{
	return mHashValue >= rhs.mHashValue;
}

const bool Hash::operator == (const Hash &rhs) const
{
	return mHashValue == rhs.mHashValue;
}

const bool Hash::operator==( uint32_t value ) const
{
	return mHashValue == value;
}

const bool Hash::operator != (const Hash &rhs) const
{
	return (!(*this == rhs));
}

const bool Hash::operator!=( uint32_t value ) const
{
	return mHashValue != value;
}

std::string Hash::StringForHash(uint32_t hash)
{
	std::string ret = "";
	HashStringMap::iterator iter = mHashedStrings.find(hash);
	if (iter != mHashedStrings.end())
	{
		return iter->second;
	}

	return "";
}

HGNAMESPACE_END