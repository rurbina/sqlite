dll : uid.dll es_mx.dll strmatch.dll
so  : uid.so  es_mx.so strmatch.so

uid.dll : uid.c
	gcc -g uid.c -shared -ouid.dll

uid.so : uid.c
	gcc -g -fPIC -I/usr/local/include -shared uid.c -o uid.so

strmatch.dll : strmatch.c
	gcc -g strmatch.c -shared -ostrmatch.dll

strmatch.so : strmatch.c
	gcc -g -fPIC -I/usr/local/include -shared strmatch.c -o strmatch.so

dice.dll : dice.c
	gcc -g dice.c -shared -odice.dll

dice.so : dice.c
	gcc -g -fPIC -I/usr/local/include -shared dice.c -o dice.so

es_mx.so : es_mx.c
	gcc -g -fPIC -I/usr/local/include -shared es_mx.c -o es_mx.so

es_mx.dll : es_mx.c
	gcc -g es_mx.c -shared -oes_mx.dll

testso : uid.so es_mx.so
	sqlite3 es_mx.db 'drop table test;' || echo 1
	sqlite3 -bail -cmd '.load ./es_mx.so' es_mx.db 'create table test(name collation es_mx); insert into test select name from src; select name,normalize(name) from test order by name collate es_mx asc; drop table test; '

testdll : uid.dll es_mx.dll
	sqlite3 es_mx.db 'drop table test;' || echo 1
	sqlite3 -bail -cmd '.load ./es_mx' -cmd '.load ./uid' es_mx.db 'create table test(name collation es_mx); insert into test select name from src; select name,normalize(name) from test order by name collate es_mx asc; drop table test; '

clean :
	rm -fr uid.so uid.dll es_mx.so es_mx.dll

