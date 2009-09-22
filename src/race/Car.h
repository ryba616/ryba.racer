/*
 * Car.h
 *
 *  Created on: 2009-09-08
 *      Author: chudy
 */

#ifndef CAR_H_
#define CAR_H_

#include "graphics/Stage.h"
#include "race/Checkpoint.h"
#include "graphics/Drawable.h"

#include <ClanLib/core.h>
#include <ClanLib/network.h>

class Level;

class Car: public Drawable {
	public:
		Car(float p_x, float p_y, float p_rotation = 0);
		virtual ~Car();

		virtual void draw(CL_GraphicContext &p_gc);

		int getLap() const { return m_lap; }

		const CL_Pointf& getPosition() const { return m_position; }

		float getRotation() const { return m_rotation.to_degrees(); }

		int prepareStatusEvent(CL_NetGameEvent &p_event);
		int applyStatusEvent(const CL_NetGameEvent &p_event, int p_beginIndex = 0);

		void setAcceleration(bool p_value) {
			m_acceleration = p_value;
#ifndef NDEBUG
			Stage::getDebugLayer()->putMessage(CL_String8("accel"),  CL_StringHelp::bool_to_local8(p_value));
#endif
		}

		void setBrake(bool p_value) {
			m_brake = p_value;
#ifndef NDEBUG
			Stage::getDebugLayer()->putMessage(CL_String8("brake"),  CL_StringHelp::bool_to_local8(p_value));
#endif
		}

		void setTurn(float p_value) {
			m_turn = normalize(p_value);
#ifndef NDEBUG
			Stage::getDebugLayer()->putMessage(CL_String8("turn"),  CL_StringHelp::float_to_local8(p_value));
#endif
		}

		void setPosition(const CL_Pointf &p_position) { m_position = p_position; }

		void setRotation(float p_rotation) { m_rotation.set_degrees(p_rotation); }

		void update(unsigned int elapsedTime);

		CL_Signal_v1<Car &> &sigStatusChange() { return m_statusChangeSignal; }

	private:

		/** Parent level */
		Level* m_level;

		/** Car sprite */
		CL_Sprite m_sprite;

		/** Central position on map */
		CL_Pointf m_position;

		/** Rotation in degrees CCW from positive X axis */
		CL_Angle m_rotation;

		/** Current turn. -1 is maximum left, 0 is center and 1 is maximum right */
		float m_turn;

		/** Acceleration switch */
		bool m_acceleration;

		/** Brake switch */
		bool m_brake;

		/** Move vector */
		CL_Vec2f m_moveVector;
		CL_Vec2f accelerationVector;
		CL_Vec2f forceVector;
		CL_Vec2f driftVector;

		/** Current speed */
		float m_speed;
		
		/** angle */
		float m_angle;

		/** Input state changed signal */
		CL_Signal_v1<Car &> m_statusChangeSignal;

		/** Input checksum */
		int m_inputChecksum;

		/** Lap number */
		int m_lap;

		/** Level checkpoints and pass state. Filled in by parent Level */
		std::vector<Checkpoint> m_checkpoints;

		/** Final lap checkpoint. Filled by parent Level */
		Checkpoint m_lapCheckpoint;

		int calculateInputChecksum() const;

		float normalize(float p_value) const;

		bool areAllCheckpointsPassed() const;

		bool resetCheckpoints();

		friend class Level;

};

inline float Car::normalize(float p_value) const {
	if (p_value < -1.0f) {
		p_value = 1.0f;
	} else if (p_value > 1.0f) {
		p_value = 1.0f;
	}

	return p_value;
}

#endif /* CAR_H_ */
