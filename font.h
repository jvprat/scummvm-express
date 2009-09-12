/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * $URL$
 * $Id$
 *
 */

#ifndef EXPRESS_FONT_H
#define EXPRESS_FONT_H

#include "common/archive.h"

namespace Express {

class Font {
public:
	Font(Common::Archive *arc);
	~Font();

	void drawString(int x, int y, Common::String str);
	void drawString(int x, int y, uint16 *str, uint16 length);

private:
	byte *getCharImg(uint16 c);
	void drawChar(int x, int y, uint16 c);

	// Font data
	byte *_data;
};

} // End of Express namespace

#endif // EXPRESS_FONT_H
