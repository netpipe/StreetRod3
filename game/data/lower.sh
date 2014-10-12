#!/bin/bash

if [ -n "$1" ]
then
if [ -d "$1" ]
then cd “$1″
else
echo Invalid Directory
exit
fi
fi

for x in `find . -type d`
do new=`echo $x | tr ‘[:upper:]‘ ‘[:lower:]‘`
mv $x $new
done

for x in `find . -type f`
do new=`echo $x | tr ‘[:upper:]‘ ‘[:lower:]‘`
mv $x $new
done
