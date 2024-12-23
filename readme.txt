由于文件较多，程序代码提交为zip压缩格式

且由于编写后面阶段的代码时修改了前面阶段的代码，我根据阶段将代码分成了四个分支

scanner为词法分析器

syntax是语法分析器

semantic是语义分析器

codemaker为中间代码生成器


运行方法：

后三个阶段的可执行文件皆为parser ，测试文件为test.txt 运行方法：./parser test.txt

词法分析阶段可执行文件：scanner 测试文件：test.c 运行方法：./scanner < test.c


编译方法：

统一，皆为：make
可在make前先删除上次的编译结果：make clean

运行环境：linux
