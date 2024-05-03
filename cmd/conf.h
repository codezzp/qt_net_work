#ifndef CONF_H
#define CONF_H

#include <fstream>
#include <ctime>
#include <filesystem>
#include <sstream>
#include <iostream>
#include "command.h"

class ConfCore {
public:
    static bool StartConfigWrite(std::string filePath, std::string content) {
        time_t rawtime;
        struct tm* timeinfo;
        char buffer[80];

        // 获取当前的系统时间并转换为年月日的格式
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        strftime(buffer, sizeof(buffer), "_%Y%m%d", timeinfo);

        std::string dateSuffix = std::string(buffer);
        std::string modifiedFilePath = filePath + dateSuffix + ".txt";
        int fileIndex = 1;

        // 如果文件名已经存在则在文件名后添加（编号）
        while (std::filesystem::exists(modifiedFilePath)) {
            std::stringstream ss;
            ss << "(" << fileIndex << ")";
            modifiedFilePath = filePath + dateSuffix + ss.str() + ".txt";
            fileIndex++;
        }

        // 创建新文件并写入内容
        std::ofstream file(modifiedFilePath);
        if (!file) {
            std::cerr << "File couldn't be opened." << std::endl;
            return false;
        }

        file << content;
        file.close();
        Command::CurrentFileName = modifiedFilePath;
        return true;
    }
    static void Start(std::string filePath, std::string content) {
        StartConfigWrite(filePath, content);
    }
};

#endif // CONF_H
