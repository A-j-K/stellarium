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

#include <QTextStream>

#include "LabelMgr.hpp"
#include "StelApp.hpp"

#include "StelUtils.hpp"
#include "StelFileMgr.hpp"
#include "StelModuleMgr.hpp"
#include "StelObjectMgr.hpp"
#include "StelPainter.hpp"
#include <StelMainScriptAPI.hpp>
#include "Planet.hpp"

#include "StelTextureMgr.hpp"
#include "StelTranslator.hpp"
#include <qglobal.h>

#include "planetsephems/sidereal_time.h"

#include "AstroNav.hpp"

const QString AstroNav::ASTRONAVSTAR_TYPE = QStringLiteral("Navigation Star");

AstroNav::AstroNav()
{
	;
}

AstroNav::~AstroNav()
{
	;
}

void AstroNav::init()
{
	QList<int> hipNumbers;
	hipNumbers = AstroNavAlmanac::StrategyFactory::getAlmanac()->getNavstarHIPids();
	populateNavstarPointers(hipNumbers);
	markerTexture =
	  StelApp::getInstance().getTextureManager().createTexture(":/AstroNav/nav-star-indicator.png");
}

void AstroNav::populateNavstarPointers(const QList<int>& hipNumbers)
{
	StelObjectMgr* objectmgr = nullptr;
	navstars.clear();
	objectmgr = GETSTELMODULE(StelObjectMgr);
	Q_CHECK_PTR(objectmgr);
	if (objectmgr)
	{
		int loopIndex = 0, hipNumberSize = hipNumbers.size();
		navstars.fill(StelObjectP(), hipNumberSize);
		for (QList<int>::const_iterator itor = hipNumbers.begin();
			 itor != hipNumbers.end() && loopIndex < hipNumberSize; ++itor, loopIndex++)
		{
			QString name = QString("HIP %1").arg(*itor);
			navstars[loopIndex] = objectmgr->searchByName(name);
		}
	}
}

void AstroNav::deinit()
{
	navstars.clear();
	markerTexture.clear();
}

void AstroNav::update(double deltaTime) {}

void AstroNav::draw(StelCore* core)
{
	const QSharedPointer<class Planet> p = core->getCurrentPlanet();
	if ("Earth" != p->getEnglishName()) return;
	if (0 == navstars.size()) return;

	StelProjectorP projector = core->getProjection(StelCore::FrameJ2000);
	markNavstars(core, projector);

	bool isSource = StelApp::getInstance().getStelObjectMgr().getWasSelected();

	if (isSource) 
	{
		QString output, extraText;
		QMap<QString, double> data;
		QTextStream oss(&output);
		StelObjectP selectedObject = StelApp::getInstance().getStelObjectMgr().getSelectedObject()[0];
		QString englishName = selectedObject->getEnglishName();
		getCelestialNavData(core, selectedObject, data);
		if ("Sun" == englishName || "Moon" == englishName) {
			double d = selectedObject->getAngularSize(core);
			data["alt_app_rad"] += ((d/2)*M_PI)/180.; 
			extraText = QString(" (lower limb)");
		}
		oss << "<h3>" << selectedObject->getNameI18n() << " Celestial Nav Data</h3>";
		oss << qc_("UTC", "universal coordinated time") << ": " << StelMainScriptAPI::getDate("utc") << " UTC" << "<br/>";
		oss << qc_("Ho", "altitude measured") << ": " << radToDm(data["alt_app_rad"]) << extraText;
		oss.setRealNumberNotation(QTextStream::FixedNotation);
		oss.setRealNumberPrecision(3);
		oss << "<h4>Target information</h4>";
		oss << qc_("GMST: ", "Earth rotation angle (sidereal time)") << " " << data["gmst_deg"] << QChar(0x00B0) << "<br/>";
		oss << qc_("LMST: ", "Earth rotation angle (sidereal time)") << " " << data["lmst_deg"] << QChar(0x00B0) << "<br/>";
		oss << qc_("GHA", "celestial navigation coordinate system") << "&#9800;: " << radToDm(data["gmst_rad"]) << "<br/>";
		oss << qc_("SHA", "celestial navigation coordinate system") << "*: " << radToDm(data["object_sha_rad"]) << "<br/>";
		oss << qc_("DEC", "celestial navigation coordinate system") << "*: " << radToDm(data["object_dec_rad"]) << "<br/>";
		oss << qc_("GHA", "celestial navigation coordinate system") << "*: " << radToDm(data["gha_rad"]) << "<br/>";
		oss << qc_("LHA", "celestial navigation coordinate system") << "*: " << radToDm(data["lha_rad"]) << "<br/>";
		oss << "<h4>Sight Reduction</h4>";
		oss << qc_("Hc/Zn", "celestial navigation coordinate system") << ": " << radToDm(data["Hc_rad"]) << " / " << radToDm(data["Zn_rad"]) << "<br/>";
		selectedObject->setExtraInfoString(oss.readAll());
	}
}

void AstroNav::getCelestialNavData(StelCore* core, StelObjectP selectedObject, QMap<QString, double>& map)
{
	double lat = static_cast<double>(core->getCurrentLocation().latitude);
	double lat_rad = lat * M_PI / 180.;
	map["lat_deg"] = lat;
	map["lat_rad"] = lat_rad;
	double lon = static_cast<double>(core->getCurrentLocation().longitude);
	double lon_rad = lon * M_PI / 180.;
	map["lon_deg"] = lon_rad;
	map["lon_rad"] = lon_rad;
		
	double object_dec, object_ra;		
	StelUtils::rectToSphe(&object_ra, &object_dec, selectedObject->getEquinoxEquatorialPos(core));
	map["object_ra_rad"] = object_ra;
	map["object_dec_rad"] = object_dec;

	double object_sha = (2. * M_PI) - object_ra;
	map["object_sha_rad"] = object_sha;

	double jd = core->getJD();
	double jde = core->getJDE();
	double gmst = wrap360(get_mean_sidereal_time(jd, jde));
	map["jd"] = jd;
	map["jde"] = jde;
	map["gmst_deg"] = gmst;

	double lmst = wrap360(gmst + lon);
	map["lmst_deg"] = lmst;

	double gmst_rad = gmst * M_PI / 180.;
	map["gmst_rad"] = gmst_rad;
	
	double lmst_rad = lmst * M_PI / 180.;
	map["lmst_rad"] = lmst_rad;
	
	map["gha_rad"] = wrap2pi(gmst_rad + object_sha);
	map["lha_rad"] = wrap2pi(lmst_rad + object_sha);
	
	double lha = wrap2pi(lmst_rad + object_sha);
	map["lha_rad"] = lha;

	double az, alt;
	StelUtils::rectToSphe(&az,&alt,selectedObject->getAltAzPosGeometric(core));
	if (az > (2 * M_PI)) az -= (2 * M_PI);
	map["az_rad"] = az;
	map["alt_rad"] = alt;

	double az_app, alt_app;
	StelUtils::rectToSphe(&az_app,&alt_app,selectedObject->getAltAzPosApparent(core));
	if (az_app > (2 * M_PI)) az_app -= (2 * M_PI);
	map["az_app_rad"] = az_app;
	map["alt_app_rad"] = alt_app;

	double Hc = (std::sin(object_dec) * std::sin(lat_rad)) + (std::cos(object_dec) * std::cos(lat_rad) * std::cos(lha));
	Hc = std::asin(Hc);
	map["Hc_rad"] = Hc;

	double Zn = (std::sin(object_dec) * std::cos(lat_rad)) - (std::cos(object_dec) * std::sin(lat_rad) * std::cos(lha));
	Zn = std::acos(Zn / std::cos(Hc));
	if (lha < M_PI) {
		Zn = (2*M_PI) - Zn;
	}
	// Zn is broken for Spica and objects around that Azm. 
	// Need to look at where the calculation breaks down, 
	// I clearly have something wrong whilst going around
	// the hour angle.
	map["Zn_rad"] = Zn;
}

double AstroNav::wrap2pi(double d)
{
	if (d > (2. * M_PI)) d -= (2 * M_PI);
	else if (d < 0.) d += (2 * M_PI);
	return d;
}

double AstroNav::wrap360(double d)
{
	if (d > 360.) d -= 360.;
	else if (d < 0.) d += 360.;
	return d;
}

QString AstroNav::radToDm(double rad)
{
	QString output;
	QTextStream oss(&output);
	bool sign;
	double s, md;
	unsigned int d, m;
	oss.setRealNumberNotation(QTextStream::FixedNotation);
	oss.setRealNumberPrecision(1);
	StelUtils::radToDms(rad, sign, d, m, s);
	md = static_cast<double>(m);
	md += s / 60.;
	oss << (sign ? '+':'-') << d << QChar(0x00B0) << " " << md << "'";
	return oss.readAll();
}

void AstroNav::markNavstars(StelCore* core, StelProjectorP projector, Vec3d colours)
{
	int numOfStars = navstars.size();
	if (numOfStars > 0)
	{
		Vec3d pos;
		StelPainter painter(projector);
		for (int i = 0; i < numOfStars; i++)
		{
			if (projector->projectCheck(navstars[i]->getJ2000EquatorialPos(core), pos))
				if (!markerTexture.isNull())
				{
					painter.setBlending(true);
					painter.setColor(colours[0], colours[1], colours[2]);
					markerTexture->bind();
					painter.drawSprite2dMode(static_cast<float>(pos[0]), static_cast<float>(pos[1]), 16.f);
					painter.drawText(
						static_cast<float>(pos[0]), static_cast<float>(pos[1]),
						navstars[i]->getNameI18n(), 0, 10.f, 10.f, false
					);
				}
		}
	}
}

double AstroNav::getCallOrder(StelModuleActionName actionName) const
{
	if (actionName == StelModule::ActionDraw)
	{
		return StelApp::getInstance().getModuleMgr().getModule("ConstellationMgr")->getCallOrder(actionName)
			   + 10.;
	}

	return 0;
}

//! Used to get a list of objects which are near to some position.
//! @return a list containing the exoplanets located inside the limitFov circle around position v.
QList<StelObjectP> AstroNav::searchAround(const Vec3d& v, double limitFov, const StelCore* core) const
{
	QList<StelObjectP> rval;
	return rval;
}

//! @param nameI18n The case in-sensitive localized exoplanet system name
StelObjectP AstroNav::searchByNameI18n(const QString& nameI18n) const
{
	int numOfStars = navstars.size();
	for (int loop = 0; loop < numOfStars; loop++)
	{
		QString name = navstars[loop]->getNameI18n();
		if (name == nameI18n)
		{
			return navstars[loop];
		}
	}

	return StelObjectP(Q_NULLPTR);
}

//! @param name The case in-sensitive english Navstar system name
StelObjectP AstroNav::searchByName(const QString& englishName) const
{
	int numOfStars = navstars.size();
	for (int loop = 0; loop < numOfStars; loop++)
	{
		QString name = navstars[loop]->getEnglishName();
		if (name == englishName)
		{
			return navstars[loop];
		}
	}

	return StelObjectP(Q_NULLPTR);
}

//! @param id The Navstar system id
StelObjectP AstroNav::searchByID(const QString& id) const
{
	StelObjectP rval = StelObjectP(Q_NULLPTR);
	return rval;
}

QStringList AstroNav::listAllObjects(bool inEnglish) const
{
	QStringList list;
	return list;
}

QString AstroNav::getName() const
{
	return "AstroNav";
}

//! @param state the new update state.
// void updateStateChanged(AstroNav::UpdateState state);
// void AstroNav::flagAstroNavVisibilityChanged(bool b) {}

//! Display a message. This is used for plugin-specific warnings and such
void AstroNav::displayMessage(const QString& message, const QString hexColor)
{
	GETSTELMODULE(LabelMgr)->labelScreen(message, 30, 30, true, 16, hexColor, false, 9000);
}
