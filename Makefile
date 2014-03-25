all : SHARED_OBJECTS

SHARED_OBJECTS : unique_id.so

unique_id.so : unique_id.c
	gcc -g -fPIC -I/usr/local/include -shared unique_id.c -o unique_id.so

clean :
	rm -fr *.so
