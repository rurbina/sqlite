/* Add your header comment here */
#include <sqlite3ext.h> /* Do not use <sqlite3.h>! */
SQLITE_EXTENSION_INIT1

/* Insert your extension code here */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
__declspec(dllexport)
#endif

void sqlite_ext_strmatch(sqlite3_context*,int,sqlite3_value**);
void sqlite_ext_strmatch_any(sqlite3_context*,int,sqlite3_value**);
void sqlite_ext_strmatch_destroy(void*);
//void sqlite_ext_uid_randomizer(const char* chars, int length, char* buffer);

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

  sqlite3_create_function_v2(db, "strmatch", 2, SQLITE_UTF8 | SQLITE_DETERMINISTIC, NULL, sqlite_ext_strmatch, NULL, NULL, NULL );
  sqlite3_create_function_v2(db, "strmatchany", 2, SQLITE_UTF8 | SQLITE_DETERMINISTIC, NULL, sqlite_ext_strmatch_any, NULL, NULL, NULL );
  return rc;
}

int tokenize(char *str, char **tokens) {
  int count = 0;
  while ( count < 255  ) {
    tokens[count] = strtok( ( count == 0 ) ? str : NULL, ", ");
    if ( tokens[count] == NULL )
      break;
    count++;
  }

  return count;
}

void sqlite_ext_strmatch(sqlite3_context *db, int row, sqlite3_value **value) {

  char *need = (char *)sqlite3_value_text(value[0]);
  char *have = (char *)sqlite3_value_text(value[1]);

  char *need_tokens[256];
  char *have_tokens[256];
  int count = 0;
  int need_index;
  int have_index;
  int ok = 0;

  
  if ( tokenize(need, (char **)need_tokens) == 0 ) {
    sqlite3_result_int(db, 1);
    return;
  }
  if ( tokenize(have, (char **)have_tokens) == 0 ) {
    sqlite3_result_int(db, 0);
    return;
  }

  // check, one by one
  for ( need_index = 0; need_tokens[need_index] != NULL; need_index++ ) {
    ok = 0;
    for ( have_index = 0; have_tokens[have_index] != NULL; have_index++ ) {
      if ( strcmp(need_tokens[need_index], have_tokens[have_index]) == 0 ) {
	ok = 1;
	break;
      }
    }
    if ( ok == 0 ) {
      sqlite3_result_int(db, 0);
      return;
    }
  }

  sqlite3_result_int(db, 1);
  return;

}

void sqlite_ext_strmatch_any(sqlite3_context *db, int row, sqlite3_value **value) {

  char *need = (char *)sqlite3_value_text(value[0]);
  char *have = (char *)sqlite3_value_text(value[1]);

  char *need_tokens[256];
  char *have_tokens[256];
  int count = 0;
  int need_index;
  int have_index;
  int ok = 0;

  
  if ( tokenize(need, (char **)need_tokens) == 0 ) {
    sqlite3_result_int(db, 1);
    return;
  }
  if ( tokenize(have, (char **)have_tokens) == 0 ) {
    sqlite3_result_int(db, 0);
    return;
  }

  // check, one by one
  ok = 0;
  for ( need_index = 0; need_tokens[need_index] != NULL; need_index++ ) {
    for ( have_index = 0; have_tokens[have_index] != NULL; have_index++ ) {
      if ( strcmp(need_tokens[need_index], have_tokens[have_index]) == 0 ) {
	ok++;
	break;
      }
    }
  }

  if ( ok == 0 ) {
    sqlite3_result_int(db, 0);
    return;
  }

  sqlite3_result_int(db, 1);
  return;

}


void sqlite_ext_strmatch_destroy(void *free_me) {

  free(free_me);

}
