#!/bin/bash

ARCH=`uname -m`
echo $ARCH

mkdir ~/.msc
if [ "$ARCH"x != "x86_64"x ]
then
	cp -R ./bin/i686/ ~/.msc/i686
	echo -e "\nexport LD_LIBRARY_PATH += ~/.msc/i686" >> ~/.bashrc
else
	cp -R ./bin/x86_64/ ~/.msc/x86_64
	echo -e "\nexport LD_LIBRARY_PATH += ~/.msc/x86_64" >> ~/.bashrc
fi

source ~/.bashrc
