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

	bool exist = false;
public:
	Field() {}
	Field(ValueType value) {
		static_assert(sizeof(ValueType) >= DumpSize, "sizeof(ValueType) must be bigger than DumpSize or equal it");
		data.value = value;
	}
	void putInDump(unsigned char * begin) {std::copy(data.raw.begin(), data.raw.end(), begin);}
	void getFromDump(unsigned char * begin) {std::copy(begin, begin + DumpSize, data.raw.begin());}
	unsigned int getSize() {return DumpSize;}
	void setValue(ValueType value) {this.value = value;}

	ValueType getValue() {
		ValueType tmp = this->data.value;
		return tmp;
	}
};


//template<unsigned short Size>
//struct ValueField {
//	std::array<unsigned char, Size> value;
//	unsigned short num_used = 0;
//	bool exist;
//public:
//	//ValueField(std::vector<unsigned char, Size> value): value(value) {}
//	ValueField() {}
//	void putInDump(unsigned char * begin) {std::copy(value.begin(), value.end(), begin);}
//	void getFromDump(unsigned char * begin, unsigned content_len) {setValue(begin, begin + content_len);}
//	void setValue(unsigned char * begin, unsigned char * end) {
//		std::copy(begin, end, value.begin());
//		num_used = std::distance(begin, end);
//	}
//	unsigned short getSize() {return num_used;}
//	std::array<unsigned char, Size> getValue() {return value;}
//};


struct ValueField {
	std::vector<unsigned char> value;
	bool exist;
public:
	ValueField(std::vector<unsigned char> value): value(value) {}
	ValueField() {}
	void putInDump(unsigned char * begin) {std::copy(value.begin(), value.end(), begin);}
	void getFromDump(unsigned char * begin, unsigned content_len) {
		while(value.size()) {
			value.pop_back();
		}
		for(int i = 0; i < content_len; i++) {
			value.push_back(begin[i]);
		}
//		value.resize(content_len);
//		int size = value.size();
//		std::copy(begin, begin + value.size(), value.begin());
	}
	void setValue(std::vector<unsigned char> value) {this->value = value;}
	unsigned short getSize() {return value.size();}
	std::vector<unsigned char> getValue() {return value;}
};
#endif //SLMP_PACKETFIELDS_H
