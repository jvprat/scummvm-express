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

#include "engines/express/nis.h"
#include "engines/express/seq.h"

#include "common/system.h"
#include "common/util.h"

// Based on Deniz Oezmen's code: http://oezmen.eu/

namespace Express {

Nis::Nis(Common::SeekableReadStream *in) : _background1(NULL), _background2(NULL), _backgroundCurrent(0) {
	load(in);
}

Nis::~Nis() {
}

bool Nis::load(Common::SeekableReadStream *in) {
	// Chunks
	uint32 numChunks = in->readUint32LE();
	warning("%d chunks", numChunks);

	for (uint n = 0; n < numChunks; n++) {
		Chunk c;
		c.type = in->readUint16LE();
		c.tag = in->readUint16LE();
		c.size = in->readUint32LE();
		_chunks.push_back(c);
	}

	for (uint i = 0; i < _chunks.size(); i++) {
		processChunk(in, &_chunks[i]);
	}

	return true;
}

void Nis::processChunk(Common::SeekableReadStream *in, Chunk *c) {
	switch (c->type) {
	case 0x3: // AudioInfo
		assert (c->tag == 0);
		warning("AUDIO INFO: %d blocks", c->size);
		break;
	case 0x4:
		assert (c->tag == 0 && c->size == 0);
		//TODO
		break;
	case 0xb: // SelectBackground1
		assert (c->tag == 0 && c->size == 0);
		_backgroundCurrent = 1;
		break;
	case 0xd: // SelectBackground2
		assert (c->tag == 0 && c->size == 0);
		_backgroundCurrent = 2;
		break;
	case 0x1:
	case 0x2:
	case 0x5:
		assert (c->tag == 0);
		warning("chunk type=%x size=%d", c->type, c->size); //TODO: c->size?
		break;
	case 0xa:
	case 0xc:
	case 0x14: { // Frames
		assert (c->tag == 0);
		Common::MemoryReadStream *str = in->readStream(c->size);
		FrameInfo i;
		i.read(str, 0x124);
		AnimFrame *f = new AnimFrame(str, &i);
		delete str;

		switch (c->type) {
		case 0xa: // BackgroundFrame1
			delete _background1;
			_background1 = f;
			break;
		case 0xc: // BackgroundFrame2
			delete _background2;
			_background2 = f;
			break;
		case 0x14: { // OverlayFrame
			// Create a temporary surface to merge the overlay with the background
			Graphics::Surface *s = new Graphics::Surface;
			s->create(640, 480, 2);

			// Paint the background
			if (_backgroundCurrent == 1 && _background1)
				_background1->paint(s);
			else if (_backgroundCurrent == 2 && _background2)
				_background2->paint(s);

			// Paint the overlay
			f->paint(s);

			// Free the overlay frame
			delete f;

			// XXX: Update the screen
			g_system->copyRectToScreen((byte *)s->pixels, s->pitch, 0, 0, s->w, s->h);
			g_system->updateScreen();

			// Free the temporary surface
			s->free();
			delete s;
		}
		}
		break;
	}
	case 0x15:
	case 0x16:
	case 0x63: // AudioEnd
		assert (c->size == 0);
		warning("chunk type=%x tag=%d", c->type, c->tag); //TODO: c->tag?
		//TODO
		break;
	case 0x20: // AudioData
		warning("chunk type=%x tag=%x size=%d", c->type, c->tag, c->size);
		in->skip(c->size);
		//TODO
		break;
	default:
		error("chunk type=%x (UNKNOWN) tag=%x size=%d", c->type, c->tag, c->size);
	}
}

} // End of Express namespace
