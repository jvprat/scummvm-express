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

#include "engines/express/snd.h"
#include "engines/express/debug.h"

#include "common/debug.h"
#include "common/util.h"
#include "common/system.h"
#include "sound/adpcm.h"
#include "sound/mixer.h"
#include "sound/audiostream.h"

// Based on the Xentax Wiki documentation:
// http://wiki.xentax.com/index.php/The_Last_Express_SND

namespace Express {

Snd::Snd(Common::SeekableReadStream *in) {
	uint32 size = in->readUint32LE();
	uint16 blocks = in->readUint16LE();
	warning("size=%d blocks=%d", size, blocks);

	assert (size % blocks == 0);
	uint32 blockSize = size / blocks;

	Audio::AudioStream *as = Audio::makeADPCMStream(in,
		true, size, Audio::kADPCMMSIma, 44100, 1, blockSize);

	Audio::SoundHandle handle;
	g_system->getMixer()->playInputStream(Audio::Mixer::kPlainSoundType, &handle, as);
}

Snd::~Snd() {
}

} // End of Express namespace
