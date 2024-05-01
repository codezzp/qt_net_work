#使用之前需要用户配置 mysql8
#使用 mysql - uroot -p
#输入 密码登录msyql后命令行执行下面代码
#执行代码 source /path/qt_creat_table.sql
#path 是你的sql的具体位置
此sql 生成 串口数据发送，字段 id 标识符，date 发送时间， config_name 报文文件名 ，file 报文内容
before_insert 触发器，用于设置发送时间为当前时间