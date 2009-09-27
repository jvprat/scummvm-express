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

#include "engines/express/font.h"
//#include "engines/express/debug.h"

#include "common/stream.h"
#include "common/system.h"

//TODO: split _data into different sections

namespace Express {

Font::Font(Common::Archive *arc) : _data(NULL) {
	Common::SeekableReadStream *fontFile;
	fontFile = arc->createReadStreamForMember("FONT.DAT");
	if (fontFile) {
		_data = new byte[fontFile->size()];
		if (_data) {
			fontFile->read(_data, fontFile->size());
		}
	}

	/*
	for (int i = 0; i < 512; i++) {
		if (_data[0x20 + i])
			warning ("char %d(%d-%d)->%d %c ok", i, i/256, i%256, _data[0x20 + i], i);
	}
	*/
}

Font::~Font() {
	delete _data;
}

byte *Font::getCharImg(uint16 c) {
	byte charPos = _data[0x20 + c];
	return _data + 0x222 + charPos * 9 * 16;
}

void Font::drawChar(int x, int y, uint16 c) {
	byte *p = getCharImg(c);

	for (int j = 0; j < 18; j++) {
		for (int i = 0; i < 8; i++) {
			g_system->copyRectToScreen(&_data[p[0] >> 4], 2, x + i + 0, y + j, 1, 1);
			g_system->copyRectToScreen(&_data[p[0] & 0xf], 2, x + i + 1, y + j, 1, 1);
			p++;
		}
	}
}

void Font::drawString(int x, int y, Common::String str) {
	int currentX = x;
	for (uint i = 0; i < str.size(); i++, currentX += 10) {
		drawChar(currentX, y, str[i]);
	}
}

void Font::drawString(int x, int y, uint16 *str, uint16 length) {
	int currentX = x;
	for (int i = 0; i < length; i++, currentX += 10) {
		drawChar(currentX, y, str[i]);
	}
}

} // End of Express namespace
