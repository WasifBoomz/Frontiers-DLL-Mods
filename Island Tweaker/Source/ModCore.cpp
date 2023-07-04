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
	if(fmod(this->rangeIn,1)==0)
		this->rangeIn *= mult;
	if (fmod(this->rangeOut, 1) == 0)
		this->rangeOut *= mult;
	return *this;
}