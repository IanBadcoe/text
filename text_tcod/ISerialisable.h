#pragma once

#include <sstream>

class ISerialisable {
public:
	virtual void SerialiseTo(std::ostringstream& out) const = 0;
	virtual void SerialiseFrom(std::istringstream& in) = 0;
};

template <typename T>
std::ostringstream& operator<<=(std::ostringstream& out, const T& in) {
	out.write(reinterpret_cast<const char*>(&in), sizeof(T));

	return out;
}

template <typename T>
std::istringstream& operator>>=(std::istringstream& in, T& out) {
	in.read(reinterpret_cast<char*>(&out), sizeof(T));

	return in;
}
