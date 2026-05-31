#pragma once

#include <string>
#include <stdint.h>

const uint32_t C_TcpPort = 9779;


const std::string C_handshake = "<HANDSHAKE>";
const std::string C_unknown = "<UNKNOWN>";
const std::string C_command = "<START COMMAND>";
const std::string C_stream = "<STREAM>";
const std::string C_commandOK = "<COMMAND OK>";
const std::string C_end = "<END>";
const std::string C_OK = "<OK>";
const std::string C_read_osd = "<READ_OSD>";
const std::string C_write_osd = "<WRITE_OSD>";
const std::string C_osd = "<OSD>";
const std::string C_item_data = "<DATA>";

static int server_fd;
