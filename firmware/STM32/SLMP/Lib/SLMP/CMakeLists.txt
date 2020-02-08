cmake_minimum_required(VERSION 3.15)
project(SLMP)

set(CMAKE_CXX_STANDARD 11)

add_executable(SLMP main.cpp SLMPPacket.h constants.h fields/PacketFields.h fields/fieldsProcessing.h SLMPRequestBuilder.h SLMPResponseParser.h SLMPRequestBuilder.cpp SLMPResponceParser.cpp)