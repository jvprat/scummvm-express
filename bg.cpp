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

#include "engines/express/bg.h"
#include "engines/express/debug.h"

#include "common/debug.h"
#include "common/system.h"

// Based on the Xentax Wiki documentation:
// http://wiki.xentax.com/index.php/The_Last_Express_BG

namespace Express {

BackGround::BackGround(Common::SeekableReadStream *in) : _data(NULL) {
	load(in);
}

BackGround::~BackGround() {
	reset();
}

void BackGround::show(void) {
	g_system->fillScreen(0);
	g_system->copyRectToScreen((byte *)_data, _width * 2, _posX, _posY, _width, _height);
	g_system->updateScreen();
}

void BackGround::reset(void) {
	delete[] _data;
	_data = NULL;
}

bool BackGround::load(Common::SeekableReadStream *in) {
	// Position on screen
	_posX = in->readUint32LE();
	_posY = in->readUint32LE();

	// Size of the image
	_width = in->readUint32LE();
	_height = in->readUint32LE();
	uint32 numPix = _width * _height;

	debugC(10, kExpressDebugData | kExpressDebugAll, "Express::BackGround: pos=(%d,%d) size=(%d,%d)", _posX, _posY, _width, _height);

	// Size of the components
	uint32 sizeR = in->readUint32LE();
	uint32 sizeB = in->readUint32LE();
	uint32 sizeG = in->readUint32LE();

	debugC(15, kExpressDebugData | kExpressDebugAll, "Express::BackGround: size of the components: (R,G,B)=(%d,%d,%d)", sizeR, sizeG, sizeB);

	// Read the red component
	byte *dataR = decodeComponent(in, sizeR, numPix);
	bool ok = true;
	if (!dataR) ok = false;

	// Read the blue component
	byte *dataB = NULL;
	if (ok) {
		dataB = decodeComponent(in, sizeB, numPix);
		if (!dataB) ok = false;
	}

	// Read the green component
	byte *dataG = NULL;
	if (ok) {
		dataG = decodeComponent(in, sizeG, numPix);
		if (!dataG) ok = false;
	}

	// Merge the components
	reset();
	if (ok) {
		_data = new uint16[numPix];
		if (!_data) {
			ok = false;
		} else {
			for (uint i = 0; i < numPix; i++) {
				_data[i] = (dataR[i] << 10) + (dataG[i] << 5) + dataB[i];
			}
		}
	}

	// Free the temporary components
	delete[] dataR;
	delete[] dataG;
	delete[] dataB;

	return ok;
}

byte *BackGround::decodeComponent(Common::SeekableReadStream *in, uint32 inSize, uint32 outSize) {
	// Create the destination array
	byte *out = new byte[outSize];
	if (!out)
		return false;

	// Initialize the decoding
	uint32 inPos = 0;
	uint32 outPos = 0;

	// Decode
	while (inPos < inSize) {
		byte inByte = in->readByte();
		inPos++;

		if (inByte < 0x80) {
			// Direct decompression (RLE)
			byte len = (inByte >> 5) + 1;
			byte data = inByte & 0x1f;
			for (int i = 0; i < len && outPos < outSize; i++)
				out[outPos++] = data;
		} else {
			// Buffer back reference, 4096 byte window
			// Take inByte and the following value as a big endian
			// OfsLen while zeroing the first bit
			uint16 ofsLen = ((inByte & 0x7F) << 8) | in->readByte();
			inPos++;

			int32 len = (ofsLen >> 12) + 3;
			int32 hisPos = outPos + (ofsLen & 0x0FFF) - 4096;
			for (int i = 0; i < len && outPos < outSize; i++)
				out[outPos++] = out[hisPos++];
		}
	}

	return out;
}

} // End of Express namespace
