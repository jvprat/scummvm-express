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

#ifndef EXPRESS_SND_H
#define EXPRESS_SND_H

#include "common/stream.h"
#include "sound/mixer.h"

namespace Audio {
class AudioStream;
class AppendableAudioStream;
}

namespace Express {

class Snd {
public:
	Snd();
	virtual ~Snd();

protected:
	void loadHeader(Common::SeekableReadStream *in);
	Audio::AudioStream *makeDecoder(Common::SeekableReadStream *in, uint32 size) const;
	void play(Audio::AudioStream *as);

	uint32 _size;
	uint16 _blocks;
	uint32 _blockSize;
	Audio::SoundHandle _handle;
};

class StreamedSnd : public Snd {
public:
	StreamedSnd();
	~StreamedSnd();

	void load(Common::SeekableReadStream *in);
};

class AppendableSnd : public Snd {
public:
	AppendableSnd();
	~AppendableSnd();

	void queueBuffer(byte *data, uint32 size);
	void queueBuffer(Common::SeekableReadStream *bufferIn);
	void finish();

private:
	Audio::AppendableAudioStream *_as;
};

} // End of Express namespace

#endif // EXPRESS_SND_H
