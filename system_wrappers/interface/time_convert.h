/////////////////////////////////////////////////////////////////////////////
// Copyright(c) 2012-2015 hujiang All Rights Reserved
// Name:        time_convert.h
// Purpose:     
// Author:      xct
// Modified by: 
// Created:     2016/11/3 15:27
// RCS-ID:      
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef GN_SYSTEM_WRAPPERS_INTERFACE_TIME_CONVERT_H_
#define GN_SYSTEM_WRAPPERS_INTERFACE_TIME_CONVERT_H_

#include <string>
#include <time.h>
#include "typedefs.h"

namespace gn
{
	const char* ConvertLocalSecondsToUtc0TimeStr(time_t bjSeconds);
	uint64_t ConvertBjTimeStringToUtc(std::string strTime,int strType);
	const char* ConvertUtcToBjTimeStr(time_t utcTime,int strType);
    int GetUtcOffsetHours();

}  // namespace gn

#endif  // GN_SYSTEM_WRAPPERS_INTERFACE_TIME_CONVERT_H_
