#include <sqlite3ext.h>
SQLITE_EXTENSION_INIT1

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <wchar.h>
#include <locale.h>

#ifdef _WIN32
__declspec(dllexport)
#endif

// remove accents from src, store in dst
// because of how this works, dst will never be longer than src
char *strnormalize(char *dst, const char *src) {

  int i;
  int len = strlen(src);
  char *ptr = dst;

  for ( i = 0; i < len; i++, ptr++ ) {
    if ( i+1 < len ) {
      if ( !strncmp(&src[i],"Á",2) ) { ptr[0] = 'A'; i++; continue; }
      if ( !strncmp(&src[i],"É",2) ) { ptr[0] = 'E'; i++; continue; }
      if ( !strncmp(&src[i],"Í",2) ) { ptr[0] = 'I'; i++; continue; }
      if ( !strncmp(&src[i],"Ó",2) ) { ptr[0] = 'O'; i++; continue; }
      if ( !strncmp(&src[i],"Ú",2) ) { ptr[0] = 'U'; i++; continue; }
      if ( !strncmp(&src[i],"á",2) ) { ptr[0] = 'a'; i++; continue; }
      if ( !strncmp(&src[i],"é",2) ) { ptr[0] = 'e'; i++; continue; }
      if ( !strncmp(&src[i],"í",2) ) { ptr[0] = 'i'; i++; continue; }
      if ( !strncmp(&src[i],"ó",2) ) { ptr[0] = 'o'; i++; continue; }
      if ( !strncmp(&src[i],"ú",2) ) { ptr[0] = 'u'; i++; continue; }
      if ( !strncmp(&src[i],"Ñ",2) ) { ptr[0] = 'N'; i++; continue; }
      if ( !strncmp(&src[i],"ñ",2) ) { ptr[0] = 'n'; i++; continue; }
      if ( !strncmp(&src[i],"Ü",2) ) { ptr[0] = 'u'; i++; continue; }
      if ( !strncmp(&src[i],"ü",2) ) { ptr[0] = 'u'; i++; continue; }
    }
    ptr[0] = src[i];
  }
  ptr[0] = 0;

  return dst;

}

// normalize(text) returns a no-accents version of text
void normalize(sqlite3_context *dbx, int argc, sqlite3_value **argv) {

  char *dirty = (char*)sqlite3_value_text(argv[0]);
  char *clean = alloca(strlen(dirty)+1);

  if ( !clean ) {
    sqlite3_result_error_nomem(dbx);
  }
  else {
    strnormalize(clean,dirty);
    sqlite3_result_text(dbx, clean, -1, SQLITE_TRANSIENT);
  }

}

// perform a collation in the requested_collation locale
int sqlite3_es_mx_xcompare(void *requested_collation, int lenA, const void *ptrA, int lenB, const void *ptrB) {

  char *strA = alloca(lenA + 4);
  char *strB = alloca(lenB + 4);
  char *xA = alloca(lenA*4 + 1);
  char *xB = alloca(lenA*4 + 1);
  int diff = 0;
  char *old_locale, *saved_locale, *set_locale;

  if ( strA == NULL || strB == NULL ) {
    return 0;
  }

  stpncpy(strA,ptrA,lenA)[0] = 0;
  stpncpy(strB,ptrB,lenB)[0] = 0;

  old_locale = setlocale(LC_COLLATE, NULL);
  saved_locale = strdup(old_locale);
  set_locale = setlocale(LC_COLLATE, (char *)requested_collation);
  if ( !set_locale ) {
    printf("Locale setting failed\n");
    return 0;
  }
  else {
    printf("Locale is %s\n", set_locale);
  }

  // the resulting strings should compare nicely
  strxfrm(xA, strA, lenA*4);
  strxfrm(xB, strB, lenB*4);
  diff = strcmp(xA,xB);

  setlocale(LC_COLLATE, saved_locale);

  printf("CMP: %s %s %s\n", xA, ( (diff==0)?"==":(diff<0)?"<":">"  ), xB);

  return diff; //sqlite3_stricmp(strA,strB);

}

int sqlite3_extension_init(sqlite3 *db, char **pzErrMsg, const sqlite3_api_routines *pApi ) {

  int rc = SQLITE_OK;
  SQLITE_EXTENSION_INIT2(pApi);

  // this is nice to have
  sqlite3_create_function(db, "normalize", 1, SQLITE_UTF8 | 0x80, NULL, normalize, NULL, NULL);

  // one for traditional spanish, one for modern (Chávez before Cuarón)
  sqlite3_create_collation(db, "es_MX", SQLITE_UTF8, "es_ES.UTF-8", sqlite3_es_mx_xcompare);
  sqlite3_create_collation(db, "es_ES", SQLITE_UTF8, "es_ES", sqlite3_es_mx_xcompare);

  return rc;

}

