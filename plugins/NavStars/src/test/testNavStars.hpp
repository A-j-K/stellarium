/*
 * Copyright (C) 2019 Alexander Wolf <alex.v.wolf@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Suite 500, Boston, MA  02110-1335, USA.
 */

#ifndef TESTNAVSTARS_HPP
#define TESTNAVSTARS_HPP

#include <QtTest>

#include "NavStars.hpp"

class TestNavStars : public QObject
{
	Q_OBJECT

private slots:
    void TestAgainstAlmancVega();
};

// Helper class.
struct TestTime
{
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;
    double jd;
    double jde;
    double lmst;
    double gmst;

	TestTime() :
		  year(0)
		, month(0)
		, day(0)
		, hour(0)
		, minute(0)
		, second(0)
        , jd(0)
        , jde(0)
        , lmst(0)
        , gmst(0)
	{}
    TestTime(int y, int m, int d, int h, int i, int s, double lat, double lon) :
        year(y)
        , month(m)
        , day(d)
        , hour(h)
        , minute(i)
        , second(s)
        , jde(0)
        , lmst(0)
        , gmst(0)
	{
		jd = julian_date(*this);
        sidereal_degrees(jd, lon, &gmst, &lmst);
	}

	~TestTime() {}

	static double julian_day_number(const TestTime& t) 
    {
        double jdn = (double)(1461 * ((int)t.year + 4800 
		    + ((int)t.month - 14) / 12)) / 4 + (367 * ((int)t.month - 2 
		    - 12 * (((int)t.month - 14) / 12))) / 12 - (3 * (((int)t.year + 4900 
		    + ((int)t.month - 14) / 12 ) / 100)) / 4 + (int)t.day - 32075;
        return jdn;
    }

    static double julian_date(const TestTime& t) 
    {
        double hour, minute, second, jd, adjust;
        hour   = (double)t.hour;
        minute = (double)t.minute;
	    second = (double)t.second;                                   
	    jd = std::floor(julian_day_number(t));
	    adjust = ((hour - 12.) / 24.) + (minute / 1440.) + second / 86400.;
	    jd += adjust;                
        return jd;
    }

    static void sidereal_degrees(double jd, double lon, double *gmst, double *lmst) 
    {
        double sidereal;
        double T  = jd - 2451545.0;
        double T1 = T / 36525.0;
        double T2 = T1 * T1;
        double T3 = T2 * T1;
         
        /* Calculate gmst angle. */
        sidereal = 280.46061837 + (360.98564736629 * T) + (0.000387933 * T2) - (T3 / 38710000.0);
     
        /* Convert to degrees. */
        sidereal = std::fmod(sidereal, 360.0);
        if (sidereal < 0.0) sidereal += 360.0;
     
        *(gmst) = sidereal;
        *(lmst) = *(gmst) + lon;
    }
};


#endif // TESTNAVSTARS_HPP
