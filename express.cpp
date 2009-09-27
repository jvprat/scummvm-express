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

#include "engines/express/express.h"
#include "engines/express/bg.h"
#include "engines/express/cursor.h"
#include "engines/express/debug.h"
#include "engines/express/font.h"
#include "engines/express/sbe.h"
#include "engines/express/snd.h"

#include "common/events.h"
#include "common/file.h"

namespace Express {

ExpressEngine::ExpressEngine(OSystem *syst, const ADGameDescription *gd) :
	Engine(syst), _gd(gd) {
	Common::addDebugChannel(kExpressDebugAll, "All", "Debug the everything");
	Common::addDebugChannel(kExpressDebugData, "Data", "Debug the data reading");
	Common::addDebugChannel(kExpressDebugCursor, "Cursor", "Debug the mouse cursor handling");
	Common::addDebugChannel(kExpressDebugUnknown, "Unknown", "Debug the unknown data");
}

ExpressEngine::~ExpressEngine() {
}

Common::Error ExpressEngine::run() {
	// Initialize backend
	_system->beginGFXTransaction();
	initCommonGFX(true);
	//_system->initSize(640, 480, new Graphics::PixelFormat(3, 8, 8, 8, 0, 0, 8, 16, 0));
	_system->initSize(640, 480, new Graphics::PixelFormat(2, 5, 5, 5, 0, 10, 5, 0, 0));
	_system->endGFXTransaction();

	// Start the resource manager
	if (!Common::File::exists(_gd->filesDescriptions[0].fileName) ||
		!_hpf.open(_gd->filesDescriptions[0].fileName)) {
		return Common::kNoGameDataFoundError;
	}
	
	HpfArchive cd;
	cd.open("cd1.hpf");
	//cd.open("cd2.hpf");
	//cd.open("cd3.hpf");

	// Test Backgrounds
	Common::ArchiveMemberList list_bg;
	int num_bg = cd.listMatchingMembers(list_bg, "*.BG");
	warning("found %d bg's", num_bg);
	Common::ArchiveMemberList::iterator i_bg = list_bg.begin();
	//for (; i != list.end(); i++) {
		//warning("bg: %s", (*i)->getName().c_str());
		//Common::SeekableReadStream *stream = (*i)->createReadStream();
		//BackGround bg(stream);
		//delete stream;
	//}

	ExpressCursorMan cur(_system, &_hpf);
	uint8 st = 0;
	cur.setStyle(st);
	cur.show(true);

	Font font(&_hpf);

	g_system->fillScreen(0);

	Common::SeekableReadStream *sbe = _hpf.createReadStreamForMember("XALX4.SBE");
	Sbe sub(sbe);
	sub.show(font, 0);
	g_system->updateScreen();

	Common::SeekableReadStream *snd = _hpf.createReadStreamForMember("MUS018.SND");
	//Snd s(snd);
	//int num = cd.listMatchingMembers(list, "*.SND");
	//warning("found %d files", num);
	//Common::ArchiveMemberList::iterator i = list.begin();

	bool end = false;
	Common::EventManager *em = _system->getEventManager();
	while (!end) {
		Common::Event ev;
		if (em->pollEvent(ev)) {
			if (ev.type == Common::EVENT_KEYDOWN) {
				if (ev.kbd.keycode == Common::KEYCODE_ESCAPE)
					end = true;
				if (ev.kbd.keycode == Common::KEYCODE_PLUS)
					cur.setStyle(++st);
				if (ev.kbd.keycode == Common::KEYCODE_MINUS)
					cur.setStyle(--st);
				if (ev.kbd.keycode == Common::KEYCODE_RETURN) {
					warning("bg: %s", (*i_bg)->getName().c_str());
					Common::SeekableReadStream *stream = (*i_bg)->createReadStream();
					BackGround bg(stream);
					bg.show();
					delete stream;
					i_bg++;
				}
			}
		}
		_system->updateScreen();
		_system->delayMillis(10);
	}

	return Common::kNoError;
}

} // End of namespace Express
