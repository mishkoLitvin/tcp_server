#pragma once


#include "m_functions.h"
#include "m_globals.h"
#include "m_server.h"

#include <sys/socket.h>


bool check_command(const std::string& buffer, const std::string& command){
    const bool end_ok = buffer.substr(buffer.size()-C_end.size(), C_end.size()) == C_end;
    if(!end_ok){
        std::cout<<"Read "<<buffer.size()<<" bytes. : "<<buffer<<"End check failed"<<std::endl;
        return false;
    }
    return buffer.size()>=command.size()
           && buffer.substr(0, command.size()) == command;
}

bool processCommand(const int client_socket, const std::string& buffer_str){
    if(check_command(buffer_str, C_handshake)){
        send(client_socket, C_handshake.data(), C_handshake.size(), 0);
        close(client_socket);
        return true;
    }

    if(check_command(buffer_str, C_command)){
        handle_command(buffer_str);
        send(client_socket, C_OK.data(), C_OK.size(), 0);
        close(client_socket);
        return true;
    }

    if(check_command(buffer_str, C_read_osd)){
        std::string osd_reply = C_osd;
        osd_reply.append(read_osd());
        osd_reply.append(C_end);

        send(client_socket, osd_reply.data(), osd_reply.size(), 0);
        close(client_socket);
        return true;
    }

    if(check_command(buffer_str, C_write_osd)){
        send(client_socket, C_write_osd.data(), C_write_osd.size(), 0);
        write_osd(buffer_str);
        close(client_socket);
        return true;
    }

    if(check_command(buffer_str, C_item_data)){
        std::string data_reply = C_item_data;
        data_reply.append(read_itemData());
        data_reply.append(C_end);
        send(client_socket, data_reply.data(), data_reply.size(), 0);

        close(client_socket);
        return true;
    }

    if(check_command(buffer_str, C_stream)){
        send(client_socket, C_commandOK.data(), C_commandOK.size(), 0);
        close(client_socket);
        std::string ip = inet_ntoa(client_addr.sin_addr);
        std::cout<<buffer_str<<"  "<<ip<<std::endl;
        std::string cmd = buffer_str.substr(C_stream.size(),
                                            buffer_str.size()-C_stream.size());
        cmd = cmd.substr(0, cmd.size()-C_end.size());
        std::string arrayOfSubStr[100];
        int index = 0;
        splitString(cmd, '_', arrayOfSubStr, index);
        if(index >= 3){
            start_stream(ip, arrayOfSubStr[2], arrayOfSubStr[1] == "ON");
        }
        return true;
    }

    return false;
}