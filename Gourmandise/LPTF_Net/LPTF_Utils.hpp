#ifndef LPTF_UTILS_H
#define LPTF_UTILS_H

#include <iostream>

#include "LPTF_Packet.hpp"

LPTF_Packet build_reply_packet(uint8_t repfrom, void *repcontent, uint16_t contentsize);
LPTF_Packet build_message_packet(const std::string &message);
LPTF_Packet build_command_packet(const std::string &cmd, const std::string &arg);
LPTF_Packet build_error_packet(uint8_t errfrom, uint8_t err_code, std::string &errmsg);

std::string get_message_from_message_packet(LPTF_Packet &packet);
std::string get_command_from_command_packet(LPTF_Packet &packet);
std::string get_arg_from_command_packet(LPTF_Packet &packet);
uint8_t get_refered_packet_type_from_reply_packet(LPTF_Packet &packet);
std::string get_reply_content_from_reply_packet(LPTF_Packet &packet);
uint8_t get_refered_packet_type_from_error_packet(LPTF_Packet &packet);
uint8_t get_error_code_from_error_packet(LPTF_Packet &packet);
std::string get_error_content_from_error_packet(LPTF_Packet &packet);

#endif