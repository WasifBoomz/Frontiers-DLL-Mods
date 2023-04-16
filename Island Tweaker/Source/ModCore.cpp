#include "ModCore.h"
bool Contains(const char* array[], int size, const char* item) {
	for (int i = 0; i < size; i++) {
		if (!std::strcmp(array[i], item)) {
			return true;
		}
	}
	return false;
}
bool Vector3::operator==(Vector3 const& mult) {
	return this->x == mult.x && this->y == mult.y && this->z == mult.z;
}
bool Vector3::operator!=(Vector3 const& mult) {
	return !(this->x == mult.x && this->y == mult.y && this->z == mult.z);
}
Vector3::Vector3(float mX, float mY, float mZ)
	:x(mX),
	y(mY),
	z(mZ)
{}
RangeSpawning& RangeSpawning::operator*=(float const& mult) {
	//const float multMax = configuration::popInStabilityMode ? 1000.0f : 10000.0f;
	//this->rangeIn = std::clamp(this->rangeIn * mult, 0.0f, max(this->rangeIn, multMax));
	//this->rangeOut = std::clamp(this->rangeOut * mult, 0.0f, max(this->rangeOut, multMax));
	this->rangeIn *= mult;
	this->rangeOut *= mult;
	return *this;
}
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
