/* Residual - A 3D game interpreter
 *
 * Residual is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the AUTHORS
 * file distributed with this source distribution.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.

 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 *
 * $URL: https://residual.svn.sourceforge.net/svnroot/residual/residual/trunk/engines/grim/grim.h $
 * $Id: grim.h 1360 2009-05-26 14:13:08Z aquadran $
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
