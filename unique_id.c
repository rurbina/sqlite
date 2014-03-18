/* Add your header comment here */
#include <sqlite3ext.h> /* Do not use <sqlite3.h>! */
SQLITE_EXTENSION_INIT1

/* Insert your extension code here */
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
__declspec(dllexport)
#endif

void sqlite_ext_unique_id(sqlite3_context*,int,sqlite3_value**);
void sqlite_ext_unique_id_destroy(void*);

/* TODO: Change the entry point name so that "extension" is replaced by
** text derived from the shared library filename as follows:  Copy every
** ASCII alphabetic character from the filename after the last "/" through
** the next following ".", converting each character to lowercase, and
** discarding the first three characters if they are "lib".
*/
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

  sqlite3_create_function_v2(db, "unique_id", 1, SQLITE_UTF8, NULL, sqlite_ext_unique_id, NULL, NULL, NULL );
  sqlite3_create_function_v2(db, "unique_id", 0, SQLITE_UTF8, NULL, sqlite_ext_unique_id, NULL, NULL, NULL );
  return rc;
}

void sqlite_ext_unique_id(sqlite3_context *db, int row, sqlite3_value **value) {

  char *uid = malloc(256);
  char *chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
  int i = 0;
  int max = strlen(chars);
  int max_length = 8;
  long rndm;

  if ( uid == NULL ) {
    sqlite3_result_error_nomem(db);
    return;
  }

  if ( value != NULL && value[0] != NULL ) {
    int type = sqlite3_value_numeric_type(value[0]);
    if ( type == SQLITE_INTEGER ) {
      max_length = sqlite3_value_int(value[0]);
    }
  }

  for ( i=0; i < 254 && i < max_length; i++ ) {
    rndm = random();
    uid[i] = chars[rndm % max];
  }
  uid[i] = 0;

  //  strcpy(uid, sqlite3_value_text(value[0]));

  sqlite3_result_text(db, uid, strlen(uid), sqlite_ext_unique_id_destroy);

  return;

}

void sqlite_ext_unique_id_destroy(void *free_me) {

  free(free_me);

}
