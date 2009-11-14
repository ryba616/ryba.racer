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

#include "Level.h"

#include <assert.h>

#include "common.h"
#include "race/Checkpoint.h"

namespace Race {

Level::Level() :
	m_loaded(false),
	m_width(0),
	m_height(0)
{
}

void Level::initialize(const CL_String &p_filename)
{
	loadFromFile(p_filename);
}

void Level::destroy()
{
	m_blocks.clear();
	m_loaded = false;
}

Level::~Level() {
	m_blocks.clear();
}

CL_String8 Level::readLine(CL_File& p_file) {
	CL_String8 line = p_file.read_string_text("", "\n", false);
	p_file.seek(1, CL_File::seek_cur);

	return line;
}

void Level::loadFromFile(const CL_String& p_filename)
{
	assert(!m_loaded && "level is already loaded");

	try {
		CL_File file(p_filename, CL_File::open_existing, CL_File::access_read);


		CL_DomDocument document(file);
		const CL_DomElement root = document.get_document_element();

		// load meta element
		const CL_DomNode metaNode = root.named_item("meta");
		loadMetaElement(metaNode);

		// load level's content
		const CL_DomNode contentNode = root.named_item("content");

		// load track
		const CL_DomNode trackNode = contentNode.named_item("track");
		loadTrackElement(trackNode);

		// load track bounds
		const CL_DomNode boundsNode = contentNode.named_item("bounds");
		loadBoundsElement(boundsNode);


		// read level content

//		CL_String8 line;
//
//		line = readLine(file);
//
//		std::vector<CL_TempString> parts = CL_StringHelp::split_text(line, " ", true);
//
//		m_width = CL_StringHelp::text_to_int(parts[0]);
//		m_height = CL_StringHelp::text_to_int(parts[2]);
//
//		m_blocks = new Block[m_width * m_height];
//
//
//		for (int i = 0; i < m_height; ++i) {
//			line = readLine(file);
//
//			parts = CL_StringHelp::split_text(line, " ", true);
//
//			for (int j = 0; j < m_width; ++j) {
//				m_blocks[m_width * i + j] = Block(decodeBlock(parts[j]), BOX_WIDTH);
//			}
//
//		}
//
//		// read bounds num
//		line = readLine(file);
//		const int boundsCount = CL_StringHelp::text_to_int(line);
//
//		// read all bounds
//		float x1, y1, x2, y2;
//
//		for (int i = 0; i < boundsCount; ++i) {
//			line = readLine(file);
//			parts = CL_StringHelp::split_text(line, " ", true);
//
//			x1 = CL_StringHelp::text_to_float(parts[0]) * BOX_WIDTH;
//			y1 = CL_StringHelp::text_to_float(parts[1]) * BOX_WIDTH;
//			x2 = CL_StringHelp::text_to_float(parts[2]) * BOX_WIDTH;
//			y2 = CL_StringHelp::text_to_float(parts[3]) * BOX_WIDTH;
//
//			Bound bound(CL_LineSegment2f(CL_Vec2f(x1, y1), CL_Vec2f(x2, y2)));
//			m_bounds.push_back(bound);
//		}
//
//		// read start positions num
//		line = readLine(file);
//		const int startPositionsCount = CL_StringHelp::text_to_int(line);
//
//		// read start positions
//		float x,y;
//
//		for (int i = 0; i < startPositionsCount; ++i) {
//			line = readLine(file);
//			parts = CL_StringHelp::split_text(line, " ", true);
//
//			x = CL_StringHelp::text_to_float(parts[0]) * BOX_WIDTH;
//			y = CL_StringHelp::text_to_float(parts[1]) * BOX_WIDTH;
//
//			m_startPositions[i + 1] = CL_Pointf(x, y);
//		}


		file.close();


	} catch (CL_Exception e) {
		CL_Console::write_line(e.message);
	}

	m_loaded = true;


}

void Level::loadMetaElement(const CL_DomNode &p_metaNode)
{
	m_width = p_metaNode.select_int("size/width");
	m_height = p_metaNode.select_int("size/height");

	cl_log_event("race", "level size set to %1 x %2", m_width, m_height);
}

void Level::loadTrackElement(const CL_DomNode &p_trackNode)
{
	// build block type map
	typedef std::map<CL_String, Common::GroundBlockType> blockMap_t;
	blockMap_t blockMap;
	blockMap_t::iterator blockMapItor;

	blockMap["vert"] = Common::BT_STREET_VERT;
	blockMap["horiz"] = Common::BT_STREET_HORIZ;
	blockMap["turn_bottom_right"] = Common::BT_TURN_BOTTOM_RIGHT;
	blockMap["turn_bottom_left"] = Common::BT_TURN_BOTTOM_LEFT;
	blockMap["turn_top_right"] = Common::BT_TURN_TOP_RIGHT;
	blockMap["turn_top_left"] = Common::BT_TURN_TOP_LEFT;

	// prepare level blocks
	const int blocksCount = m_width * m_height;
	m_blocks.clear();
	m_blocks.reserve(blocksCount);

	for (int i = 0; i < blocksCount; ++i) {
		m_blocks.push_back(CL_SharedPtr<Block>(new Block(Common::BT_GRASS)));
	}

	// read blocks
	const CL_DomNodeList blockList = p_trackNode.get_child_nodes();
	const int blockListSize = blockList.get_length();

	cl_log_event("debug", "Track node child count: %1", blockListSize);

	for (int i = 0; i < blockListSize; ++i) {
		const CL_DomNode blockNode = blockList.item(i);

		if (blockNode.get_node_name() == "block") {
			CL_DomNamedNodeMap attrs = blockNode.get_attributes();

			const int x = CL_StringHelp::local8_to_int(attrs.get_named_item("x").get_node_value());
			const int y = CL_StringHelp::local8_to_int(attrs.get_named_item("y").get_node_value());
			const CL_String typeStr = attrs.get_named_item("type").get_node_value();

			if (x < 0 || y < 0 || x >= m_width || y >= m_height) {
				cl_log_event("debug", "coords x=%1, y=%2", x, y);
				throw CL_Exception("Blocks coords out of bounds");
			}

			blockMapItor = blockMap.find(typeStr);

			if (blockMapItor != blockMap.end()) {
				m_blocks[m_width * y + x]->setType(blockMapItor->second);
			} else {
				cl_log_event("race", "Unknown block type: %1", typeStr);
			}

		} else {
			cl_log_event("race", "Unknown node '%1', ignoring", blockNode.get_node_name());
		}

	}

}

void Level::loadBoundsElement(const CL_DomNode &p_boundsNode)
{
	const CL_DomNodeList boundList = p_boundsNode.get_child_nodes();
	const int boundListSize = boundList.get_length();

	for (int i = 0; i < boundListSize; ++i) {
		const CL_DomNode boundNode = boundList.item(i);

		if (boundNode.get_node_name() == "bound") {
			CL_DomNamedNodeMap attrs = boundNode.get_attributes();

			float x1 = CL_StringHelp::local8_to_float(attrs.get_named_item("x1").get_node_value());
			float y1 = CL_StringHelp::local8_to_float(attrs.get_named_item("y1").get_node_value());
			float x2 = CL_StringHelp::local8_to_float(attrs.get_named_item("x2").get_node_value());
			float y2 = CL_StringHelp::local8_to_float(attrs.get_named_item("y2").get_node_value());

			x1 *= Block::WIDTH;
			y1 *= Block::WIDTH;
			x2 *= Block::WIDTH;
			y2 *= Block::WIDTH;

			cl_log_event("debug", "Loading bound %1 x %2 -> %3 x %4", x1, y1, x2, y2);

			const CL_LineSegment2f segment(CL_Pointf(x1, y1), CL_Pointf(x2, y2));
			m_bounds.push_back(CL_SharedPtr<Bound>(new Bound(segment)));
		} else {
			cl_log_event("race", "Unknown node '%1', ignoring", boundNode.get_node_name());
		}
	}
}

Common::GroundBlockType Level::decodeBlock(const CL_String8& p_str) {

	const char c = p_str[0];

	switch (c) {
		case '0':
			return Common::BT_GRASS;
		case '|':
			return Common::BT_STREET_VERT;
		case '-':
			return Common::BT_STREET_HORIZ;
		case '1':
			return Common::BT_TURN_BOTTOM_RIGHT;
		case '2':
			return Common::BT_TURN_BOTTOM_LEFT;
		case '3':
			return Common::BT_TURN_TOP_LEFT;
		case '4':
			return Common::BT_TURN_TOP_RIGHT;
		case '5':
			return Common::BT_START_LINE;
		default:
			assert(0 && "unknown char");
	}

	return Common::BT_GRASS;

}

float Level::getResistance(float p_x, float p_y) {
	if (p_x < 0 || p_y < 0 || p_x >= Block::WIDTH * m_width || p_y >= Block::WIDTH * m_height) {
		return 0.0f;
	}

	int blockX = (int) floor(p_x / Block::WIDTH);
	int blockY = (int) floor(p_y / Block::WIDTH);

	int localX = (int) (p_x - blockX * Block::WIDTH);
	int localY = (int) (p_y - blockY * Block::WIDTH);

	return m_blocks[blockY * m_width + blockX]->getResistance(localX, localY);
}

void Level::addCar(Car *p_car) {

	assert(m_loaded && "Level is not loaded");

	p_car->m_level = this;

	// fill car checkpoints
	for (int x = 0; x < m_width; ++x) {
		for (int y = 0; y < m_height; ++y) {

			const Block &block = getBlock(x, y);

			if (block.getType() != Common::BT_GRASS) {

				const CL_Rectf rect(x * Block::WIDTH, y * Block::WIDTH, (x + 1) * Block::WIDTH, (y + 1) * Block::WIDTH);
				const Checkpoint checkpoint(rect);

				p_car->m_checkpoints.push_back(checkpoint);

			}

			// if this is a start/finish line, then add finish line checkpoint
			if (block.getType() == Common::BT_START_LINE) {
				const CL_Rectf rect(x * Block::WIDTH, (y - 1) * Block::WIDTH, (x + 1) * Block::WIDTH, y * Block::WIDTH);
				p_car->m_lapCheckpoint = Checkpoint(rect);
			}
		}
	}


	m_cars.push_back(p_car);
	m_carsDriftPoints[p_car] = new CL_Pointf[4];
}

void Level::removeCar(Car *p_car) {
	for (
		std::vector<Car*>::iterator itor = m_cars.begin();
		itor != m_cars.end();
		++itor
	) {
		if (*itor == p_car) {
			m_cars.erase(itor);
			break;
		}
	}

	p_car->m_level = NULL;

	delete[] m_carsDriftPoints[p_car];
	m_carsDriftPoints.erase(m_carsDriftPoints.find(p_car));
}

CL_Pointf Level::getStartPosition(int p_num) const {

	std::map<int, CL_Pointf>::const_iterator startPositionItor = m_startPositions.find(p_num);

	if (startPositionItor != m_startPositions.end()) {
		return startPositionItor->second;
	} else {
		return CL_Pointf(200, 200);
	}

}

void Level::update(unsigned p_timeElapsed)
{
#ifdef CLIENT

	// check collisions
	checkCollistions();

#ifndef NO_TYRE_STRIPES
	foreach (Car* car, m_cars) {

		CL_Pointf* lastDriftPoints = m_carsDriftPoints[car];

		const CL_Pointf &carPosition = car->getPosition();

		if (car->isDrifting()) {

			static const float tyreRadius = 10.0f;
			CL_Angle carAngle(car->getRotationRad(), cl_radians);

			CL_Vec2f v;
			float rad;

			for (int i = 0; i < 4; ++i) {

				carAngle += CL_Angle(i == 0 ? 45 : 90, cl_degrees);

				rad = carAngle.to_radians();

				v.x = cos(rad);
				v.y = sin(rad);

				v.normalize();

				v *= tyreRadius;

				CL_Pointf stripePointEnd(carPosition);
				stripePointEnd += v;

				// when last drift point is valid, then add the tire stripe
				// if not, only update the drift point
				if (lastDriftPoints[i].x != 0.0f && lastDriftPoints[i].y != 0.0f) {

					CL_Pointf stripePointStart(lastDriftPoints[i]);

					m_tyreStripes.add(stripePointStart, stripePointEnd, car);
				}

				lastDriftPoints[i].x = stripePointEnd.x;
				lastDriftPoints[i].y = stripePointEnd.y;

			}

		} else {
			// nullify all tires drift positions when no drift is done
			for (int i = 0; i < 4; ++i) {
				lastDriftPoints[i].x = 0;
				lastDriftPoints[i].y = 0;
			}
		}
	}
#endif // !NO_TYRE_STRIPES
#endif // CLIENT
}

#ifdef CLIENT
void Level::checkCollistions()
{
	CL_CollisionOutline coll1, coll2;


	foreach (Car *c1, m_cars) {
		coll1 = c1->calculateCurrentCollisionOutline();

		// check car collisions
		// TODO: later :-)
//		foreach (Car *c2, m_cars) {
//
//			if (c1 == c2) {
//				continue;
//			}
//
//			coll2 = c2->calculateCurrentCollisionOutline();
//
//			if (coll1.collide(coll2)) {
//				cl_log_event("debug", "collision");
//			}
//		}

		// check bounds collisions
		foreach (const CL_SharedPtr<Bound> &bound, m_bounds) {
			if (coll1.collide(bound->getCollisionOutline())) {
				c1->performBoundCollision(*bound);
			}
		}
	}

}
#endif // CLIENT

} // namespace
