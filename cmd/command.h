#ifndef COMMAND_H
#define COMMAND_H

#include <vector>
#include <string>
#include <iostream>
#include <functional>

//void ConfigFuncCB(std::string filePath, std::string content)
//用于启动写配置文件,filePath:配置文件路径 content:配置文件内容
using ConfigFuncCB = void(std::string, std::string);
//void SerialportFuncCB(std::string filePath, int second, int bps, bool sendWay)
//用于启动串口,filePath:配置文件路径 second:每隔多少秒输出一次 bps:波特率(9600,19200,115200) sendWay:选择 十六进制或者文本
using SerialportFuncCB = void(std::string, int, int, bool);

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
        for (int i = 1; i < argc_; ++i) {
            std::string arg = argv_[i];
            if (arg == "-c" || arg == "--config") {
                if (i + 1 < argc_) {
                    ConfigPath = argv_[++i];
                } else {
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
                if (i + 1 < argc_ && isPositiveInteger(argv_[i+1])) {
                    OuputPeriod = std::stoi(argv_[++i]);
                    if (OuputPeriod < 3) {
                        std::cerr << "second must greater than 3!" <<std::endl;
                        return 1;
                    }
                } else {
                    std::cerr << "--second option requires a positive integer argument." << std::endl;
                    return 1;
                }
            } else if (arg == "-b" || arg == "--bps") {
                if (i + 1 < argc_ && isPositiveInteger(argv_[i+1])) {
                    BPS = std::stoi(argv_[++i]);
                } else {
                    std::cerr << "--bps option requires a positive integer argument." << std::endl;
                    return 1;
                }
            } else if (arg == "-f" || arg == "--format") {
                if (i + 1 < argc_) {
                    std::string format = argv_[++i];
                    if (format == "text") {
                        OuputInText = true;
                    } else if (format == "hex") {
                        OuputInText = false;
                    } else {
                        std::cerr << "--format option requires 'hex' or 'text' as an argument. Found: " << format << std::endl;
                        return 1;
                    }
                } else {
                    std::cerr << "--format option requires one argument." << std::endl;
                    return 1;
                }
            } else {
                // 如果没有匹配到任何已知命令行参数
                std::cerr << "Unknown cmd args. Please check the args by -h or --help." << std::endl;
                return 1;
            }
        }

        // 根据获取的配置执行操作
        configCB_(ConfigPath, ConfigContent);
        serialportCB_(CurrentFileName, OuputPeriod, BPS, OuputInText);
        return 0;
    }
    // 打印帮助文档
    void PrintHelp() {
        std::cout << "Usage: program_name [options]\n";
        std::cout << "Options:\n";
        std::cout << "  -c, --config <path>\tSpecify the configuration file path(default: config)\n";
        std::cout << "  -h, --help \t\tDisplay this help message\n";
        std::cout << "  -s, --second <time>\tSet the period in seconds for output (default: 3)\n";
        std::cout << "  -b, --bps <rate>\tSet the baud rate for serial output (default: 9600)\n";
        std::cout << "  -f, --format <hex|text>\tChoose output format: Hexadecimal or Textual (default: text)\n";
        std::cout << "  -t, --text <content>\tSpecify the contents of the configuration file(default: hello)\n" << std::endl;
    }


public:
    //配置文件路径
    inline static std::string ConfigPath = "config";
    //配置文件内容
    inline static std::string ConfigContent = "hello";
    //当前文件路径
    inline static std::string CurrentFileName;
    //输出十六进制或者文本 默认文本
    inline static bool OuputInText = true;
    //输出到串口的周期 (每?秒一次) 默认一秒
    inline static int OuputPeriod = 3;
    //输出到串口的波特率 (默认9600)
    inline static int BPS = 9600;
    //串口名 /dev/ttyS1
    inline static std::string SerialPortName = "/dev/ttyS1";

private:
    int argc_;
    std::vector<std::string> argv_;
    std::function<ConfigFuncCB> configCB_;
    std::function<SerialportFuncCB> serialportCB_;
};

#endif // COMMAND_H
