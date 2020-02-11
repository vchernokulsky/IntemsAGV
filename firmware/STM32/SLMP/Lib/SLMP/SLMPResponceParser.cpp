//
// Created by data-scientist on 07.02.2020.
//

#include "SLMPResponseParser.h"

SLMPPacket parseResponse(unsigned char * msg, unsigned int msg_len) {
    SLMPPacket packet = SLMPPacket();
	unsigned char *slider = msg;

	packet.is_serial_no.getFromDump(slider);
	packet.is_serial_no.exist = true;
	slider += 2;

//	if (packet.is_serial_no.getValue() == 0x0050) {
//		packet.serial_no.getFromDump(slider);
//		slider += 2;
//	}
//	if (packet.is_serial_no.getValue() == 0x0050) {
//		packet.field_If_serial_no.getFromDump(slider);
//		slider += 2;
//	}

	packet.request_dest_net_no.getFromDump(slider);
	packet.request_dest_net_no.exist = true;
	slider += 1;

	packet.request_dest_station_no.getFromDump(slider);
	packet.request_dest_station_no.exist = true;
	slider += 1;

	packet.request_processor.getFromDump(slider);
	packet.request_processor.exist = true;
	slider += 2;

	packet.request_reserved.getFromDump(slider);
	packet.request_reserved.exist = true;
	slider += 2;

	packet.data_length.getFromDump(slider);
	packet.data_length.exist = true;
	slider += 2;

//	packet.end_code.exist = true;
//	packet.end_code.getFromDump(slider);
//	slider += 2;
//
//	if (packet.data_length.getValue() > packet.end_code.getSize() && packet.end_code.getValue() != 0) {
//		packet.value.exist = true;
//		packet.value.getFromDump(slider, packet.data_length.getValue() - packet.end_code.getSize());
//	}

    return packet;
}
