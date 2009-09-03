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

namespace Express {

// Titles of the games
static const PlainGameDescriptor ExpressGameTitles[] = {
	{"express", "The Last Express"},
	{0, 0}
};

// Game descriptions
static const ADGameDescription ExpressGameDescriptions[] = {
	// The Last Express - English Demo
	{"express", "Demo", {
		{"demo.hpf", 0, "baf3b1f64155d34872896e61c3d3cb78", 58191872},
		{NULL, 0, NULL, 0}}, Common::EN_ANY, Common::kPlatformPC, ADGF_DEMO, Common::GUIO_NONE},

	// The Last Express - Spanish (from jvprat)
	{"express", "", {
		{"hd.hpf", 0, "46bed8832f06cf7160883a2aae2d667f", 29657088},
		{NULL, 0, NULL, 0}}, Common::ES_ESP, Common::kPlatformPC, 0, Common::GUIO_NONE},

	AD_TABLE_END_MARKER
};

// Generic entries for fallback detection
static const ADGameDescription ExpressGameGeneric[] = {
	{"express", 0, {{NULL, 0, NULL, 0}}, Common::UNK_LANG, Common::kPlatformUnknown, 0, Common::GUIO_NONE},
	AD_TABLE_END_MARKER
};

// File based fallback detection
static const ADFileBasedFallback ExpressFileBasedFallback[] = {
	{&ExpressGameGeneric[0], {"hd.hpf", NULL}},
	{NULL, {NULL}}
};

static const ADParams detectionParams = {
	// Pointer to ADGameDescription or its superset structure
	(const byte *)ExpressGameDescriptions,
	// Size of that superset structure
	sizeof(ADGameDescription),
	// Number of bytes to compute MD5 sum for
	5000,
	// List of all engine targets
	ExpressGameTitles,
	// Structure for autoupgrading obsolete targets
	0,
	// Name of single gameid (optional)
	"express",
	// List of files for file-based fallback detection (optional)
	ExpressFileBasedFallback,
	// Flags
	0,
	// GUI Options
	Common::GUIO_NOSUBTITLES | Common::GUIO_NOMIDI
	//GUIO_NOLAUNCHLOAD?
};

class ExpressMetaEngine : public AdvancedMetaEngine {
public:
	ExpressMetaEngine() : AdvancedMetaEngine(detectionParams) {}

	const char *getName() const {
		return "The Last Express Engine";
	}

	const char *getOriginalCopyright() const {
		return "1997 (C) Smoking Car Productions";
	}

	bool createInstance(OSystem *syst, Engine **engine, const ADGameDescription *gd) const;
};

bool ExpressMetaEngine::createInstance(OSystem *syst, Engine **engine, const ADGameDescription *gd) const {
	*engine = new Express::ExpressEngine(syst, gd);

	return (*engine != NULL);
}

} // End of namespace Express

#if PLUGIN_ENABLED_DYNAMIC(EXPRESS)
	REGISTER_PLUGIN_DYNAMIC(EXPRESS, PLUGIN_TYPE_ENGINE, Express::ExpressMetaEngine);
#else
	REGISTER_PLUGIN_STATIC(EXPRESS, PLUGIN_TYPE_ENGINE, Express::ExpressMetaEngine);
#endif
