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

// Based on the Xentax Wiki documentation:
// http://wiki.xentax.com/index.php/The_Last_Express_HPF

#include "engines/express/hpf.h"
#include "engines/express/debug.h"

#include "common/debug.h"
#include "common/file.h"

namespace Express {

// ARCHIVE MEMBER

class HpfMember : public Common::ArchiveMember {
public:
	HpfMember(const HpfArchive *hpf, Common::ReadStream &stream);

	Common::SeekableReadStream *createReadStream() const;
	Common::String getName() const { return _name; }
	uint32 getSize() const { return _size; }
	uint32 getOffset() const { return _offset; }

private:
	const HpfArchive *_hpf;
	Common::String _name;
	uint32 _offset;
	uint32 _size;
};

HpfMember::HpfMember(const HpfArchive *hpf, Common::ReadStream &stream) {
	_hpf = hpf;

	// Read the information about this archive member
	char name[12];
	stream.read(name, 12);
	_name = Common::String(name, 12);

	_offset = stream.readUint32LE() * 2048;
	_size = stream.readUint32LE() * 2048;
	debugC(20, kExpressDebugData, "Express::Hpf: Member \"%s\" starts at offset=%d and has size=%d", _name.c_str(), _offset, _size);

	uint16 unknown = stream.readUint16LE();
	debugC(5, kExpressDebugUnknown, "Express::Hpf: Member \"%s\" has unknown=%d", _name.c_str(), unknown);

#if 0
	// DUMP
	Common::SeekableReadStream *in = _hpf->createReadStreamForMember(this);
	byte *data = new byte[_size];
	in->read(data, _size);
	delete in;

	Common::DumpFile out;
	out.open(Common::String("dumps/") + _name);
	out.write(data, _size);
	out.close();

	delete[] data;
#endif
}

Common::SeekableReadStream *HpfMember::createReadStream() const {
	return _hpf->createReadStreamForMember(this);
}


// ARCHIVE

bool HpfArchive::open(const Common::String &filename) {
	Common::File stream;
	if (!stream.open(filename)) {
		return false;
	}

	_filename = filename;
	_members.clear();

	// Read the number of contained files
	uint32 numFiles = stream.readUint32LE();
	debugC(15, kExpressDebugData, "Express::Hpf: \"%s\" contains %d files", _filename.c_str(), numFiles);

	for (uint32 i = 0; i < numFiles; i++) {
		HpfMember *member = new HpfMember(this, stream);
		_members.push_back(Common::ArchiveMemberPtr(member));
	}

	return true;
}

bool HpfArchive::hasFile(const Common::String &name) {
	for (Common::ArchiveMemberList::iterator it = _members.begin(); it != _members.end(); ++it) {
		if ((*it)->getName() == name) {
			// Found it
			return true;
		}
	}

	// Not found
	return false;
}

int HpfArchive::listMatchingMembers(Common::ArchiveMemberList &list, const Common::String &pattern) {
	int matches = 0;
	for (Common::ArchiveMemberList::iterator it = _members.begin(); it != _members.end(); ++it) {
		if ((*it)->getName().matchString(pattern)) {
			// This file matches, add it
			list.push_back(*it);
			matches++;
		}
	}

	return matches;
}

int HpfArchive::listMembers(Common::ArchiveMemberList &list) {
	int files = 0;
	for (Common::ArchiveMemberList::iterator it = _members.begin(); it != _members.end(); ++it) {
		// Add all the members to the list
		list.push_back(*it);
		files++;
	}

	return files;
}

Common::ArchiveMemberPtr HpfArchive::getMember(const Common::String &name) {
	for (Common::ArchiveMemberList::iterator it = _members.begin(); it != _members.end(); ++it) {
		if ((*it)->getName() == name) {
			// Found it
			return *it;
		}
	}

	// Not found, return an empty ptr
	return Common::ArchiveMemberPtr();
}

Common::SeekableReadStream *HpfArchive::createReadStreamForMember(const Common::String &name) const {
	for (Common::ArchiveMemberList::const_iterator it = _members.begin(); it != _members.end(); ++it) {
		if ((*it)->getName() == name) {
			// Found it
			return createReadStreamForMember((const HpfMember *)it->get());
		}
	}

	// Not found
	return 0;
}

Common::SeekableReadStream *HpfArchive::createReadStreamForMember(const HpfMember *member) const {
	// Open the hpf file
	Common::File *f = new Common::File;
	if (!f)
		return NULL;

	if (!f->open(_filename)) {
		delete f;
		return NULL;
	}

	// Return the substream that contains the archive member
	uint32 offset = member->getOffset();
	uint32 size = member->getSize();
	return new Common::SeekableSubReadStream(f, offset, offset + size, true);
}

} // End of namespace Express
