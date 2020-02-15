//
// Created by data-scientist on 18.01.2020.
// all packet classes are in big little

#ifndef SLMP_SLMPPACKET_H
#define SLMP_SLMPPACKET_H

#include "PacketFields.h"


struct SLMPPacket {
	Field<unsigned short, 2> is_serial_no;
	Field<unsigned short, 2> serial_no;
	Field<unsigned char, 1> request_dest_net_no;
	Field<unsigned char, 1> request_dest_station_no;
	Field<unsigned short, 2> request_processor;
	Field<unsigned char, 1> request_reserved;
	Field<unsigned short, 2> monitoring_time;
	Field<unsigned short, 2> command;
	Field<unsigned short, 2> subcommand;
	Field<unsigned int, 3> head_device_no;
	Field<unsigned char, 1> device_code;
	Field<unsigned short, 2> no_of_device_points;
	Field<unsigned short, 2> end_code;
	ValueField value;
	//ValueField<100> value;

	SLMPPacket() {data_length.exist = true;}

private:
	Field<unsigned short, 2> data_length;
	Field<unsigned short, 2> field_If_serial_no;

	friend void buildRequest(SLMPPacket *packet, unsigned char *msg);
	friend void parseResponse(SLMPPacket *packet, unsigned char * msg);
	friend unsigned short getMsgLen(SLMPPacket *packet);
};

#endif //SLMP_SLMPPACKET_H
