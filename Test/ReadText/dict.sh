#!/bin/bash
ARGS=1
E_BADARGS=65
 
if [ $# -ne "$ARGS" ]
then
    echo "Usage:`basename $0` word"
    exit $E_BADARGS
fi
 
w3m -no-cookie -dump 'http://dict.baidu.com/s?wd='$1'&f=3'  \
| sed '/以下结果来自互联网网络释义/,$d'| sed '1,15d' | tac \
| sed '1,2d' | tac |sed -r '/^[0-9]+\./N;s/\n//' > /tmp/rxdict.tmp
 
echo
echo -e "--------------------\033[1;40;33m $1 \033[0m--------------------"
cat /tmp/rxdict.tmp
