/*
 * Navigational Stars plug-in
 * Copyright (C) 2020 Andy Kirkham
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

#ifndef NAVSTARSCALC_HPP
#define NAVSTARSCALC_HPP

/*! @defgroup navigationalStars Navigational Stars Plug-in
@{
The Navigational Stars plugin marks the 58 navigational stars of the
Nautical Almanach and the 2102-D Rude Star Finder on the sky.

The NavStars class is the main class of the plug-in. It manages the list of
navigational stars and manipulate show/hide markers of them. All markers
are not objects!

The plugin is also an example of a custom plugin that just marks existing stars.

<b>Configuration</b>

The plug-ins' configuration data is stored in Stellarium's main configuration
file (section [NavigationalStars]).

@}
*/

#include <cmath>

#include <QMap>
#include <QString>

#ifndef DEG2RAD
#define DEG2RAD(x) ((x * M_PI) / 180.)
#endif

#ifndef RAD2DEG
#define RAD2DEG(x) ((x * 180.) / M_PI)
#endif

struct NavStarsCalcInputs
{
    double julianDate;
    double latitudeDeg;
    double longitudeDeg;
    double altitudeRad;
    double azimuthRad;
    double rightAscensionRad;
    double declinationRad;
};

struct NavStarsCalcOutputs
{
    //! Get a string that represents degrees and decimal minutes of an angle.
	//! Format is as per the standard Nautical Almanac, +/-DDDMM.m
	//! @param double rad The angle in radians.
	//! @param QString pos Use + or N or E. 
	//! @param QString neg Use - or S or W.
	//! @return QString the representation of angle rad.
    QString radToDm(double rad, const QString pos = "+", const QString neg = "-");

    double decRad;
    QString dec() { return radToDm(decRad); }

    double latRad;
    QString lat() { return radToDm(latRad, "N", "S"); }

    double lonRad;
    QString lon() { return radToDm(lonRad, "E", "W"); }

    double HiDeg;
    double HoDeg;
    double HoRad;
    QString Ho() { return radToDm(HoRad); }

    double HcDeg;
    double HcRad;
    QString Hc() { return radToDm(HcRad); }

    double ZnDeg;
    double ZnRad;
    QString Zn() { return radToDm(ZnRad); }

    double ghaDeg;
    double ghaRad;
    QString gha() { return radToDm(ghaRad); }

    double lhaDeg;
    double lhaRad;
    QString lha() { return radToDm(lhaRad); }

    double shaDeg;
    double shaRad;
    QString sha() { return radToDm(shaRad); }

    double gmstDeg;
    double gmstRad;
    QString gmst() { return radToDm(gmstRad); }

    double lmstDeg;
    double lmstRad;
    QString lmst() { return radToDm(lmstRad); }


};


//! @class NavStarsCalc
//! @author Andy Kirkham
//! @ingroup navigationalStars
class NavStarsCalc 
{
public:	
    NavStarsCalc();
    ~NavStarsCalc();

    //! Calculate intermediates.
    void execute(const NavStarsCalcInputs& inputs, NavStarsCalcOutputs& outputs);
    
    //! Get a string that represents degrees and decimal minutes of an angle.
	//! Format is as per the standard Nautical Almanac, +/-DDDMM.m
	//! @param double rad The angle in radians.
	//! @param QString pos Use + or N or E. 
	//! @param QString neg Use - or S or W.
	//! @return QString the representation of angle rad.
    //static QString radToDm(double rad, const QString pos = "+", const QString neg = "-");

    //! Ensure the supplied angle, in radians, is with 0 to 2PI.
	//! @param double d The angle in radians.
	//! @return The wrapped value.
    static double wrap2pi(double d);

    //! Ensure the supplied angle, in degrees, is with 0 to 360.
	//! @param double d The angle in degrees.
	//! @return The wrapped value.
    static double wrap360(double d);

    //! Implementation of Bennet's atmosperic refraction.
    //! Accurate enough for general marine navigation.
    //! @param double Input angle in radians.
    //! @return double Input angle refracted in radians.
    double reverseRefractionRad(double h);
    double reverseRefractionDeg(double h) { return RAD2DEG(reverseRefractionRad(DEG2RAD(h))); }

    //! Implementation of gmst, Greenwich Mean Sidereal Time.
    //! @param double Julian Date.
    //! @param double longitude, west is negative.
    //! @return double gmst in degrees.
    double gmstDeg(double jd);
    double gmstRad(double jd) { return DEG2RAD(gmstDeg(jd)); }

    //! Implementation of lmst, Local Mean Sidereal Time.
    //! @param double Julian Date.
    //! @param double longitude in degrees, west is negative.
    //! @return double gmst.
    double lmstDeg(double jd, double lon) { return wrap360(gmstDeg(jd) + lon); }

    //! Calculate Julian Day Number from supplied args.
    //! @param int year
    //! @param int month
    //! @param int seconds
    //! @return double Julian Day Number
    double julianDayNumber(int y, int m, int d);

    //! Calculate Julian Date from supplied args.
    //! @param int year
    //! @param int month
    //! @param int seconds
    //! @param int hour
    //! @param int minute
    //! @param int second
    //! @return double Julian Date
    double julianDate(int y, int m, int d, int h, int i, double s);



};

#endif /* NAVSTARSCALC_HPP */
