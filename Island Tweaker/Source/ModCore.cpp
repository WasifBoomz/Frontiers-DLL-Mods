#include <iostream>
bool Contains(const char* array[], int size, const char* item) {
	for (int i = 0; i < size; i++) {
		if (!std::strcmp(array[i], item)) {
			return true;
		}
	}
	return false;
}