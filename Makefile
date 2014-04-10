dll : uid.dll es_mx.dll
so  : uid.so  es_mx.so

uid.dll : uid.c
	gcc -g uid.c -shared -ouid.dll

uid.so : uid.c
	gcc -g -fPIC -I/usr/local/include -shared uid.c -o uid.so

es_mx.so : es_mx.c
	gcc -g -fPIC -I/usr/local/include -shared es_mx.c -o es_mx.so
	sqlite3 es_mx.db 'drop table test;' || echo 1
	sqlite3 -bail -cmd '.load ./es_mx.so' es_mx.db 'create table test(name collation es_mx); insert into test select name from src; select name,normalize(name) from test order by name collate es_mx asc; drop table test; '

clean :
	rm -fr uid.so uid.dll es_mx.so es_mx.dll

