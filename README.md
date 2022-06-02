# tcpserver 

>该项目为练习项目，其中主要练习内容为 TCP服务器、客户端以及多线程编程
# 文件构成

## tcoserver.pro
- **mian.cpp** 主程序
- **MainWindow.cpp** 主页面，提供开启服务端、客户端按钮，服务端端一时间只有一个，客户端不限制。

## server.pri
- **server/dialog.cpp** 服务端ui界面并创建timeserver对象。
- **server/timeserver.cpp** 创建tcp服务端，绑定端口并开始监听。
- **server/timethread.cpp** 当tcp客户端请求时调用incomingConnection函数，创建线程发送数据。

## client.pri
- **client/client.cpp** 服务端ui界面并创建tcp客户端，发送连接请求，获取传输过来的内容并改变当前窗口下的时间。

---
>注意关闭服务端请点击关闭按钮而不是右上角的X，否则主窗口识别不到关闭信号，服务器按钮一直不可用，需要重启软件。
