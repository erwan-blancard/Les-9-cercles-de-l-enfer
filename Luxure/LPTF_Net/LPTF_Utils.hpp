#ifndef LPTF_UTILS_H
#define LPTF_UTILS_H

#include <iostream>

#include "LPTF_Packet.hpp"

LPTF_Packet build_reply_packet(uint8_t repfrom, void *repcontent, uint16_t contentsize);
LPTF_Packet build_message_packet(const std::string &message);
LPTF_Packet build_command_packet(const std::string &cmd, const std::string &arg);
LPTF_Packet build_error_packet(uint8_t errfrom, const std::string &errmsg);

#endif