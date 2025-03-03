
#include "stdio.h"
#include "sqlite3.h"
#include <stdlib.h>

int main(int argc, char *argv[]) {

  int res = system("sqlite3 --version");

  if (res != 0) {
    printf(
        "TODO cli app use sqlite3, plese install sqlite3 and run TODO again");
    return (1);
  }

  sqlite3 *db;
  int rc;

  rc = sqlite3_open("TODO", &db);
  if (rc) {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    return (1);
  } else {
    printf("db opened");
    scanf("confirm");
    sqlite3_close(db);
  }
}
