#include "ModCore.h"
#include "ObjStructs.h"
ObjFanSpawner::ObjFanSpawner(u32 m_type, u32 m_place, float m_length, float m_velocity, float m_durOn, float m_durOff, float m_phase, bool m_eventDriven)
	:type(m_type),
	place(m_place),
	length(m_length),
	velocity(m_velocity),
	durOn(m_durOn),
	durOff(m_durOff),
	phase(m_phase),
	eventDriven(m_eventDriven)
{}