all : SHARED_OBJECTS
dll : uid.dll
so  : uid.so

SHARED_OBJECTS : uid.so uid.dll

uid.dll : uid.c
	gcc -g uid.c -shared -ouid.dll

uid.so : uid.c
	gcc -g -fPIC -I/usr/local/include -shared uid.c -o uid.so

clean :
	rm -fr uid.so uid.dll

