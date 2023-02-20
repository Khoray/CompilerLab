重庆大学 2022 编译原理教改项目

目录结构：
/bin		可执行文件 + 库文件
/build		构建项目的文件
/include	头文件
/src		源代码
/src/...	子模块: IR, frontend, backend, opt, tools
            third_party: 第三方库, 目前使用了 jsoncpp 用于生成和读取 json
/docs       各种文档
/examples   ###TODO 一些示例程序
/examples/...
/CMakeList.txt
/readme.txt	


编译：
首先进入 /build 若CMakeList修改后应执行 cmake 命令
1. cd /build
2. cmake ..
如果一切正常没有报错 执行make命令
3. make


执行：
1. cd /bin
2. compiler <src_filename> [-step] -o <output_filename> [-O1]
    -step: 支持以下几种输入
        s0: 词法结果 token 串
        s1: 语法分析结果语法树, 以 json 格式输出
        s2: 语义分析结果, 以 IR 程序形式输出                  ### TODO
        -S: RISC-v 汇编                                     ### TODO

测试:
1. cd /test
2. python [files]:
    build.py:   进入到 build 目录, 执行 cmake .. & make     ### TODO
    
    run.py: 运行可执行文件 compiler 编译所有测试用例, 打印 compiler 返回值和报错, 输出编译结果至 /test/out
        执行方法: python run.py [s0/s1/s2/S]
    
    test.py: 将 run.py 生成的编译结果与标准结果进行对比并打分   ### TODO