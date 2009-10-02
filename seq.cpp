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

#include "engines/express/seq.h"

#include "common/system.h"
//#include "common/util.h"

// Based on Deniz Oezmen's code: http://oezmen.eu/

namespace Express {

void FrameInfo::read(Common::SeekableReadStream *in, uint16 decompOffset) {
	// Save the current position
	uint32 basePos = in->pos();

	dataOffset = in->readUint32LE();
	//warning("frame, dataofs: 0x%04x %d", dataOffset, dataOffset);
	/*uint32 unknown =*/ in->readUint32LE();
	//warning("frame, unk1: 0x%04x %d", unknown, unknown);
	palOffset = in->readUint32LE();
	//warning("frame, palofs: 0x%04x %d", palOffset, palOffset);
	xPos1 = in->readUint32LE();
	//warning("frame, xpos1: 0x%04x %d", xPos1, xPos1);
	yPos1 = in->readUint32LE();
	//warning("frame, ypos1: 0x%04x %d", yPos1, yPos1);
	xPos2 = in->readUint32LE();
	//warning("frame, xpos2: 0x%04x %d", xPos2, xPos2);
	yPos2 = in->readUint32LE();
	//warning("frame, ypos2: 0x%04x %d", yPos2, yPos2);
	initialSkip = in->readUint32LE();
	//warning("frame, initialskip: 0x%04x %d", initialSkip, initialSkip);
	decompSize = in->readUint32LE();
	//warning("frame, decompsize: 0x%04x %d", decompSize, decompSize);

	// Read the compression type
	in->seek(basePos + decompOffset);
	compType = in->readByte();
	//warning("frame, comptype: 0x%04x %d", compType, compType);

	/*
	unknown = in->readUint32LE();
	//warning("frame, val 0: 0x%04x %d", unknown, unknown);
	unknown = in->readUint32LE();
	//warning("frame, val 1: 0x%04x %d", unknown, unknown);
	compType = in->readByte();
	//warning("frame, comptype: 0x%04x %d", compType, compType);
	unknown = in->readByte();
	//warning("frame, val 2: 0x%04x %d", unknown, unknown);
	unknown = in->readUint16LE();
	//warning("frame, val 2: 0x%04x %d", unknown, unknown);

	// decomp?
	for (int i = 3; i < 8; i++) {
		unknown = in->readUint32LE();
		//warning("frame, val %d: 0x%04x %d", n, i, unknown, unknown);
		if (i == 5 || i == 7)
			assert (unknown == 0);
	}
	*/
}

// AnimFrame

AnimFrame::AnimFrame(Common::SeekableReadStream *in, FrameInfo *f) : _palette(NULL) {
	_palSize = 1;
	_image.create(640, 480, 1);

	switch (f->compType) {
	case 0:
		// Empty frame
		break;
	case 3:
		decomp3(in, f);
		break;
	case 4:
		decomp4(in, f);
		break;
	case 5:
		decomp5(in, f);
		break;
	case 7:
		decomp7(in, f);
		break;
	case 255:
		decompFF(in, f);
		break;
	default:
		error("Unknown compression: %d", f->compType);
	}

	readPalette(in, f);
}

AnimFrame::~AnimFrame() {
	_image.free();
	delete[] _palette;
}

void AnimFrame::paint(Graphics::Surface *s) {
	byte *inp = (byte *)_image.pixels;
	uint16 *outp = (uint16 *)s->pixels;
	for (int i = 0; i < 640 * 480; i++, inp++, outp++) {
		if (*inp)
			*outp = _palette[*inp];
	}
}

void AnimFrame::readPalette(Common::SeekableReadStream *in, FrameInfo *f) {
	// Read the palette
	in->seek(f->palOffset);
	_palette = new uint16[_palSize];
	for (uint32 i = 0; i < _palSize; i++) {
		_palette[i] = in->readUint16LE();
	}
}

void AnimFrame::decomp3(Common::SeekableReadStream *in, FrameInfo *f) {
	decomp34(in, f, 0x7, 3);
}

void AnimFrame::decomp4(Common::SeekableReadStream *in, FrameInfo *f) {
	decomp34(in, f, 0xf, 4);
}

void AnimFrame::decomp34(Common::SeekableReadStream *in, FrameInfo *f, byte mask, byte shift) {
	byte *p = (byte *)_image.getBasePtr(0, 0);

	uint32 skip = f->initialSkip / 2;
	uint32 size = f->decompSize / 2;
	//warning("skip: %d, %d", skip % 640, skip / 640);
	//warning("size: %d, %d", size % 640, size / 640);
	//assert (f->yPos1 == skip / 640);
	//assert (f->yPos2 == size / 640);

	uint32 numBlanks = 640 - (f->xPos2 - f->xPos1);

	in->seek(f->dataOffset);
	for (uint32 out = skip; out < size; ) {
		uint16 opcode = in->readByte();

		if (opcode & 0x80) {
			if (opcode & 0x40) {
				opcode &= 0x3f;
				out += numBlanks + opcode + 1;
			} else {
				opcode &= 0x3f;
				if (opcode & 0x20) {
					opcode = ((opcode & 0x1f) << 8) + in->readByte();
					if (opcode & 0x1000) {
						out += opcode & 0xfff;
						continue;
					}
				}
				out += opcode + 2;
			}
		} else {
			byte value = opcode & mask;
			opcode >>= shift;
			if (_palSize <= value)
				_palSize = value + 1;
			if (!opcode)
				opcode = in->readByte();
			for (int i = 0; i < opcode; i++, out++) {
				p[out] = value;
			}
		}
	}
}

void AnimFrame::decomp5(Common::SeekableReadStream *in, FrameInfo *f) {
	byte *p = (byte *)_image.getBasePtr(0, 0);

	uint32 skip = f->initialSkip / 2;
	uint32 size = f->decompSize / 2;
	//warning("skip: %d, %d", skip % 640, skip / 640);
	//warning("size: %d, %d", size % 640, size / 640);
	//assert (f->yPos1 == skip / 640);
	//assert (f->yPos2 == size / 640);

	in->seek(f->dataOffset);
	for (uint32 out = skip; out < size; ) {
		uint16 opcode = in->readByte();
		if (!(opcode & 0x1f)) {
			opcode = (opcode << 3) + in->readByte();
			if (opcode & 0x400) {
				// skip these 10 bits
				out += (opcode & 0x3ff);
			} else {
				out += opcode + 2;
			}
		} else {
			byte value = opcode & 0x1f;
			opcode >>= 5;
			if (_palSize <= value)
				_palSize = value + 1;
			if (!opcode)
				opcode = in->readByte();
			for (int i = 0; i < opcode; i++, out++) {
				p[out] = value;
			}
		}
	}
}

void AnimFrame::decomp7(Common::SeekableReadStream *in, FrameInfo *f) {
	byte *p = (byte *)_image.getBasePtr(0, 0);

	uint32 skip = f->initialSkip / 2;
	uint32 size = f->decompSize / 2;
	//warning("skip: %d, %d", skip % 640, skip / 640);
	//warning("size: %d, %d", size % 640, size / 640);
	//assert (f->yPos1 == skip / 640);
	//assert (f->yPos2 == size / 640);

	uint32 numBlanks = 640 - (f->xPos2 - f->xPos1);

	in->seek(f->dataOffset);
	for (uint32 out = skip; out < size; ) {
		uint16 opcode = in->readByte();
		if (opcode & 0x80) {
			if (opcode & 0x40) {
				if (opcode & 0x20) {
					opcode &= 0x1f;
					out += numBlanks + opcode + 1;
				} else {
					opcode &= 0x1f;
					if (opcode & 0x10) {
						opcode = ((opcode & 0xf) << 8) + in->readByte();
						if (opcode & 0x800) {
							// skip these 11 bits
							out += (opcode & 0x7ff);
							continue;
						}
					}

					// skip these 4 bits
					out += opcode + 2;
				}
			} else {
				opcode &= 0x3f;
				byte value = in->readByte();
				if (_palSize <= value)
					_palSize = value + 1;
				for (int i = 0; i < opcode; i++, out++) {
					p[out] = value;
				}
			}
		} else {
			if (_palSize <= opcode)
				_palSize = opcode + 1;
			// set the given value
			p[out] = opcode;
			out++;
		}
	}
}

void AnimFrame::decompFF(Common::SeekableReadStream *in, FrameInfo *f) {
	byte *p = (byte *)_image.getBasePtr(0, 0);

	uint32 skip = f->initialSkip / 2;
	uint32 size = f->decompSize / 2;

	in->seek(f->dataOffset);
	for (uint32 out = skip; out < size; ) {
		uint16 opcode = in->readByte();

		if (opcode < 0x80) {
			if (_palSize <= opcode)
				_palSize = opcode + 1;
			// set the given value
			p[out] = opcode;
			out++;
		} else {
			if (opcode < 0xf0) {
				if (opcode < 0xe0) {
					// copy old part
					uint32 old = out + ((opcode & 0x7) << 8) + in->readByte() - 2048;
					opcode = ((opcode >> 3) & 0xf) + 3;
					for (int i = 0; i < opcode; i++, out++, old++) {
						p[out] = p[old];
					}
				} else {
					opcode = (opcode & 0xf) + 1;
					byte value = in->readByte();
					if (_palSize <= value)
						_palSize = value + 1;
					for (int i = 0; i < opcode; i++, out++) {
						p[out] = value;
					}
				}
			} else {
				out += ((opcode & 0xf) << 8) + in->readByte();
			}
		}
	}
}


// Seq

Seq::Seq(Common::SeekableReadStream *in) {
	load(in);
}

Seq::~Seq() {
}

bool Seq::load(Common::SeekableReadStream *in) {
	// Frames
	uint32 numFrames = in->readUint32LE();

	// Unknown
	uint32 unknown = in->readUint32LE();

	warning("%d frames unk(0x%x)", numFrames, unknown);
	assert (unknown == 0);

	for (uint n = 0; n < numFrames; n++) {
		FrameInfo f;
		in->seek(8 + 0x44 * n);
		f.read(in, 0x2c);
		_frames.push_back(f);
	}

	for (uint i = 0; i < _frames.size(); i++) {
		warning("decoding frame %d", i);
		AnimFrame *f = new AnimFrame(in, &_frames[i]);

		//TEST: paint it directly to screen
		//Graphics::Surface *s = g_system->lockScreen();
		Graphics::Surface *s = new Graphics::Surface;
		s->create(640, 480, 2);

		f->paint(s);

		g_system->copyRectToScreen((byte *)s->pixels, s->pitch, 0, 0, s->w, s->h);
		s->free();
		delete s;
		//g_system->unlockScreen();
		g_system->updateScreen();

		delete f;
	}

	return true;
}

} // End of Express namespace
