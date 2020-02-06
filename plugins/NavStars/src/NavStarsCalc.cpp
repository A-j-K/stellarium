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

#include "StelUtils.hpp"
#include "NavStarsCalc.hpp"

using std::sin;
using std::cos;
using std::tan;
using std::asin;
using std::acos;
using std::fmod;
using std::atan2;

NavStarsCalc::NavStarsCalc()
{}

NavStarsCalc::~NavStarsCalc()
{}

void NavStarsCalc::execute(const NavStarsCalcInputs& inputs, NavStarsCalcOutputs& outputs)
{
	double latitudeRad = 0.;
	outputs.HiDeg = RAD2DEG(inputs.altitudeRad);
	outputs.decRad = inputs.declinationRad;
	outputs.latRad = DEG2RAD(inputs.latitudeDeg);
	outputs.lonRad = DEG2RAD(inputs.longitudeDeg);
	outputs.gmstDeg = gmstDeg(inputs.julianDate);
	outputs.gmstRad = DEG2RAD(outputs.gmstDeg);
	outputs.lmstDeg = wrap360(outputs.gmstDeg + inputs.longitudeDeg);
	outputs.lmstRad = DEG2RAD(outputs.lmstDeg);
	outputs.shaRad = (2. * M_PI) - inputs.rightAscensionRad;
	outputs.shaDeg = RAD2DEG(outputs.shaRad);
	outputs.ghaRad = wrap2pi(outputs.gmstRad + outputs.shaRad);
	outputs.ghaDeg = RAD2DEG(outputs.ghaRad);
	outputs.lhaRad = wrap2pi(outputs.ghaRad + DEG2RAD(inputs.longitudeDeg));
	outputs.lhaDeg = RAD2DEG(outputs.lhaRad);
	outputs.HoRad = reverseRefractionRad(inputs.altitudeRad);
	outputs.HoDeg = RAD2DEG(outputs.HoRad);
	latitudeRad = DEG2RAD(inputs.latitudeDeg);
	outputs.HcRad = asin((sin(inputs.declinationRad) * sin(latitudeRad)) + 
					(cos(inputs.declinationRad) * cos(latitudeRad) * cos(outputs.lhaRad)));
	outputs.HcDeg = RAD2DEG(outputs.HcRad);
	outputs.ZnRad = (sin(inputs.declinationRad) * cos(latitudeRad)) - 
		            (cos(inputs.declinationRad) * sin(latitudeRad) * cos(outputs.lhaRad));
	outputs.ZnRad = acos(outputs.ZnRad / cos(outputs.HcRad));

	if (outputs.lhaRad < M_PI) 
		outputs.ZnRad = (2 * M_PI) - outputs.ZnRad;

	outputs.ZnDeg = RAD2DEG(outputs.ZnRad);
}

QString NavStarsCalcOutputs::radToDm(double rad, const QString pos, const QString neg)
{
	QString rval;
	bool sign;
	double s, md;
	unsigned int d, m;
	StelUtils::radToDms(rad, sign, d, m, s);
	md = static_cast<double>(m);
	md += (s / 60.);
	rval += (sign ? pos : neg)
		+ QString::number(d, 'f', 0)
		+ QString("&deg;")
		+ QString::number(md, 'f', 1)
		+ "'";
	return rval;
}

double NavStarsCalc::reverseRefractionRad(double h)
{
	// Sæmundsson inverse formula for determining refraction
	// https://en.wikipedia.org/wiki/Atmospheric_refraction
	double x;
	x = h + (DEG2RAD(10.3) / (h + DEG2RAD(5.11)));
	x = cos(x) / sin(x);
	x = DEG2RAD(1.02) * x / 60.;
	return h + x;
}

double NavStarsCalc::gmstDeg(double jd)
{
	    double sidereal;
        double T  = jd - 2451545.0;
        double T1 = T / 36525.0;
        double T2 = T1 * T1;
        double T3 = T2 * T1;
         
        // Calculate gmst angle. 
        sidereal = 280.46061837 + (360.98564736629 * T) + (0.000387933 * T2) - (T3 / 38710000.0);
     
        // Convert to degrees.
        sidereal = fmod(sidereal, 360.0);
        if (sidereal < 0.0) sidereal += 360.0;
     
		return sidereal;
}

double NavStarsCalc::julianDayNumber(int y, int m, int d) 
{
	return (double)(1461 * (y + 4800 + (m - 14) / 12)) / 
				4 + (367 * (m - 2 - 12 * ((m - 14) / 12))) / 12 - (3 * ((y + 4900 + (m - 14) / 12 ) / 100)) / 4 + d - 32075;
}

double NavStarsCalc::julianDate(int y, int m, int d, int h, int i, double s) 
{
    double hour   = (double)h;
    double minute = (double)i;
	double second = (double)s;                                   
	double jd = std::floor(julianDayNumber(y, m, d));
	double adjust = ((hour - 12.) / 24.) + (minute / 1440.) + (second / 86400.);
	jd += adjust;                
    return jd;
}



double NavStarsCalc::wrap2pi(double d)
{
	if (d > (2. * M_PI)) d -= (2 * M_PI);
	else if (d < 0.) d += (2 * M_PI);
	return d;
}

double NavStarsCalc::wrap360(double d)
{
	if (d > 360.) d -= 360.;
	else if (d < 0.) d += 360.;
	return d;
}
