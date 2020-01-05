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

#include "StelApp.hpp"
#include "StelTranslator.hpp"

#include "AstroNav.hpp"
#include "AstroNavPlugin.hpp"

/*
 This method is the one called automatically by the StelModuleMgr just
 after loading the dynamic library
*/
StelModule* AstroNavStelPluginInterface::getStelModule() const
{
	return new AstroNav();
}

StelPluginInfo AstroNavStelPluginInterface::getPluginInfo() const
{
	Q_INIT_RESOURCE(AstroNav);

	StelPluginInfo info;
	info.id = "AstroNav";
	info.displayedName = N_("AstroNav");
	info.authors = "Andy Kirkham";
	info.contact = "https://github.com/Stellarium/stellarium";
	info.description = N_("This plugin ... ToDo");
	info.version = ASTRONAV_PLUGIN_VERSION;
	info.license = ASTRONAV_PLUGIN_LICENSE;
	return info;
}

