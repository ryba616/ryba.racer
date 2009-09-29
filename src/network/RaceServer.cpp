/*
 * RaceServer.cpp
 *
 *  Created on: 2009-09-27
 *      Author: chudy
 */

#include "network/RaceServer.h"

#include <assert.h>

#include "common.h"
#include "network/events.h"
#include "network/Server.h"

RaceServer::RaceServer(Server* p_server) :
	m_initialized(false),
	m_server(p_server),
	m_slotsConnected(false)
{
}

RaceServer::~RaceServer()
{
}

void RaceServer::initialize()
{
	if (m_initialized) {
		assert(0 && "already initialized");
	}

	if (!m_slotsConnected) {
		m_slots.connect(m_server->signalPlayerConnected(), this, &RaceServer::slotPlayerConnected);
		m_slots.connect(m_server->signalPlayerDisconnected(), this, &RaceServer::slotPlayerDisconnected);

		m_slotsConnected = true;
	}

	RacePlayer* player;

	std::pair<CL_NetGameConnection*, Player*> pair;

	foreach (pair, m_server->m_connections) {
		player = new RacePlayer(pair.second);
		m_racePlayers[pair.first] = player;
	}

	m_initialized = true;
}

void RaceServer::destroy()
{
	if (!m_initialized) {
		assert(0 && "not initialized");
	}

	std::pair<CL_NetGameConnection*, RacePlayer*> pair;

	foreach (pair, m_racePlayers) {
		delete pair.second;
	}

	m_racePlayers.clear();
}

void RaceServer::slotPlayerConnected(CL_NetGameConnection *p_connection, Player *p_player)
{
	if (!m_initialized) {
		return;
	}

	m_racePlayers[p_connection] = new RacePlayer(p_player);
}

void RaceServer::slotPlayerDisconnected(CL_NetGameConnection *p_connection, Player *p_player)
{
	if (!m_initialized) {
		return;
	}

	std::map<CL_NetGameConnection*, RacePlayer*>::iterator itor = m_racePlayers.find(p_connection);

	assert(itor != m_racePlayers.end());

	delete itor->second;
	m_racePlayers.erase(itor);
}

void RaceServer::handleEvent(CL_NetGameConnection *p_connection, const CL_NetGameEvent &p_event)
{
	const CL_String eventName = p_event.get_name();

	if (eventName == EVENT_CAR_STATE_CHANGE) {
		handleCarStateChangeEvent(p_connection, p_event);
	}  else if (eventName == EVENT_TRIGGER_RACE_START) {
		handleTriggerRaceStartEvent(p_connection, p_event);
	} else {
		cl_log_event("error", "unhandled event: %1", eventName);
	}
}

void RaceServer::handleCarStateChangeEvent(CL_NetGameConnection *p_connection, const CL_NetGameEvent &p_event)
{
	cl_log_event("handling %1", p_event.to_string());

	m_server->sendToAll(p_event, p_connection);
}

void RaceServer::handleTriggerRaceStartEvent(CL_NetGameConnection *p_connection, const CL_NetGameEvent &p_event)
{
	cl_log_event("handling %1", p_event.to_string());
}
