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

#ifndef ASTRONAV_ALMANACRUSSIAN_HPP
#define ASTRONAV_ALMANACRUSSIAN_HPP

#include "AlmanacStrategy.hpp"

namespace AstroNavAlmanac
{

class Russian : public Strategy_if
{
public:
	QList<int> getNavstarHIPids(void)
	{
		QList<int> rval;
		rval << 677 << 746 << 1067 << 2021 << 2081 << 3179 << 3419 << 4427 << 5447 << 6686 << 7588 << 8886
			 << 8903 << 9236 << 9640 << 9884 << 13847 << 14135 << 14576 << 15863 << 17702 << 18246 << 18532
			 << 21421 << 23015 << 23875 << 24436 << 24608 << 25336 << 25428 << 25606 << 25930 << 25985
			 << 26241 << 26311 << 26451 << 26634 << 26727 << 27913 << 27989 << 28360 << 28380 << 30324
			 << 30438 << 31681 << 32349 << 32768 << 33579 << 33152 << 34444 << 35264 << 35904 << 36188
			 << 36850 << 37279 << 37826 << 39429 << 39757 << 39953 << 41037 << 42913 << 44816 << 45238
			 << 45556 << 46390 << 46701 << 49669 << 50583 << 52419 << 52727 << 53910 << 54061 << 54872
			 << 57632 << 58001 << 59196 << 59747 << 59774 << 59803 << 60718 << 61084 << 61359 << 61585
			 << 61932 << 61941 << 62434 << 62956 << 63121 << 63608 << 65109 << 65378 << 65474 << 66657
			 << 67301 << 67927 << 68002 << 68702 << 68933 << 69673 << 71075 << 71352 << 71681 << 71860
			 << 72105 << 72603 << 72607 << 73273 << 74946 << 74785 << 76297 << 76267 << 77070 << 78401
			 << 78820 << 79593 << 80331 << 80763 << 80816 << 81266 << 81377 << 81693 << 82273 << 82396
			 << 83081 << 84012 << 85258 << 85792 << 85670 << 85927 << 86032 << 86228 << 79540 << 86742
			 << 87833 << 88635 << 89931 << 90185 << 90496 << 91262 << 95347 << 93506 << 93747 << 94141
			 << 97165 << 97278 << 97649 << 100453 << 100751 << 102098 << 102488 << 105199 << 107315 << 107556
			 << 109268 << 110130 << 112122 << 113368 << 113881 << 113963 << 11767;
		return rval;
	}
};

}; // end namespace AstroNavAlmanac

#endif /* ASTRONAV_ALMANACRUSSIAN_HPP */
