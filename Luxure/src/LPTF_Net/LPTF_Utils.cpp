#include <iostream>
#include <cstring>

#include "../../include/LPTF_Net/LPTF_Packet.hpp"


// repfrom is the packet type this reply refers to
LPTF_Packet build_reply_packet(uint8_t repfrom, void *repcontent, uint16_t contentsize) {
    uint8_t *rawcontent = (uint8_t*)malloc(sizeof(uint8_t)+contentsize);

    if (!rawcontent)
        throw std::runtime_error("Memory allocation failed !");

    memcpy(rawcontent, &repfrom, sizeof(uint8_t));
    memcpy(rawcontent + sizeof(uint8_t), repcontent, contentsize);

    LPTF_Packet packet(REPLY_PACKET, rawcontent, sizeof(uint8_t)+contentsize);

    free(rawcontent);
    return packet;
}


LPTF_Packet build_message_packet(const std::string &message) {
    LPTF_Packet packet(MESSAGE_PACKET, (void*)message.c_str(),
                       message.size()+1);     // include null term
    return packet;
}


LPTF_Packet build_command_packet(const std::string &cmd, const std::string &arg) {
    uint8_t *rawcontent = (uint8_t*)malloc(cmd.size() + arg.size() + 2);

    if (!rawcontent)
        throw std::runtime_error("Memory allocation failed !");
    
    memcpy(rawcontent, cmd.c_str(), cmd.size()+1);
    memcpy(rawcontent + cmd.size() + 1, arg.c_str(), arg.size()+1);

    LPTF_Packet packet(COMMAND_PACKET, rawcontent,
                       cmd.size() + arg.size() + 2);
    free(rawcontent);
    return packet;
}


// errfrom is the packet type this error refers to
LPTF_Packet build_error_packet(uint8_t errfrom, const std::string &errmsg) {
    uint8_t *rawcontent = (uint8_t*)malloc(sizeof(uint8_t)+errmsg.size()+1);

    if (!rawcontent)
        throw std::runtime_error("Memory allocation failed !");
    
    memcpy(rawcontent, &errfrom, sizeof(uint8_t));
    memcpy(rawcontent + sizeof(uint8_t), errmsg.c_str(), errmsg.size()+1);

    LPTF_Packet packet(ERROR_PACKET, rawcontent,
                       sizeof(uint8_t)+errmsg.size()+1);
    free(rawcontent);
    return packet;
}