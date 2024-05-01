#!/bin/bash

# 检查 net_work 程序是否存在
if [ -x "/usr/local/net_work/net_work" ]; then
    echo "net_work program found."
    # 调用 net_work 程序
    /usr/local/net_work/net_work
    echo "net_work 启动脚本执行完毕"
else
    echo "net_work program not found."
    exit 1
fi


echo "net_work 启动脚本执行完毕"
