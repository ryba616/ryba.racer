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

#include "boost/utility.hpp"

#include "common/GroundBlockType.h"

namespace Race {

class Block : public boost::noncopyable
{
	public:

		static const int WIDTH;

		Block(Common::GroundBlockType p_type);

		virtual ~Block();

		float getResistance(int x, int y) const;

		void setType(Common::GroundBlockType p_type) { m_type = p_type; }

		Common::GroundBlockType getType() const { return m_type; }

	private:
		/** Subtype of this block */
		Common::GroundBlockType m_type;

		/** Resistance map */
		float *m_resistanceMap;

};

class PixelTranslator {
	public:
		virtual void translate(int &p_x, int &p_y, int p_width, int p_height) {
		}
};

class PixelTranslator270 : public PixelTranslator {
	public:
		virtual void translate(int &p_x, int &p_y, int p_width, int p_height) {
			int tmp = p_x;
			p_x = p_y;
			p_y = p_width - tmp - 1;
		}
};

class PixelTranslator180 : public PixelTranslator {
	public:
		virtual void translate(int &p_x, int &p_y, int p_width, int p_height) {
			p_x = p_width - p_x - 1;
			p_y = p_height - p_y - 1;
		}
};

class PixelTranslator90 : public PixelTranslator {
	public:
		virtual void translate(int &p_x, int &p_y, int p_width, int p_height) {
			int tmp = p_x;
			p_x = p_height - p_y - 1;
			p_y = tmp;
		}
};

} // namespace
