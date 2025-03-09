#include "sqlite/sqlite3.h"
#include "stdio.h"
#include <string.h>

int main() {
  sqlite3 *db;
  char *err_msg = 0;
  char command[8];
  int prg_exit = 0;

  int oc = sqlite3_open("todo.db", &db);

  if (oc != SQLITE_OK) {
    printf("Errore nell'apertura del db %s \n", sqlite3_errmsg(db));
    return 1;
  }

  const char create_todo_table[] = "CREATE TABLE IF NOT EXISTS todos( "
                                   "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                   "description TEXT,"
                                   "status TEXT,"
                                   "date DATE,"
                                   "end_date DATE);";

  oc = sqlite3_exec(db, create_todo_table, 0, 0, &err_msg);

  if (oc != SQLITE_OK) {
    printf("Error on create TODO table");
    return 1;
  }

  printf("\033c");

  printf("What do you want to do? \n");
  printf("[show] - Show all TODOS (if exists) \n");
  printf("[create] - Create new TODO \n");
  printf("[edit] - Edit TODO (need id of TODO) \n");
  printf("[delete] - Delete TODO (need id of TODO) \n");
  printf("[complete] - Change on COMPLETE status of TODO (need id of TODO) \n");
  printf("[reset] - Change on AVAILABLE status of TODO (need id of TODO) \n");
  scanf("%s", command);

  int command_match;

  if (strcmp(command, "show")) {
    command_match = 1;
    // show
  }

  if (strcmp(command, "create")) {
    command_match = 1;
    // create
  }

  if (strcmp(command, "edit")) {
    command_match = 1;
    // edit
  }

  if (strcmp(command, "delete")) {
    command_match = 1;
    // delete
  }

  if (strcmp(command, "complete")) {
    command_match = 1;
    // complete
  }

  if (strcmp(command, "reset")) {
    command_match = 1;
    // reset
  }

  if (command_match == 0) {
    // error on insert option
    printf("Comand not found");
    prg_exit = 1;
  }

  sqlite3_close(db);
  return 0;
}
