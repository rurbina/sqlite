/* Add your header comment here */
#include <sqlite3ext.h> /* Do not use <sqlite3.h>! */
SQLITE_EXTENSION_INIT1

/* Insert your extension code here */
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
__declspec(dllexport)
#endif

void sqlite_ext_normalize(sqlite3_context *db, int row, sqlite3_value **value) {


  return;

}


int sqlite3_extension_init(sqlite3 *db, char **pzErrMsg, const sqlite3_api_routines *pApi )
{
  int rc = SQLITE_OK;
  SQLITE_EXTENSION_INIT2(pApi);
  /* Insert here calls to
  **     sqlite3_create_function_v2(),
  **     sqlite3_create_collation_v2(),
  **     sqlite3_create_module_v2(), and/or
  **     sqlite3_vfs_register()
  ** to register the new features that your extension adds.
  */

  sqlite3_create_function_v2(db, "uid", 1, SQLITE_UTF8, NULL, sqlite_ext_uid, NULL, NULL, NULL );
  sqlite3_create_function_v2(db, "uid", 0, SQLITE_UTF8, NULL, sqlite_ext_uid, NULL, NULL, NULL );
  return rc;
}

void sqlite_ext_uid_randomizer(const char* chars, int length, char* buffer) {

  int i = 0;
  int max = strlen(chars);
  long rndm;
  time_t time_seed;
  int last = 0;

  time(&time_seed);
  srand(time_seed);

  for ( i=0; i < length; i++ ) {
    do {
      rndm = rand() % max;
    } while ( rndm == last );
    last = rndm;
    buffer[i] = chars[rndm % max];
  }
  buffer[i] = 0;

}

