//
// Created by data-scientist on 21.01.2020.
//

#ifndef SLMP_SLMPREQUESTBUILDER_H
#define SLMP_SLMPREQUESTBUILDER_H

#include "SLMPPacket.h"

struct Msg {
	unsigned char *content;
	unsigned int len;
};

Msg buildRequest(SLMPPacket *packet);
unsigned int getMsgLen(SLMPPacket *packet);
#endif //SLMP_SLMPREQUESTBUILDER_H
