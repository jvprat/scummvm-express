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

#ifndef EXPRESS_HPF_H
#define EXPRESS_HPF_H

#include "common/archive.h"

namespace Express {

class HpfMember;

class HpfArchive : public Common::Archive {
public:
	bool open(const Common::String &filename);

	bool hasFile(const Common::String &name);
	int listMatchingMembers(Common::ArchiveMemberList &list, const Common::String &pattern);
	int listMembers(Common::ArchiveMemberList &list);
	Common::ArchiveMemberPtr getMember(const Common::String &name);
	Common::SeekableReadStream *createReadStreamForMember(const Common::String &name) const;
	Common::SeekableReadStream *createReadStreamForMember(const HpfMember *member) const;

private:
	Common::String _filename;
	Common::ArchiveMemberList _members;
};

} // End of namespace Express

#endif // EXPRESS_HPF_H
