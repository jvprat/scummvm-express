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

namespace Express {

Font::Font(Common::Archive *arc) : _glyphs(NULL), _glyphWidths(NULL) {
	Common::SeekableReadStream *fontFile;
	fontFile = arc->createReadStreamForMember("FONT.DAT");
	if (fontFile) {
		// Read the palette
		for (int i = 0; i < 0x10; i++) {
			_palette[i] = fontFile->readUint16LE();
		}

		// Read the character map
		fontFile->read(_charMap, 0x200);

		// Read the glyphs
		_numGlyphs = fontFile->readUint16LE();
		_glyphs = new byte[_numGlyphs * 18 * 8];
		fontFile->read(_glyphs, _numGlyphs * 18 * 8);

		// TODO: Read something else?
		//uint16 unknown = fontFile->readByte();
		//warning("unknown = %d", unknown);
		//warning("pos = %d", fontFile->pos());
		//warning("left = %d", fontFile->size() - fontFile->pos());

		//while (!fontFile->eos()) {
			//unknown = fontFile->readByte();
			//warning("val = %d", unknown);
		//}

		// Precalculate glyph widths
		_glyphWidths = new byte[_numGlyphs];
		for (int i = 0; i < _numGlyphs; i++) {
			_glyphWidths[i] = getGlyphWidth(i);
		}
	}
}

Font::~Font() {
	delete[] _glyphs;
	delete[] _glyphWidths;
}

uint16 Font::getCharGlyph(uint16 c) {
	warning("%c", c);
	if (c >= 0x200)
		error("Express::Font: Invalid character %d", c);

	return _charMap[c];
}

byte *Font::getGlyphImg(uint16 g) {
	if (g >= _numGlyphs)
		error("Express::Font: Invalid glyph %d (%d available)",
			g, _numGlyphs);

	return _glyphs + g * 18 * 8;
}

uint8 Font::getGlyphWidth(uint16 g) {
	byte *p = getGlyphImg(g);

	uint8 maxLineWidth = 0;
	for (int j = 0; j < 18; j++) {
		uint8 currentLineWidth = 0;
		for (int i = 0; i < 16; i++) {
			byte index;
			if (i % 2)
				index = *p & 0xf;
			else
				index = *p >> 4;
			uint16 color = _palette[index];
			if (color != 0x1f)
				currentLineWidth = i;
			if (i % 2)
				p++;
		}
		if (currentLineWidth > maxLineWidth)
			maxLineWidth = currentLineWidth;
	}

	return maxLineWidth;
}

byte *Font::getCharImg(uint16 c) {
	return getGlyphImg(getCharGlyph(c));
}

uint8 Font::getCharWidth(uint16 c) {
	if (c == 0x20) {
		// Space is a special case
		// TODO: this is an arbitrary value
		return 10;
	} else {
		return _glyphWidths[getCharGlyph(c)];
	}
}

uint16 Font::getStringWidth(Common::String str) {
	uint16 width = 0;
	for (uint i = 0; i < str.size(); i++)
		width += getCharWidth(str[i]);
	
	return width;
}

uint16 Font::getStringWidth(uint16 *str, uint16 length) {
	uint16 width = 0;
	for (uint i = 0; i < length; i++)
		width += getCharWidth(str[i]);
	
	return width;
}

void Font::drawChar(int x, int y, uint16 c) {
	byte *p = getCharImg(c);

	for (int j = 0; j < 18; j++) {
		for (int i = 0; i < 16; i++) {
			byte index;
			if (i % 2)
				index = *p & 0xf;
			else
				index = *p >> 4;
			uint16 color = _palette[index];
			if (color != 0x1f)
				g_system->copyRectToScreen((byte *)&color, 2, x + i, y + j, 1, 1);
			if (i % 2)
				p++;
		}
	}
}

void Font::drawString(int x, int y, Common::String str) {
	int currentX = x;
	for (uint i = 0; i < str.size(); i++) {
		drawChar(currentX, y, str[i]);
		currentX += getCharWidth(str[i]);
	}
}

void Font::drawString(int x, int y, uint16 *str, uint16 length) {
	int currentX = x;
	for (uint i = 0; i < length; i++) {
		drawChar(currentX, y, str[i]);
		currentX += getCharWidth(str[i]);
	}
}

} // End of Express namespace
