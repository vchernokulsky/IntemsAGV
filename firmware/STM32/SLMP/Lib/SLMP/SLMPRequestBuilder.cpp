//
// Created by data-scientist on 07->02->2020->
//

#include "SLMPRequestBuilder.h"

void buildRequest(SLMPPacket *packet, unsigned char *msg) {
	unsigned char *slider = msg;

	if (packet->is_serial_no.exist) {
		packet->is_serial_no.putInDump(slider);
		slider += 2;
	}

	if (packet->is_serial_no.getValue() != 0x0050) {
			packet->serial_no.putInDump(slider);
			packet->serial_no.exist = true;
			slider += 2;

			packet->field_If_serial_no.putInDump(slider);
			packet->field_If_serial_no.exist = true;
			slider += 2;
	}

	if (packet->request_dest_net_no.exist) {
		packet->request_dest_net_no.putInDump(slider);
		slider += 1;
	}
	if (packet->request_dest_station_no.exist) {
		packet->request_dest_station_no.putInDump(slider);
		slider += 1;
	}
	if (packet->request_processor.exist) {
		packet->request_processor.putInDump(slider);
		slider += 2;
	}
	if (packet->request_reserved.exist) {
		packet->request_reserved.putInDump(slider);
		slider += 1;
	}

	packet->data_length = Field<unsigned short, 2>(0xc + ((packet->value.exist) ? packet->value.getSize(): 0));
	//packet->data_length.setValue(12 + ((packet->value.exist) ? packet->value.getSize(): 0));

	packet->data_length.putInDump(slider);
	slider += 2;

	if (packet->monitoring_time.exist) {
		packet->monitoring_time.putInDump(slider);
		slider += 2;
	}
	if (packet->command.exist) {
		packet->command.putInDump(slider);
		slider += 2;
	}
	if (packet->subcommand.exist) {
		packet->subcommand.putInDump(slider);
		slider += 2;
	}
	if (packet->head_device_no.exist) {
		packet->head_device_no.putInDump(slider);
		slider += 3;
	}
	if (packet->device_code.exist) {
		packet->device_code.putInDump(slider);
		slider += 1;
	}
	if (packet->no_of_device_points.exist) {
		packet->no_of_device_points.putInDump(slider);
		slider += 2;
	}

	if (packet->value.exist) {
		packet->value.putInDump(slider);
		slider += 2;
	}
}

unsigned short getMsgLen(SLMPPacket* packet) {
	unsigned short len = 0;

	if (packet->is_serial_no.exist) {len += 2;}
	if (packet->serial_no.exist) {len += 2;}
	if (packet->field_If_serial_no.exist) {len += 2;}
	if (packet->request_dest_net_no.exist) {len += 1;}
	if (packet->request_dest_station_no.exist) {len += 1;}
	if (packet->request_processor.exist) {len += 2;}
	if (packet->request_reserved.exist) {len += 1;}
	if (packet->data_length.exist) {len += 2;}
	if (packet->monitoring_time.exist) {len += 2;}
	if (packet->command.exist) {len += 2;}
	if (packet->subcommand.exist) {len += 2;}
	if (packet->head_device_no.exist) {len += 3;}
	if (packet->device_code.exist) {len += 1;}
	if (packet->no_of_device_points.exist) {len += 2;}
	if (packet->end_code.exist) {len += 2;}
	if (packet->value.exist) {len += packet->value.getSize();}

	return len;
}
