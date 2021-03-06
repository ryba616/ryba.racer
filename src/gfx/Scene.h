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

#include <ClanLib/gui.h>
#include <ClanLib/display.h>

#include "gfx/Stage.h"

class Scene : public CL_GUIComponent {

	public:

		Scene(CL_GUIComponent *p_parent) : CL_GUIComponent(p_parent), m_loaded(false) {
			set_visible(false),
			func_render().set(this, &Scene::onRender);
			set_geometry(CL_Rectf(0.0f, 0.0f, Gfx::Stage::getWidth(), Gfx::Stage::getHeight()));
		}

		virtual ~Scene() {}

		virtual void draw(CL_GraphicContext &p_gc) {};

		bool isLoaded() const { return m_loaded; }

		virtual void load(CL_GraphicContext &p_gc) { m_loaded = true; }

		virtual void update(unsigned p_timeElapsed) {}

	private:

		/** True when load() method was invoked */
		bool m_loaded;

		void onRender(CL_GraphicContext &p_gc, const CL_Rect &p_clipRect) { draw(p_gc); }

};
