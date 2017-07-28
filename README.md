# real-time_video
1.环境配置：
	1）需要安装ffmpeg
	2) 需要安装opencv
	3) 需要安装boost
2.代码编译方法
cd build
cmake ..
make

3.使用方法
电脑端：
./build/bin/testNetWork   //程序写死监听8888端口

树莓派（安装了Camkit）：
cktool -s 15 -a 192.168.37.37 -p 8888
