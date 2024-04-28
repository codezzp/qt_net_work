#ifndef COMMAND_H
#define COMMAND_H

#include <vector>
#include <string>
#include <iostream>
#include <functional>

//void ConfigFuncCB(std::string filePath, std::string content)
//用于启动写配置文件,filePath:配置文件路径 content:配置文件内容
using ConfigFuncCB = void(std::string, std::string);
//void SerialPortCB(std::string filePath, int second, int times)
//用于启动串口,filePath:配置文件路径 second:每隔多少秒输出一次 times:一共输出几次
using SerialportFuncCB = void(std::string, int, int);

class Command {
public:
    Command(int argc, char *argv[], ConfigFuncCB configCB, SerialportFuncCB serialportCB) {
        argc_ = argc;
        for (int i = 0; i != argc; ++i) {
            argv_.push_back(argv[i]);
        }
        configCB_ = configCB;
        serialportCB_ = serialportCB;
    }
    // 判断字符串是否是正整数
    bool isPositiveInteger(const std::string& s) {
        return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
    }
    //启动
    int Start() {
        // 检查命令行参数
        if (argc_ == 2) {
            std::string arg = argv_[1];
            if (arg == "-h" || arg == "--help") {
                // 打印帮助文档
                PrintHelp();
                return 0;
            }
        }

        // 处理其他命令行参数和逻辑...
        for (int i = 1; i < argc_; i++) {
            std::string arg = argv_[i];
            if (arg == "-c" || arg == "--config") {
                if (i + 1 < argc_) { // 确保不是在argv的末尾
                    ConfigPath = argv_[++i]; // 增加'i'，这样就不会把参数作为下一个argv[i]
                } else { // 没有参数
                    std::cerr << "--config option requires one argument." << std::endl;
                    return 1;
                }
            } else if (arg == "-t" || arg == "--text") {
                if (i + 1 < argc_) {
                    ConfigContent = argv_[++i];
                } else {
                    std::cerr << "--text option requires one argument." << std::endl;
                    return 1;
                }
            } else if (arg == "-s" || arg == "--second") {
                if (i + 1 < argc_) {
                    std::string nextArg = argv_[++i];
                    if (isPositiveInteger(nextArg)) {
                        OuputPeriod = std::stoi(nextArg);
                    } else {
                        std::cerr << "--second option requires a positive integer argument." << std::endl;
                        return 1;
                    }
                } else {
                    std::cerr << "--second option requires one argument." << std::endl;
                    return 1;
                }
            } else if (arg == "-n" || arg == "--number") { // 添加处理 -n 或 --number 参数的代码
                if (i + 1 < argc_) {
                    std::string nextArg = argv_[++i];
                    if (isPositiveInteger(nextArg)) {
                        OuputTimes = std::stoi(nextArg);
                    } else {
                        std::cerr << "--number option requires a positive integer argument." << std::endl;
                        return 1;
                    }
                } else {
                    std::cerr << "--number option requires one argument." << std::endl;
                    return 1;
                }
            } else {
                // 如果没有匹配到任何已知命令行参数
                std::cerr << "Unknown cmd args. Please check the args by -h or --help." << std::endl;
                return 1;
            }
        }

        configCB_(Command::ConfigPath, Command::ConfigContent);
        serialportCB_(Command::ConfigPath, Command::OuputPeriod, Command::OuputTimes);
        return 0;
    }
    // 打印帮助文档
    void PrintHelp() {
        std::cout << "Usage: program_name [options]\n";
        std::cout << "Options:\n";
        std::cout << "  -c, --config \tSpecify the configuration file directory\n";
        std::cout << "  -h, --help \tDisplay this help message\n";
        std::cout << "  -n, --number \tHow many times have to output(Default 1)\n";
        std::cout << "  -s, --second \tHow many seconds is output once(Default 1s)\n";
        std::cout << "  -t, --text \tSpecifies the contents of the configuration file" << std::endl;
    }

public:
    //配置文件路径
    inline static std::string ConfigPath = "config";
    //配置文件内容
    inline static std::string ConfigContent = "hello";
    //输出到串口的周期 (每?秒一次) 默认一秒
    inline static int OuputPeriod = 1;
    //输出到串口的次数 (默认一次)
    inline static int OuputTimes = 1;

private:
    int argc_;
    std::vector<std::string> argv_;
    std::function<ConfigFuncCB> configCB_;
    std::function<SerialportFuncCB> serialportCB_;
};

#endif // COMMAND_H
