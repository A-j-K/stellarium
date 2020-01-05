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

#ifndef ASTRONAV_ALMANACANGLOAMERICAN_HPP
#define ASTRONAV_ALMANACANGLOAMERICAN_HPP

#include "AlmanacStrategy.hpp"

namespace AstroNavAlmanac
{

class AngloAmerican : public Strategy_if
{
public:
	QList<int> getNavstarHIPids(void)
	{
		QList<int> rval;
		rval << 677 << 2081 << 3179 << 3419 << 7588 << 9884 << 13847 << 14135 << 15863 << 21421 << 24436
			 << 24608 << 25336 << 25428 << 26311 << 27989 << 30438 << 32349 << 33579 << 37279 << 37826
			 << 41037 << 44816 << 45238 << 46390 << 49669 << 54061 << 57632 << 59803 << 60718 << 61084
			 << 62956 << 65474 << 67301 << 68702 << 68933 << 69673 << 71683 << 72603 << 72607 << 76267
			 << 80763 << 82273 << 84012 << 85927 << 86032 << 87833 << 90185 << 91262 << 92855 << 97649
			 << 100751 << 102098 << 107315 << 109268 << 113368 << 113963;
		return rval;
	}
};

}; // end namespace AstroNavAlmanac

#endif /* ASTRONAV_ALMANACANGLOAMERICAN_HPP */
