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

#include "engines/express/cursor.h"
#include "engines/express/debug.h"

#include "common/endian.h"
#include "common/stream.h"

#include "graphics/cursorman.h"

#define MAX_CURSOR 48

namespace Express {

ExpressCursorMan::ExpressCursorMan(OSystem *system, Common::Archive *arc) :
	_syst(system), _current(255), _data(NULL) {

	Common::SeekableReadStream *cursorFile;
	cursorFile = arc->createReadStreamForMember("CURSORS.TBM");
	if (cursorFile) {
		_data = new byte[cursorFile->size()];
		if (_data) {
			cursorFile->read(_data, cursorFile->size());
		}
	}
}

ExpressCursorMan::~ExpressCursorMan() {
	delete _data;

	//CursorMan.popAllCursors();
}

void ExpressCursorMan::show(bool visible) {
	CursorMan.showMouse(visible);
}

void ExpressCursorMan::setStyle(uint8 style) {
	assert (_data);
	assert (style < MAX_CURSOR);
	debugC(10, kExpressDebugCursor | kExpressDebugAll, "Express::Cursor: setting style: %d", style);

	// Save the new cursor
	_current = style;

	// Prepare the pixel data
	uint16 pixels[32 * 32];
	byte *fileImage = _data + MAX_CURSOR * 4 + (style * 32 * 32 * 2);
	for(int i = 0; i < 32 * 32; i++, fileImage += 2) {
		pixels[i] = READ_LE_UINT16(fileImage);
		// TODO: transparency: now it makes the black pixels transparent,
		// but there's still 1 unused bit (always 0?)
		//pixels[i] = (READ_LE_UINT16(getStyleImage(style) + (i * 2)) & 0x8000) ? 0 : 0xffff;
		//pixels[i] = (fileImage[1] & 0x80) ? 0 : 0xffff;
	}

	uint16 hotspotX = READ_LE_UINT16(_data + (style * 4));
	uint16 hotspotY = READ_LE_UINT16(_data + (style * 4) + 2);
	debugC(15, kExpressDebugCursor | kExpressDebugAll, "Express::Cursor: hotspot x: %d, hotspot y: %d", hotspotX, hotspotY);
	CursorMan.replaceCursor((const byte *)pixels, 32, 32, hotspotX, hotspotY, 0, 1, new Graphics::PixelFormat(2, 5, 5, 5, 0, 10, 5, 0, 0));
}

uint8 ExpressCursorMan::getStyle() {
	return _current;
}

} // End of Express namespace
