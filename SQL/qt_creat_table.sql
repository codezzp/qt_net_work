insert into qt_uart VALUES(2,null,'config_1','hhhh');#测试代码

SELECT * FROM qt_uart;//#查看当前记录

create trigger before_insert BEFORE INSERT on qt_uart  #创建触发器，设置时间为当前时间
for each row 
set new.date = NOW();

