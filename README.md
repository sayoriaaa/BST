# 二叉查找树

一个可以自动生成图像的二叉查找树

## Getting Started

*bst* requires:

* gcc
* make
* graphviz

you can build *bst* from repository by executing the following command:

```
make
```

## Commands

- -h：帮助
- -v：版本
- -r：随机产生数字，例 `-r 40` ，即产生40个节点的二叉树，数值为0-39
- -l：指定二叉树，例`-l 5 2 8 7 5 0`，即产生5个节点(-l后第一个数字)的二叉树，依次为2，8，7，5，0
- assert：例`assert 50`，插入数值为50的节点(图像自动更新)
- exit：退出

## Usage

通过-l或-r生成二叉树，然后在当前目录下自动生成bst.png。在程序运行时使用assert等命令，bst.png也会相应更改
