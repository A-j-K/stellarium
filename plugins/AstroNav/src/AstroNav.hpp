/*
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

#ifndef ASTRONAV_HPP
#define ASTRONAV_HPP

#include "StelCore.hpp"
#include "StelObject.hpp"
#include "StelObjectModule.hpp"
#include "StelTexture.hpp"
#include "StelProjector.hpp"

#include <QDateTime>
#include <QFont>
#include <QList>
#include <QSharedPointer>
#include <QVariantMap>
#include <QVector>

#include "Almanac/AlmanacStrategy.hpp"

class StelPainter;

/*! @defgroup AstroNav AstroNavStar Plug-in
@{
The %AstroNav plugin plots the position of nomiated known navigation stars and solves the sphical triangle as per the Nautical Almanac.
@}
*/

//! @ingroup AstroNav
// typedef QSharedPointer<AstroNavStar> AstroNavStarP;

//! @class AstroNav
//! Main class of the %AstroNav plugin.
//! @author Andy Kirkham
//! @ingroup AstroNav
class AstroNav : public StelObjectModule
{
	Q_OBJECT

public:
	static const QString ASTRONAVSTAR_TYPE;

	AstroNav();
	virtual ~AstroNav();

	/////////////////////////////////////////////
	// IF Methods defined in the StelModule class
	virtual void init();
	virtual void deinit();
	virtual void update(double deltaTime);
	virtual void draw(StelCore* core);
	virtual double getCallOrder(StelModuleActionName actionName) const;

	///////////////////////////////////////////////
	// IF Methods defined in StelObjectModule class
	//! Used to get a list of objects which are near to some position.
	//! @param v a vector representing the position in th sky around which to search for exoplanets.
	//! @param limitFov the field of view around the position v in which to search for exoplanets.
	//! @param core the StelCore to use for computations.
	//! @return a list containing the exoplanets located inside the limitFov circle around position v.
	virtual QList<StelObjectP> searchAround(const Vec3d& v, double limitFov, const StelCore* core) const;

	//! Return the matching exoplanet system object's pointer if exists or Q_NULLPTR.
	//! @param nameI18n The case in-sensitive localized exoplanet system name
	virtual StelObjectP searchByNameI18n(const QString& nameI18n) const;

	//! Return the matching exoplanet system if exists or Q_NULLPTR.
	//! @param name The case in-sensitive english exoplanet system name
	virtual StelObjectP searchByName(const QString& name) const;

	//! Return the matching exoplanet system if exists or Q_NULLPTR.
	//! @param id The exoplanet system id
	virtual StelObjectP searchByID(const QString& id) const;

	//! Further interface implementations.
	virtual QString getAuthorName() const { return "Andy Kirkham"; }
	virtual QStringList listAllObjects(bool inEnglish) const;
	virtual QString getName() const;
	virtual QString getStelObjectType() const { return AstroNav::ASTRONAVSTAR_TYPE; }

	// Public API functions.
	StelTextureSP getNavstarMarker() { return markerTexture; }
	void getCelestialNavData(StelCore* core, StelObjectP stelObject, QMap<QString, double>& map);

signals:

public slots:

private:
	//! Highlight the Nav stars on the display.
	void markNavstars(StelCore* core, StelProjectorP projector, Vec3d colours = {1., 0., 0.}); 

	//! Create a list of Nav stars based on the current strategy.
	void populateNavstarPointers(const QList<int>& hipNumbers);

	//! Build the extra sinfo string.
	QString extraInfoString(StelCore* core, StelObjectP selectedObject, bool withTables);

	//! Convert an angle in radians to DM.m format.
	QString radToDm(double rad);

	//! Convert an angle in radians to a geodetic location.
	QString radToDmPos(double rad, QChar pos = 'N', QChar neg = 'S');

	//! Ensure value is between zero and 2PI
	double wrap2pi(double d);

	//! Ensure value is between zero and 360 degrees.
	double wrap360(double d);

	//! Create a single information string from two string values.
	QString oneRowTwoCells(const QString& a, const QString& b, bool w, QString c = "");	
	QString oneRowTwoCells(const QString& a, double b, bool w, QString = "");

	//! A fake method for strings marked for translation.
	//! Use it instead of translations.h for N_() strings, except perhaps for
	//! keyboard action descriptions. (It's better for them to be in a single
	//! place.)
	// static void translations();

	//! Used to render the nav star pointers.
	StelTextureSP markerTexture;

	//! List of loaded nav stars.
	QVector<StelObjectP> navstars;

private slots:
	//! Display a message. This is used for plugin-specific warnings and such
	void displayMessage(const QString& message, const QString hexColor = "#999999");
};

#endif /* ASTRONAV_HPP */
