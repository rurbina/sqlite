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

void sqlite_ext_seed(sqlite3_context*,int,sqlite3_value**);
void sqlite_ext_roll(sqlite3_context*,int,sqlite3_value**);
void sqlite_ext_dice_avg(sqlite3_context*,int,sqlite3_value**);
//void sqlite_ext_uid_randomizer(const char* chars, int length, char* buffer);

/* TODO: Change the entry point name so that "extension" is replaced by
** text derived from the shared library filename as follows:  Copy every
** ASCII alphabetic character from the filename after the last "/" through
** the next following ".", converting each character to lowercase, and
** discarding the first three characters if they are "lib".
*/
int sqlite3_dice_init(sqlite3 *db, char **pzErrMsg, const sqlite3_api_routines *pApi )
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

  srandom(time(NULL));

  sqlite3_create_function_v2(db, "roll", 1, SQLITE_UTF8, NULL, sqlite_ext_roll, NULL, NULL, NULL );
  sqlite3_create_function_v2(db, "roll", 2, SQLITE_UTF8, NULL, sqlite_ext_roll, NULL, NULL, NULL );
  sqlite3_create_function_v2(db, "roll", 3, SQLITE_UTF8, NULL, sqlite_ext_roll, NULL, NULL, NULL );
  sqlite3_create_function_v2(db, "dice_avg", 1, SQLITE_UTF8 | SQLITE_DETERMINISTIC, NULL, sqlite_ext_dice_avg, NULL, NULL, NULL );
  sqlite3_create_function_v2(db, "dice_avg", 2, SQLITE_UTF8 | SQLITE_DETERMINISTIC, NULL, sqlite_ext_dice_avg, NULL, NULL, NULL );
  sqlite3_create_function_v2(db, "dice_avg", 3, SQLITE_UTF8 | SQLITE_DETERMINISTIC, NULL, sqlite_ext_dice_avg, NULL, NULL, NULL );
  sqlite3_create_function_v2(db, "dice_seed", 1, SQLITE_UTF8, NULL, sqlite_ext_seed, NULL, NULL, NULL );
  return rc;
}

void sqlite_ext_seed(sqlite3_context *db, int row, sqlite3_value **value) {

  int  seed   = sqlite3_value_int(value[0]);
  srandom(seed);

  return;
}

int parse_dice(char *dice, int *qty, int *sides, int *plus) {

  int parsed = sscanf(dice, "%id%i%i", qty, sides, plus);

  if ( parsed < 2 ) return 0;

  if ( parsed == 2 ) {
    *plus = 0;
  }

  return ( *qty > 0 && sides > 0 );

}

int roll_dice(int qty, int sides, int plus) {

  int i, total = plus;

  for ( i = 0; i < qty; i++ ) {
    total += ( random() % sides ) + 1;
  }

  return total;

}

void sqlite_ext_roll(sqlite3_context *db, int row, sqlite3_value **value) {

  int qty, sides, plus, parse;

  if ( value[1] == NULL ) {
    parse = parse_dice((char *)sqlite3_value_text(value[0]), &qty, &sides, &plus);
  }
  else {
    qty   = sqlite3_value_int(value[0]);
    sides = sqlite3_value_int(value[1]);
    plus  = value[2] == NULL ? 0 : sqlite3_value_int(value[2]);
  }

  if ( qty < 1 || sides < 1 )
    sqlite3_result_int(db, 0);
  else
    sqlite3_result_int(db, roll_dice(qty, sides, plus) );

}

double dice_avg(int qty, int sides, int plus) {

  double total = plus + ( ( (sides / 2) + .5 ) * qty );

  return total;

}

void sqlite_ext_dice_avg(sqlite3_context *db, int row, sqlite3_value **value) {

  int qty, sides, plus, parse;

  if ( value[1] == NULL ) {
    parse = parse_dice((char *)sqlite3_value_text(value[0]), &qty, &sides, &plus);
  }
  else {
    qty   = sqlite3_value_int(value[0]);
    sides = sqlite3_value_int(value[1]);
    plus  = value[2] == NULL ? 0 : sqlite3_value_int(value[2]);
  }

  if ( qty < 1 || sides < 1 )
    sqlite3_result_int(db, 0);
  else
    sqlite3_result_double(db, dice_avg(qty, sides, plus) );

}
