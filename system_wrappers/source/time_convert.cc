// An OS-independent sleep function.
#include "system_wrappers/interface/time_convert.h"

namespace gn
{
	static uint32_t GetDayFromMonth(int year,int month)
	{
		static int MonthArray[] = {0,31,59,90,120,151,181,212,243,273,304,334};
		uint32_t monthDay = MonthArray[month%12];
		if (month>1 && (year%4) == 0)
		{
			monthDay++;
		}
		return monthDay;
	}

	static uint32_t GetDayFromYear(int endYear)
	{
		uint32_t count1 = (endYear-1968)/4;
		uint32_t count2 = (endYear-1968)%4;
		return ((366 + 365 * 3) * count1) + (count2 - 1) * 365; 
	}

	const char* ConvertLocalSecondsToUtc0TimeStr(time_t bjSeconds)
	{
		static char szTime[256] = {0};
		memset(szTime, 0 ,256);

		struct tm* pTime = gmtime(&bjSeconds);
#ifdef _MSC_VER
		_snprintf_s(szTime, sizeof(szTime), "%d-%02d-%02dT%02d:%02d:%02d.000Z",
			1900 + pTime->tm_year, 1 + pTime->tm_mon, pTime->tm_mday, pTime->tm_hour, pTime->tm_min, pTime->tm_sec);
#else
        snprintf(szTime, sizeof(szTime), "%d-%02d-%02dT%02d:%02d:%02d.000Z",
                    1900 + pTime->tm_year, 1 + pTime->tm_mon, pTime->tm_mday, pTime->tm_hour, pTime->tm_min, pTime->tm_sec);
#endif
		return szTime;
	}

	uint64_t ConvertBjTimeStringToUtc(std::string strTime,int strType)
	{
		// conver time string to struct tm
		tm stTime = {0};
		int32_t nHourDelta = 0;
		int32_t nMinDelta = 0;
		std::string strFormat;
		switch(strType)
		{
		case 1:
			{
				// e.g. 2014-06-16T11:06:48+08:00
				strFormat = "%d-%d-%dT%d:%d:%d+%d:%d";
			}
			break;
		case 2:
		case 3:
			{
				// e.g. 2016-11-04 14:42:31:000
				strFormat = "%d-%02d-%02d %02d:%02d:%02d";
			}
			break;
		case 4:
			{
				// e.g. 2017-06-30T21:30:00+0800
				strFormat = "%d-%d-%dT%d:%d:%d+%02d%02d";
			}
			break;
		}
#ifdef _MSC_VER
		sscanf_s(strTime.c_str(),strFormat.c_str(),
#else
		sscanf(strTime.c_str(),strFormat.c_str(),
#endif
			&stTime.tm_year,
			&stTime.tm_mon,
			&stTime.tm_mday,
			&stTime.tm_hour,
			&stTime.tm_min,
			&stTime.tm_sec,
			&nHourDelta,
			&nMinDelta);
		static uint64_t secPerDay = 86400;
		static uint64_t secPerHour = 3600;
		stTime.tm_mon--;
		stTime.tm_isdst = 0;
		stTime.tm_min -= nMinDelta;
		uint64_t totalDay = ((stTime.tm_year > 1970) ? GetDayFromYear(stTime.tm_year - 1) : 0)
			+ GetDayFromMonth(stTime.tm_year,stTime.tm_mon)+stTime.tm_mday - 1;
		uint64_t timeUTC = totalDay * secPerDay + stTime.tm_hour * secPerHour + stTime.tm_min * 60 + stTime.tm_sec;
        timeUTC -= (nHourDelta * 3600 + nMinDelta * 60);
		return timeUTC;
	}

	const char* ConvertUtcToBjTimeStr(time_t utcTime,int strType)
	{
		static char szTime[256] = {0};
		memset(szTime,20*sizeof(char),256);
		if (strType == 1)
		{
			utcTime += 8 * 3600;
		}
		struct tm* pTime = gmtime(&utcTime);
		std::string strFormat;
		switch(strType)
		{
		case 1:
			{
				// e.g. 2014-06-16T11:06:48+08:00
				strFormat = "%d-%d-%dT%d:%d:%d+%d:%d";
			}
			break;
		case 2:
			{
				// e.g. 2016-11-04 14:42:31:000
				strFormat = "%d-%02d-%02d %02d:%02d:%02d:%03d";
			}
			break;
		}
		if (pTime)
		{
#ifdef _MSC_VER
			_snprintf_s(szTime, 255, strFormat.c_str(), 
				1900 + pTime->tm_year, 1 + pTime->tm_mon, pTime->tm_mday, pTime->tm_hour, pTime->tm_min, pTime->tm_sec, 0);
#else
			snprintf(szTime, 255, strFormat.c_str(), 
				1900 + pTime->tm_year, 1 + pTime->tm_mon, pTime->tm_mday, pTime->tm_hour, pTime->tm_min, pTime->tm_sec, 0);
#endif
		}
		else
		{
#ifdef _MSC_VER
			_snprintf_s(szTime, 255,strFormat.c_str(), 1970, 1, 1, 0, 0, 0, 0);
#else
			snprintf(szTime, 255, strFormat.c_str(), 1970, 1, 1, 0, 0, 0, 0);
#endif
		}
		return szTime;
	}

    int GetUtcOffsetHours()
    {
        time_t zero = 24*60*60L;
        struct tm * timeptr;
        int gmtime_hours;
        
        /* get the local time for Jan 2, 1900 00:00 UTC */
        timeptr = localtime( &zero );
        gmtime_hours = timeptr->tm_hour;
        
        /* if the local time is the "day before" the UTC, subtract 24 hours
           from the hours to get the UTC offset */
        if( timeptr->tm_mday < 2 )
            gmtime_hours -= 24;
        
        return gmtime_hours;
    }

} // namespace gn
