#pragma once


#include <thread>
#include <iostream>
#include <sstream>
#include <string>

#include "m_globals.h"

std::thread* stream_thread = nullptr;
std:: string command;

void splitString(std::string& input, char delimiter,
                 std::string arr[], int& index)
{
    std::istringstream stream(input);

    std::string token;
    while (std::getline(stream, token, delimiter)) {
        arr[index++] = token;
    }
}

std::string run(const std::string& command, bool no_sh = false){
    std::string l_command(command);
    std::string data;
    FILE * stream;
    char buffer[100];

    if(!no_sh){
        l_command.append(".sh 2>&1");
    }
    std::cout<<"run: "<<l_command<<std::endl;

    stream = popen(l_command.data(), "r");

    if (stream) {
        while (!feof(stream))
            if (fgets(buffer, 100, stream) != NULL) {
                data.append(buffer);
            }
        pclose(stream);
    }
    return data;
}

const std::string C_stream_link = "data/stream.conf";

void start_stream(std::string ip, std::string port, bool on){
    if(stream_thread != nullptr || !on){
        std::string kill_cmd = "kill $(pgrep -f 'port=" + port + "')";
        std::system(kill_cmd.data());

        if(stream_thread != nullptr){
            if(stream_thread->joinable()){
                stream_thread->join();
            }
            delete stream_thread;
            stream_thread = nullptr;
        }
    }
    if(!on){
        return;
    }

    std::string stream_link = run("cat "+C_stream_link, true);


    command = "/usr/bin/gst-launch-1.0 rtspsrc location='";
    command += stream_link;
    command +=  "'";
    command += " latency=0 protocols=udp ! rtph264depay ! h264parse ! rtph264pay config-interval=1 pt=96 ! udpsink";
    command += " host=" + ip;
    command += " port=" + port;
    command += " sync=false async=false";
    command += " 2>&1";
    stream_thread = new std::thread([](){
        system(command.data());
    });
    stream_thread->detach();

}

void handle_command(const std::string& command){
    std::string cmd = command.substr(C_command.size(), command.size()-C_command.size());
    cmd = cmd.substr(0, cmd.size()-C_end.size());
    std::replace( cmd.begin(), cmd.end(), ' ', '/');
    std::string res = run(cmd);
    std::cout<<"Handle command "<<cmd<<" res:"<<res<<std::endl;

}

const std::string C_osd_filename = "data/osd.conf";

std::string read_osd(){
    std::string osd_data = run("cat "+C_osd_filename, true);
    if(osd_data.size() >= C_OK.size() && osd_data.substr(0, C_OK.size()) == C_OK){
        return osd_data;
    }
    return "";
}

void write_osd(const std::string& command){
    run("rm "+C_osd_filename, true);
    std::string write_cmd = "echo \"";

    std::string data = command.substr(C_write_osd.size(), command.size()-C_write_osd.size());
    data = data.substr(0, data.size()-C_end.size());
    
    write_cmd.append(C_OK);
    write_cmd.append("\n");
    write_cmd.append(data);
    write_cmd.append("\">>");
    write_cmd.append(C_osd_filename);

    std::cout<<write_cmd<<std::endl;

    run(write_cmd, true);
}

const std::string C_itemData_filename = "data/itemData.json";

std::string read_itemData(){
    std::string osd_data = run("cat "+C_itemData_filename, true);
    if(osd_data.size() >= C_OK.size() && osd_data.substr(0, C_OK.size()) == C_OK){
        return osd_data;
    }
    return "";
}
