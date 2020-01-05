/*
 * Copyright (C) 2012 Alexander Wolf
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

#ifndef ASTRONAVSTAR_HPP
#define ASTRONAVSTAR_HPP

#include <QVariant>
#include <QString>
#include <QStringList>
#include <QFont>
#include <QList>
#include <QDateTime>

#include "StelObject.hpp"
#include "StelTextureTypes.hpp"
#include "StelFader.hpp"

//! @ingroup exoplanets
typedef struct
{
	int starid;
	QString planetName;		//! AstroNavStar designation
	QString planetProperName;	//! AstroNavStar proper name
	double mass;				//! AstroNavStar mass (Mjup)
	double radius;				//! AstroNavStar radius (Rjup)
	double period;				//! AstroNavStar period (days)
	double semiAxis;				//! AstroNavStar orbit semi-major axis (AU)
	double eccentricity;			//! AstroNavStar orbit eccentricity
	double inclination;			//! AstroNavStar orbit inclination
	double angleDistance;			//! AstroNavStar angle distance
	int discovered;				//! AstroNavStar discovered year
	QString pclass;				//! AstroNavStar classification from host star spectral type (F, G, K, M), habitable zone (hot, warm, cold) and size (miniterran, subterran, terran, superterran, jovian, neptunian)
	int EqTemp;				//! AstroNavStar equilibrium temperature in kelvins (K) assuming a 0.3 bond albedo (Earth = 255 K).
	int flux;					//! Average stellar flux of the planet in Earth fluxes (Earth = 1.0 SE).
	int ESI;					//! AstroNavStar Earth Similarity Index
	QString detectionMethod;		//! Method of detection of exoplanet
	bool conservative;			//! Conservative sample
} AstroNavStarData;

class StelPainter;

//! @class AstroNavStar
//! A exoplanet object represents one planetary system on the sky.
//! Details about the exoplanets are passed using a QVariant which contains
//! a map of data from the json file.
//! @ingroup exoplanets

class AstroNavStar : public StelObject
{
	friend class AstroNavMgr;

public:
	static const QString ASTRONAVSTAR_TYPE;

	//! @param id The official designation for a exoplanet, e.g. "Kepler-10 b"
	AstroNavStar(const QVariantMap& map);
	~AstroNavStar();

	//! Get a QVariantMap which describes the exoplanet. Could be used to
	//! create a duplicate.
	QVariantMap getMap(void) const;

	//! Get the type of object
	virtual QString getType(void) const
	{
		return ASTRONAVSTAR_TYPE;
	}

	virtual QString getID(void) const
	{
		return getDesignation();
	}

	virtual float getSelectPriority(const StelCore* core) const;

	//! Get an HTML string to describe the object
	//! @param core A pointer to the core
	//! @flags a set of flags with information types to include.
	virtual QString getInfoString(const StelCore* core, const InfoStringGroup& flags) const;
	//! Return a map like StelObject, but with a few extra tags also available in getMap().
	//! - distance = distance in pc
	//! - stype = Spectral type of star
	//! - smass = Mass of star in Msun
	//! - smetal = [Fe/H] of star
	//! - sradius = Radius of star in Rsun
	//! - effectiveTemp = Effective temperature of star in K
	//! - hasHabitablePlanets (true/false)
	virtual QVariantMap getInfoMap(const StelCore *core) const;
	virtual Vec3f getInfoColor(void) const;
	virtual Vec3d getJ2000EquatorialPos(const StelCore*) const
	{
		return XYZ;
	}
	//! Get the visual magnitude
	virtual float getVMagnitude(const StelCore* core) const;
	//! Get the angular size of host star
	virtual double getAngularSize(const StelCore* core) const;
	//! Get the localized name of host star
	virtual QString getNameI18n(void) const;
	//! Get the english name
	virtual QString getEnglishName(void) const;

	QString getDesignation(void) const;
	QStringList getAstroNavStarsEnglishNames(void) const;
	QStringList getAstroNavStarsNamesI18n(void) const;
	QStringList getAstroNavStarsDesignations(void) const;

	bool isDiscovered(const StelCore* core);

	void update(double deltaTime);

	int getCountAstroNavStars(void) const
	{
		return EPCount;
	}
	int getCountHabitableAstroNavStars(void) const
	{
		return PHEPCount;
	}

	QList<double> getData(int mode)
	{
		switch(mode)
		{
			case 1:
				return semiAxisList;
			case 2:
				return massList;
			case 3:
				return radiusList;
			case 4:
				return periodList;
			case 5:
				return angleDistanceList;
			case 6:
				return effectiveTempHostStarList;
			case 7:
				return yearDiscoveryList;
			case 8:
				return metallicityHostStarList;
			case 9:
				return vMagHostStarList;
			case 10:
				return raHostStarList;
			case 11:
				return decHostStarList;
			case 12:
				return distanceHostStarList;
			case 13:
				return massHostStarList;
			case 14:
				return radiusHostStarList;
			default:
				return eccentricityList;
		}
	}

private:
	QString getPlanetaryClassI18n(QString ptype) const;

	bool initialized;

	Vec3d XYZ;                         // holds J2000 position	

	static StelTextureSP markerTexture;
	static Vec3f habitableAstroNavStarMarkerColor;
	static Vec3f exoplanetMarkerColor;
	static bool distributionMode;
	static bool timelineMode;
	static bool habitableMode;
	static bool showDesignations;	
	static int temperatureScaleID;

	void draw(StelCore* core, StelPainter *painter);

	QString getTemperatureScaleUnit() const;
	float getTemperature(float temperature) const;

	int EPCount;
	int PHEPCount;

	//! Variables for description of properties of exoplanets
	QString designation;			//! The designation of the host star
	QString starProperName;			//! The proper name of the host star
	double RA;				//! J2000 right ascension of host star // ALMOST USELESS AFTER CONSTRUCTOR!
	double DE;				//! J2000 declination of host star     // ALMOST USELESS AFTER CONSTRUCTOR!   use XYZ
	double distance;			//! Distance to star in pc
	QString stype;				//! Spectral type of star
	double smass;				//! Mass of star in Msun
	double smetal;				//! [Fe/H] of star
	double Vmag;				//! Visual magnitude of star
	double sradius;				//! Radius of star in Rsun
	int effectiveTemp;			//! Effective temperature of star in K
	bool hasHabitableAstroNavStars;		//! Has potential habitable exoplanets
	QList<exoplanetData> exoplanets;	//! List of exoplanets

	QStringList englishNames, translatedNames, exoplanetDesignations;

	// FIXME: Document the purpose of these lists?
	QList<double> eccentricityList, semiAxisList, massList, radiusList, periodList, angleDistanceList,
		      effectiveTempHostStarList, yearDiscoveryList, metallicityHostStarList, vMagHostStarList,
		      raHostStarList, decHostStarList, distanceHostStarList, massHostStarList, radiusHostStarList;

	LinearFader labelsFader;
};

#endif // ASTRONAVSTAR_HPP
