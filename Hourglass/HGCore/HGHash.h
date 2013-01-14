/**
 *  @file    HGHash.h
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

#ifndef HGHASH_H_
#define HGHASH_H_

#include "HGSystem.h"

namespace HG
{
    
#define INVALID_HASH	0xFFFFFFFF
    
typedef std::map<uint32_t, std::string> HashStringMap;
    
class Hash
{
public:
	explicit Hash();
	explicit Hash(uint32_t hashValue);
	explicit Hash(const char *szString);
	explicit Hash(const std::string &str);
	explicit Hash(const Hash &rhs);
	Hash& operator= (const Hash &rhs);

	void set(const uint32_t hash);

	uint32_t hashValue(void)	{ return mHashValue; }

	bool isValid() const;
	operator uint32_t()		{ return mHashValue; }


	static std::string StringForHash(uint32_t hash);

	const bool operator<(const Hash &rhs) const;
	const bool operator>(const Hash &rhs) const;
	const bool operator<=(const Hash &rhs) const;
	const bool operator>=(const Hash &rhs) const;
	const bool operator==(const Hash &rhs) const;
	const bool operator==(const uint32_t value) const;
	const bool operator!=(const uint32_t value) const;
	const bool operator!=(const Hash &rhs) const;

private:
    static HashStringMap mHashedStrings;
	
private:
	uint32_t makeHash(const char *szString);
	
	uint32_t mHashValue;
};

}

#endif // HGHASH_H_
