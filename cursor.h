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

#ifndef EXPRESS_CURSOR_H
#define EXPRESS_CURSOR_H

#include "common/archive.h"
#include "common/system.h"

namespace Express {

class ExpressCursorMan {
public:
	ExpressCursorMan(OSystem *system, Common::Archive *arc);
	~ExpressCursorMan();

	void show(bool visible);
	void setStyle(uint8 style);
	uint8 getStyle();

private:
	OSystem *_syst;

	// Style
	uint8 _current;

	// Cursors data
	byte *_data;
};

} // End of Express namespace

#endif // EXPRESS_CURSOR_H
