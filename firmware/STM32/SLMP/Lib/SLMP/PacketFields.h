//
// Created by data-scientist on 18.01.2020.
//

#ifndef SLMP_PACKETFIELDS_H
#define SLMP_PACKETFIELDS_H

#include <algorithm>
#include <array>
#include <vector>

template<typename ValueType, unsigned int DumpSize>
struct Field {
	union U {
		ValueType value;
		std::array<std::uint8_t, DumpSize> raw;
	} data;

	bool endian; // 0 - big, 1 - little
	bool exist = false;
public:
	Field() {}
	Field(ValueType value, bool endian) {
		static_assert(sizeof(ValueType) >= DumpSize, "sizeof(ValueType) must be bigger than DumpSize or equal it");
		data.value = value;
		this->endian = endian;
	}

	void swapEndian() {std::reverse(data.raw.begin(), data.raw.end());}
	void putInDump(unsigned char * begin) {std::copy(data.raw.begin(), data.raw.end(), begin);}
	void getFromDump(unsigned char * begin) {std::copy(begin, begin + DumpSize, data.raw.begin());}
	unsigned int getSize() {return DumpSize;}
	void setValue(ValueType value) {
		this.value = value;

		if (endian) {swapEndian();}
	}

	ValueType getValue() {
		if (endian) {swapEndian();}

		ValueType tmp = this->data.value;

		if (endian) {swapEndian();}

		return tmp;
	}
};

struct ValueField {
	std::vector<unsigned char> value;
	bool exist;
public:
	ValueField(std::vector<unsigned char> value): value(value) {}
	ValueField() {}
	void putInDump(unsigned char * begin) {std::copy(value.begin(), value.end(), begin);}
	void getFromDump(unsigned char * begin, unsigned content_len) {
		value.resize(content_len);
		std::copy(begin, begin + value.size(), value.begin());
	}
	void setValue(std::vector<unsigned char> value) {this->value = value;}
	unsigned short getSize() {return value.size();}
	std::vector<unsigned char> getValue() {return value;}
};
#endif //SLMP_PACKETFIELDS_H
