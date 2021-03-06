/*
 * Copyright (c) 2009, Piotr Korzuszek
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the <organization> nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include <list>
#include <ClanLib/display.h>

#include "common/GroundBlockType.h"
#include "gfx/race/ui/RaceUI.h"
#include "gfx/Viewport.h"

namespace Race {
	class Block;
	class Car;
	class RaceLogic;
}

namespace Gfx {

class Car;
class DecorationSprite;
class GroundBlock;
class Sandpit;
class Smoke;

class RaceGraphics {

	public:

		RaceGraphics(const Race::RaceLogic *p_logic);

		virtual ~RaceGraphics();

		void draw(CL_GraphicContext &p_gc);

		void load(CL_GraphicContext &p_gc);

		void update(unsigned p_timeElapsed);

	private:

		/** Logic with data for reading only */
		const Race::RaceLogic *m_logic;

		/** How player sees the scene */
		Gfx::Viewport m_viewport;

		/** Race scene interface */
		Gfx::RaceUI m_raceUI;

		/** FPS counter */
		unsigned m_fps, m_nextFps;

		/** Last fps count time */
		unsigned m_lastFpsRegisterTime;

		/** Logic car to gfx car mapping */
		typedef std::map<const Race::Car*, CL_SharedPtr<Gfx::Car> > TCarMapping;
		TCarMapping m_carMapping;

		/** Block types to gfx ground blocks */
		typedef std::map<Common::GroundBlockType, CL_SharedPtr<Gfx::GroundBlock> > TBlockMapping;
		TBlockMapping m_blockMapping;

		/** Car smoke clouds */
		typedef std::list< CL_SharedPtr<Gfx::Smoke> > TSmokeList;
		TSmokeList m_smokes;

		/** Decorations */
		typedef std::list< CL_SharedPtr<Gfx::DecorationSprite> > TDecorationList;
		TDecorationList m_decorations;

		/** Sandpits */
		typedef std::list< CL_SharedPtr<Gfx::Sandpit> > TSandpitList;
		TSandpitList m_sandpits;


		// initialize routines

		void loadGroundBlocks(CL_GraphicContext &p_gc);

		void loadDecorations(CL_GraphicContext &p_gc);

		void loadSandPits(CL_GraphicContext &p_gc);


		// update routines

		void updateViewport(unsigned p_timeElapsed);

		void updateSmokes(unsigned p_timeElapsed);


		// drawing routines

		void drawLevel(CL_GraphicContext &p_gc);

		void drawBackBlocks(CL_GraphicContext &p_gc);

		void drawForeBlocks(CL_GraphicContext &p_gc);

		void drawGroundBlock(CL_GraphicContext &p_gc, const Race::Block& p_block, size_t x, size_t y);

		void drawTireTracks(CL_GraphicContext &p_gc);

		void drawUI(CL_GraphicContext &p_gc);

		void drawCars(CL_GraphicContext &p_gc);

		void drawCar(CL_GraphicContext &p_gc, const Race::Car &p_car);

		void drawSmokes(CL_GraphicContext &p_gc);

		void drawSandpits(CL_GraphicContext &p_gc);


		void countFps();

		// helpers

		// FIXME: this is copy of Level helpers
		CL_Pointf real(const CL_Pointf &p_point) const;

		float real(float p_coord) const;
};

} // namespace

