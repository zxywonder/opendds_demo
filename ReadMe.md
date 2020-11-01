

# 0 idl文件



# 1 idl编译

## 1.1 使用tao_idl.exe opendds_idl

条件：ACE_ROOT、DDS_ROOT、TAO_ROOT环境变量，并将bin添加到Path下

```
CD open_dds_idl
tao_idl.exe dds_demo.idl
opendds_idl.exe dds_demo.idl
```

此共生成如下九个文件

```
< filename >C.cpp
< filename >C.h
< filename >C.inl
< filename >S.cpp
< filename >S.h
< filename >S.inl
< filename >TypeSupport.idl
< filename >TypeSupportImpl.h
< filename >TypeSupportImpl.cpp
```

其中生成的TypeSupport.IDL文件中包括了TypeSupport, DataWriter 和DataReader的接口定义。这些特定的DDS类型接口将会在稍后注册数据类型，发布数据样本和接收数据样本时使用，而生成的cpp文件实现了这些接口。

使用tao_idl编译TypeSupport.idl

```
tao_idl.exe dds_demoTypeSupport.idl
```

## 1.2 使用MPC文件编译

编写*MPC文件，使用 mwc.pl 生成文件和工程文件。##需要设置CIAO_ROOT和DANCE_ROOT环境变量

```
project(*idl): dcps {
    TypeSupport_Files {
        Demo.idl
    }
    custom_only = 1
}
```

```
perl $(ACE_ROOT)\bin\mwc.pl -type vs2017
```

# 2发布和订阅

# 3程序运行

程序执行前需要先运行DDS的信息仓库

```
%DDS_ROOT%/bin/DCPSInfoRepo  -ORBListenEndpoints  iiop://localhost:1001
```

执行示例

```
open_dds_pub.exe -DCPSInfoRepo  corbaloc::localhost:1001/DCPSInfoRepo
```

