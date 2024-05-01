#qt_ttysim

文件目录简介：
SQL----数据库配置
cmd----命令行版本
net_work----源代码
可执行程序----linux打包发布的程序


1.软件简介：
实现Linux版本下的仿真串口发送和接收指定报文，能编辑、 保存、打开报文，能通过命令行使用，可以持续发送也可以间断发送。可以指定文本发送和十六进制发送。


2.注意事项1：
Qt6.x版本已经没有自带mysql驱动，需要自己编译 ，下面是手动编译驱动教程：
windows:
https://blog.csdn.net/m0_52072919/article/details/130094915

linux(开发环境 ubuntu 20.04 , QT6.5.3)：
https://www.cnblogs.com/judes/p/12060752.html

3.如果出现mysql相关的问题（类如 mysql connection error) 点击取消即可， 这是因为没有mysql驱动，或者 没有修改 qt_mysql.h的具体参数
请用户根据自己的情况自行修改(使用db_qt)  使用数据库前先阅读/SQL/sql_readme

4
使用简介
4.1用户可以自行通过windows /linux qt 编译代码，生成执行文件 ,QT版本 qt6.5.3

windows自行编译的话需要将 mainwindow.cpp 159行代码 中的“/dev/”去掉


4.2 ！！！！（可执行文件只能支持linux版本）
也可将可执行文件克隆到 linux下面 （目前能运行的版本 ubuntu 20.04   , ubuntu 22.04）


4.2.1 前置工作 

串口通信需要串口对
需要现在windows创建虚拟串口（通过virtual serial ports driver)
参考：
https://blog.csdn.net/qq_17351161/article/details/89607458


以下操作均在linux环境下
直接给虚拟机配置串口（通过VMware设置 添加串行端口）
具体步骤
1.虚拟机关机 
2.选择该虚拟机，然后选择虚拟机 > 设置。
3.在硬件选项卡中，单击添加。
4.在添加硬件向导中，选择串行端口。
5.使用物理串口（不要自动检测）用 virtual serial ports driver创建的虚拟串口
6.单击完成将虚拟串行端口添加到虚拟机。
添加两个 分别使用 virtual serial ports driver创建的虚拟串口对
重启虚拟机

4.2.2 配置
使用 root 用户赋予 net_work_conf.sh执行权限
列如  chmod /path/net_work_conf.sh 777 
这里path是你的具体路径
该目录下执行 net_work_conf.sh （./net_work_conf.sh） 就可以自动配置 net_work 

4.2.3 启动
切换root用户 执行 start_net_work.sh 
列如：
root@zzp-virtual-machine:/# start_net_work.sh

(随便那个目录下都可以执行，配置到了/usr/local/bin）
(必须切换到root用户， 目前串口通信需要root权限)

5.功能介绍

5.1发送配置
有发送编辑框，可以编辑，保存报文，也可以通过打开文本文件读取报文
发送设置：
1.周期发送（定时发送） 可以指定发送间隔（单位ms)
2.次数启用 ,规定发送次数
3.格式选择：a.十六进制（字节） b.文本发送
4.文本框监视


5.2接收配置
有接收编辑框，可以显示，保存报文
1.格式选择：a.十六进制（字节） b.文本发送



5.3.串口配置
1.校验位
2.波特率
3.结束位
4.数据位
5.串口名
两个串口要通信，1，2，3，4需要一致


