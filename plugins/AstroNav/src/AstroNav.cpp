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

#include "LabelMgr.hpp"
#include "StelApp.hpp"

#include "StelFileMgr.hpp"
#include "StelModuleMgr.hpp"
#include "StelObjectMgr.hpp"
#include "StelPainter.hpp"

#include "StelTextureMgr.hpp"
#include "StelTranslator.hpp"
#include <qglobal.h>

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
	StelProjectorP prj = core->getProjection(StelCore::FrameJ2000);
	markNavstars(core, prj);
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
					painter.drawText(static_cast<float>(pos[0]), static_cast<float>(pos[1]),
					  navstars[i]->getNameI18n(), 0, 10.f, 10.f, false);
				}
		}
	}
}
// void AstroNav::drawPointer(StelCore* core, StelPainter& painter)
//{
//}

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
