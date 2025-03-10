#include "sqlite/sqlite3.h"
#include "stdio.h"
#include <string.h>

#define CLEAR_SCREEN printf("\033c");

int main() {
  sqlite3 *db;
  char *err_msg = 0;
  char command[10];
  char tobe_completed[15] = "TO BE COMPLETED";
  char state_completed[9] = "COMPLETED";

  int oc = sqlite3_open("todo.db", &db);

  if (oc != SQLITE_OK) {
    printf("Error on opening db %s \n", sqlite3_errmsg(db));
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

  CLEAR_SCREEN;

  printf("What do you want to do? \n");
  printf("[show] - Show all TODOS (if exists) \n");
  printf("[create] - Create new TODO \n");
  printf("[edit] - Edit TODO (need id of TODO) \n");
  printf("[delete] - Delete TODO (need id of TODO) \n");
  printf("[complete] - Change on COMPLETE status of TODO (need id of TODO) \n");
  printf("[reset] - Change on AVAILABLE status of TODO (need id of TODO) \n");
  scanf("%s", command);

  int command_match;

  printf("command %s", command);

  // show
  if (!strcmp(command, "show")) {
    command_match = 1;
  }

  // create
  if (!strcmp(command, "create")) {
    command_match = 1;

    char todo[100];
    CLEAR_SCREEN;

    printf("Write your TODO: \n");
    // read up to newline
    scanf(" %[^\n]", todo);

    const char create_todo[] =
        "INSERT INTO todos (description, status) values (?, ?);";
    sqlite3_stmt *stmt;
    oc = sqlite3_prepare(db, create_todo, -1, &stmt, 0);

    if (oc != SQLITE_OK) {
      sqlite3_close(db);
      printf("Error on insert todo %s \n", sqlite3_errmsg(db));
      return 1;
    }

    sqlite3_bind_text(stmt, 1, todo, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, tobe_completed, -1, SQLITE_STATIC);

    oc = sqlite3_step(stmt);
    if (oc != SQLITE_DONE) {
      printf("Error during insertion %s \n", sqlite3_errmsg(db));
    } else {
      CLEAR_SCREEN;
      printf("TODO insert correctly (press enter to continue)");
      scanf("");
    }
  }

  if (!strcmp(command, "edit")) {
    CLEAR_SCREEN;
    command_match = 1;
    // edit
  }

  if (!strcmp(command, "delete")) {
    CLEAR_SCREEN;
    command_match = 1;
    // delete
  }

  if (!strcmp(command, "complete")) {
    CLEAR_SCREEN;
    command_match = 1;
    // complete
  }

  if (!strcmp(command, "reset")) {
    CLEAR_SCREEN;
    command_match = 1;
    // reset
  }

  if (command_match == 0) {
    CLEAR_SCREEN;
    // error on insert option
    printf("Comand not found");
  }

  sqlite3_close(db);
  return 0;
}
