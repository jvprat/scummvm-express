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

#ifndef EXPRESS_SBE_H
#define EXPRESS_SBE_H

#include "engines/express/font.h"

namespace Express {

class Subtitle {
public:
	Subtitle();
	~Subtitle();

	bool load(Common::SeekableReadStream *in);
	void show(Font &font);

private:
	uint16 _timeStart;
	uint16 _timeStop;

	uint16 _topLength;
	uint16 *_topText;

	uint16 _bottomLength;
	uint16 *_bottomText;
};

class Sbe {
public:
	Sbe(Common::SeekableReadStream *in);
	~Sbe();

	void show(Font &font, uint16 i);

private:
	void reset(void);
	bool load(Common::SeekableReadStream *in);
	Common::Array<Subtitle> _subtitles;
};

} // End of Express namespace

#endif // EXPRESS_SBE_H
