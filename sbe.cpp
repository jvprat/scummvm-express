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

#include "engines/express/sbe.h"

#include "common/stream.h"

namespace Express {

Subtitle::Subtitle() : _timeStart(0), _timeStop(0),
	_topLength(0), _topText(NULL), _bottomLength(0), _bottomText(NULL) {
}

Subtitle::~Subtitle() {
	delete _topText;
	delete _bottomText;
}

void Subtitle::show(Font &font) {
	font.drawString(100, 100, _topText, _topLength);
	font.drawString(100, 300, _bottomText, _bottomLength);
}

bool Subtitle::load(Common::SeekableReadStream *in) {
	// Read the display times
	_timeStart = in->readUint16LE();
	_timeStop = in->readUint16LE();

	// Read the text lengths
	_topLength = in->readUint16LE();
	_bottomLength = in->readUint16LE();

	// Create the buffers
	if (_topLength) {
		_topText = new uint16[_topLength];
		if (!_topText)
			return false;
	}
	if (_bottomLength) {
		_bottomText = new uint16[_bottomLength];
		if (!_bottomText)
			return false;
	}

	// Read the texts
	for (int i = 0; i < _topLength; i++)
		_topText[i] = in->readUint16LE();
	for (int i = 0; i < _bottomLength; i++)
		_bottomText[i] = in->readUint16LE();

	return true;
}


Sbe::Sbe(Common::SeekableReadStream *in) {
	load(in);
}

Sbe::~Sbe() {
	_subtitles.clear();
}

void Sbe::show(Font &font, uint16 i) {
	_subtitles[i].show(font);
}

bool Sbe::load(Common::SeekableReadStream *in) {
	uint16 numSubtitles = in->readUint16LE();
	warning("%d subs", numSubtitles);

	for (int i = 0; i < numSubtitles; i++) {
		_subtitles.push_back(Subtitle());
		if (!_subtitles.back().load(in)) {
			// Failed to read this line
			_subtitles.clear();
			return false;
		}
	}

	return true;
}

} // End of Express namespace
