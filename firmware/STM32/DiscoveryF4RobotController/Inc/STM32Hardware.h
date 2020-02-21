/* 
 * Software License Agreement (BSD License)
 *
 * Copyright (c) 2018, Kenta Yonekura (a.k.a. yoneken)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer in the documentation and/or other materials provided
 *    with the distribution.
 *  * Neither the name of Willow Garage, Inc. nor the names of its
 *    contributors may be used to endorse or promote prducts derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef ROS_STM32_HARDWARE_H_
#define ROS_STM32_HARDWARE_H_

#include "stm32f4xx_hal.h"

#include "SocketClient.h"

extern SocketClient socket_client;


class STM32Hardware {
  protected:


    const static uint16_t rbuflen = 1024;
    uint8_t rbuf[rbuflen];
    uint32_t rdmaInd;
    uint32_t rind;


    const static uint16_t tbuflen = 1024;
    uint8_t tbuf[tbuflen];
    uint32_t twind, tfind;

  public:
    STM32Hardware():
      rind(0), twind(0), tfind(0){
    }

    void init(){

    }

    void reset_rbuf(void){

    }

    int read_stm32hw(){
    	uint8_t c = -1;
		socket_client.socket_receive(&c, (uint16_t)1, &rdmaInd);
		if(rdmaInd > 0){
			return c;
		} else{
			return -1;
		}
    }


    void write_stm32hw(uint8_t* data, uint16_t length) {
    	socket_client.socket_send(data, length);
    }

    unsigned long time()
    {
//    	return HAL_GetTick();
    	return osKernelSysTick();
    }

  protected:
};

#endif
