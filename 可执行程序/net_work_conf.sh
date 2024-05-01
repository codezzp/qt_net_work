#!/bin/bash

# 检查是否存在 net_work.tar.gz 文件
if [ ! -f "net_work.tar.gz" ]; then
    echo "Error: net_work.tar.gz 文件不存在"
    exit 1
fi

# 解压 net_work.tar.gz 文件到 /usr/local 目录下（强制覆盖）
tar -xzf net_work.tar.gz -C /usr/local/

# 修改 net_work 文件夹权限为 777
chmod 777 /usr/local/net_work

echo "解压和权限修改完成"

#配置启动脚本到全局环境
sudo cp start_net_work.sh /usr/local/bin

#赋予启动脚本权限
sudo chmod +x /usr/local/bin/start_net_work.sh

