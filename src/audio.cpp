#include <cstdlib>
#include <string>
#include <cstdio>
#include <iostream>

int get_yt_audio(std::string url, std::string &final_path) {
	/* 251 means .webm with opus codec at 48KHz */
	std::string command = "yt-dlp -f 251 --print after_move:filepath -P \"./temp\" " + url;
	FILE* fp = popen(command.c_str(),"r");

    if (fp == nullptr) {
        std::cerr << "Failed to run yt-dlp" << std::endl;
        return -1;
    }

    char buffer[128];
    if (fgets(buffer, sizeof(buffer), fp) != nullptr) {
        std::cout << "Received output: " << buffer << std::endl; 
    } else {
        std::cerr << "Failed to read output or no output received" << std::endl;
    }

    fclose(fp);
	final_path = buffer;
	return 1;
}