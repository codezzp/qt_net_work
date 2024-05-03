### qt命令行参数(如果不提供参数则进入gui界面)

文档命令

```shell
./cmd -h 或者 ./cmd.exe -h
```

结果

```shell
Usage: program_name [options]
Options:
  -c, --config <path>	Specify the configuration file path(default: config)
  -h, --help 		Display this help message
  -s, --second <time>	Set the period in seconds for output (default: 3)
  -b, --bps <rate>	Set the baud rate for serial output (default: 9600)
  -f, --format <hex|text>	Choose output format: Hexadecimal or Textual (default: text)
  -t, --text <content>	Specify the contents of the configuration file(default: hello)
```

### py脚本参数

文档命令

```shell
python3 qt_verify.py -h
```

结果

```shell
usage: qt_verify.py [-h] [-b BAUD] -p PORT -f FILE

Serial Port Echo with Match

options:
  -h, --help            show this help message and exit
  -b BAUD, --baud BAUD  Specify the baud rate. Default is 9600.
  -p PORT, --port PORT  Specify the serial port. E.g., COM3 or /dev/ttyUSB0
  -f FILE, --file FILE  Specify the file path for matching content.
```

