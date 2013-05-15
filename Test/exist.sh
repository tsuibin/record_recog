if test $( pgrep -f $1 | wc -l ) -eq 0
then
	echo "进程不存在"
else
	echo "存在进程"
fi
