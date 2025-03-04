
#include "stdio.h"
#include "sqlite3.h"
#include <stdlib.h>
#include "sqlqueries.c"



int check_sqlite_version(){
  int res = system("sqlite3 --version");
  if (res != 0) {
    printf(
        "TODO cli app use sqlite3, plese install sqlite3 and run TODO again");
    return 1;
  }
  return 0;
};

int open_db(sqlite3 *db){
  int rc = sqlite3_open("todo_db", &db);
  if(rc){
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    return 1;
  }
  return 0;
};

int check_table_presence(sqlite3 *db){
  int rc;
  char *zErrMsg = 0;
  rc = sqlite3_exec(db, check_table, NULL, NULL, &zErrMsg);
  if(rc != SQLITE_OK){
    printf("Table todo NOT exist %s \n", zErrMsg);
    sqlite3_free(zErrMsg);
    return 1;
  } else {
    return 0;
  }
};





int main() {
  sqlite3 *db;
  int todo_table;
  int fres;
  char *zErrMsg = 0;


  fres = check_sqlite_version();
  if(fres) return fres;

  fres = open_db(db);
  if(fres) return fres;

  todo_table = check_table_presence(db);

  if(todo_table){
    fres = sqlite3_exec(db, create_todo_table, NULL, 0, &zErrMsg);
    if(fres != SQLITE_OK){
      printf("Error on create todo table %s \n", zErrMsg);
      sqlite3_free(zErrMsg);
    }
  }
  
  sqlite3_close(db);
  return 0;
}
