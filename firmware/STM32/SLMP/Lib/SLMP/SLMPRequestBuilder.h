//
// Created by data-scientist on 21.01.2020.
//

#ifndef SLMP_SLMPREQUESTBUILDER_H
#define SLMP_SLMPREQUESTBUILDER_H

#include "SLMPPacket.h"

void buildRequest(SLMPPacket *packet, unsigned char *msg);
unsigned short getMsgLen(SLMPPacket *packet);
#endif //SLMP_SLMPREQUESTBUILDER_H
