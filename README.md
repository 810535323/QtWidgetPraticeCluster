# CarProducker 

>该项目为练习项目，其中主要练习内容为 SQL数据库的使用、XML文件的读写以及QT Widget编程

## 编译注意

    编译后需要将"attrib.xml"文件复制到编译文件夹下，否则程序无法找到"attrib.xml"

## 文件构成

## CarProducer.pro

- **mian.cpp** 主程序，先启动conndlg对话框选择连接的数据库，默认选择SQLITE。
- **MainWindow.cpp** 浏览界面，从数据库读取汽车生产商列表、汽车品牌，并根据选中的生产商和汽车品牌选择展示细节信息，可以进行增加、删除汽车品牌操作。

## conndlg.pri

- **/conndlg/conndlg.cpp** 数据库连接对话框UI及其功能，包含数据库创建与连接，写入默认数据。

## editdialog.pri

- **/editdialog/editdialog.cpp** 添加汽车品牌对话框UI及其功能，主要功能为将用户填写的数据写入XML文件与数据库中。

# 存在问题
  - 在删除数据后不会重新排序，汽车id唯一且固定。
  - 在linux环境下无法输入中文
  - 只能添加却无法删除汽车生产商
