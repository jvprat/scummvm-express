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

#ifndef EXPRESS_NIS_H
#define EXPRESS_NIS_H

#include "common/array.h"
#include "common/stream.h"

namespace Express {

class AnimFrame;

struct Chunk {
	uint16 type;
	uint16 tag;
	uint32 size;
};

class Nis {
public:
	Nis(Common::SeekableReadStream *in);
	~Nis();

private:
	bool load(Common::SeekableReadStream *in);
	void processChunk(Common::SeekableReadStream *in, Chunk *c);

	Common::Array<Chunk> _chunks;
	AnimFrame *_background1, *_background2;
	byte _backgroundCurrent;
};

} // End of Express namespace

#endif // EXPRESS_NIS_H
