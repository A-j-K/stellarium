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

#ifndef ASTRONAV_ALMANACFRENCH_HPP
#define ASTRONAV_ALMANACFRENCH_HPP

#include "AlmanacStrategy.hpp"

namespace AstroNavAlmanac
{

class French : public Strategy_if
{
public:
	QList<int> getNavstarHIPids(void)
	{
		QList<int> rval;
		rval << 677 << 746 << 1067 << 2081 << 3179 << 3419 << 4427 << 5447 << 7588 << 9884 << 11767 << 14135
			 << 14576 << 15863 << 21421 << 24436 << 24608 << 25336 << 25428 << 26311 << 26727 << 27989
			 << 28360 << 30324 << 30438 << 31681 << 32349 << 33579 << 34444 << 36850 << 37279 << 37826
			 << 39429 << 39953 << 41037 << 42913 << 44816 << 45238 << 45556 << 46390 << 49669 << 53910
			 << 54061 << 54872 << 57632 << 58001 << 59803 << 60718 << 61084 << 61932 << 62434 << 62956
			 << 65378 << 65474 << 67301 << 68702 << 68933 << 69673 << 71683 << 72607 << 76267 << 80763
			 << 82273 << 82396 << 85927 << 86032 << 87833 << 90185 << 91262 << 92855 << 97649 << 100453
			 << 100751 << 102098 << 105199 << 107315 << 109268 << 112122 << 113368 << 113881 << 113963;
		return rval;
	}
};

}; // end namespace AstroNavAlmanac

#endif /* ASTRONAV_ALMANACFRENCH_HPP */
