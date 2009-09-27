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

#ifndef EXPRESS_SEQ_H
#define EXPRESS_SEQ_H

#include "common/array.h"
#include "common/stream.h"
#include "graphics/surface.h"

namespace Express {

struct SeqFrame {
	uint32 dataOffset;
	uint32 palOffset;
	uint32 xPos1;
	uint32 yPos1;
	uint32 xPos2;
	uint32 yPos2;
	uint32 initialSkip;
	uint32 decompSize;

	byte compType;
};

class Seq {
public:
	Seq(Common::SeekableReadStream *in);
	~Seq();

private:
	bool load(Common::SeekableReadStream *in);
	void decodeFrame(Common::SeekableReadStream *in, uint32 numFrame);
	void decomp3(Common::SeekableReadStream *in, SeqFrame *f);
	void decomp4(Common::SeekableReadStream *in, SeqFrame *f);
	void decomp34(Common::SeekableReadStream *in, SeqFrame *f, byte mask, byte shift);
	void decomp5(Common::SeekableReadStream *in, SeqFrame *f);
	void decomp7(Common::SeekableReadStream *in, SeqFrame *f);
	Graphics::Surface *applyPalette(Common::SeekableReadStream *in, SeqFrame *f, Graphics::Surface *s, uint32 palSize);

	Common::Array<SeqFrame> _frames;
};

} // End of Express namespace

#endif // EXPRESS_SEQ_H
