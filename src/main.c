#include "sqlite/sqlite3.h"
#include "stdio.h"
#include <string.h>

#define CLEAR_SCREEN printf("\033c");
#define CLEAR_INPUT while (getchar() != '\n')

int main() {
  sqlite3 *db;
  char *err_msg = 0;
  char command[100];
  char tobe_completed[15] = "TO BE COMPLETED";
  char completed[15] = "COMPLETED      ";
  char deleted[7] = "DELETED";

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

select_options:
  CLEAR_SCREEN;
  printf("What do you want to do? \n");
  printf("[show] - Show all TODOS (ready) \n");
  printf("[create] - Create new TODO (ready) \n");
  printf("[edit] - Edit TODO (not ready) \n");
  printf("[delete] - Delete TODO (wip) \n");
  printf("[complete] - Change on COMPLETE status of TODO (not ready) \n");
  printf("[reset] - Change on TO BE COMPLETED status of TODO (not ready) \n");
  printf("[quit] - Exit (ready) \n");
  printf("\n");
  scanf("%s", command);

  if (!strcmp(command, "show")) {
    CLEAR_SCREEN

    const char all_todo[] = "SELECT * FROM todos WHERE status != 'DELETED';";
    int res = 0;
    sqlite3_stmt *stmt;
    res = sqlite3_prepare_v2(db, all_todo, -1, &stmt, 0);
    if (res != SQLITE_OK) {
      printf(
          "Error on fetch TODOS or TODOS are empty, press any key to continue");
      CLEAR_INPUT;
      getchar();
      goto select_options;
    }

    printf("All TODOS \n \n");
    while (sqlite3_step(stmt) == SQLITE_ROW) {
      printf("| #%s | %s | %s \n", sqlite3_column_text(stmt, 0),
             sqlite3_column_text(stmt, 2), sqlite3_column_text(stmt, 1));
    }

    printf("\n");

    sqlite3_finalize(stmt);

    printf("TODO insert correctly, press any key to continue");
    CLEAR_INPUT;
    getchar();
    goto select_options;
  }

  if (!strcmp(command, "create")) {

    char todo[100];
    CLEAR_SCREEN;

    printf("Write your TODO: \n \n");
    // read up to newline
    scanf(" %[^\n]", todo);

    const char create_todo[] =
        "INSERT INTO todos (description, status) values (?, ?);";
    sqlite3_stmt *stmt;
    oc = sqlite3_prepare(db, create_todo, -1, &stmt, 0);

    if (oc != SQLITE_OK) {
      printf("Error on insert todo %s \n", sqlite3_errmsg(db));
      CLEAR_INPUT;
      getchar();
      goto select_options;
    }

    sqlite3_bind_text(stmt, 1, todo, -1, 0);
    sqlite3_bind_text(stmt, 2, tobe_completed, -1, 0);

    oc = sqlite3_step(stmt);
    if (oc != SQLITE_DONE) {
      printf("Error during insertion %s \n", sqlite3_errmsg(db));
      CLEAR_INPUT;
      getchar();
      goto select_options;
    } else {
      CLEAR_SCREEN;
      printf("TODO insert correctly, press any key to continue");
      CLEAR_INPUT;
      getchar();
      goto select_options;
    }
  }

  if (!strcmp(command, "edit")) {
    CLEAR_SCREEN;
  }

  if (!strcmp(command, "delete")) {
    char id[3];
    CLEAR_SCREEN;
    printf("Enter the #ID of the todo you want to delete \n");
    scanf("#%s", id);

    const char delete_todo[] = "UPDATE todos SET status = ? WHERE id = ?;";
    sqlite3_stmt *stmt;
    oc = sqlite3_prepare(db, delete_todo, -1, &stmt, 0);

    if (oc != SQLITE_OK) {
      printf("Error on delete todo %s \n", sqlite3_errmsg(db));
      CLEAR_INPUT;
      getchar();
      goto select_options;
    }

    sqlite3_bind_text(stmt, 1, deleted, -1, 0);
    sqlite3_bind_text(stmt, 1, id, -1, 0);

    oc = sqlite3_step(stmt);
    if (oc != SQLITE_DONE) {
      printf("Error during deleting %s \n", sqlite3_errmsg(db));
      CLEAR_INPUT;
      getchar();
      goto select_options;
    } else {
      CLEAR_SCREEN;
      printf("TODO deleted correctly, press any key to continue");
      CLEAR_INPUT;
      getchar();
      goto select_options;
    }
  }

  if (!strcmp(command, "complete")) {
    CLEAR_SCREEN;
  }

  if (!strcmp(command, "reset")) {
    CLEAR_SCREEN;
  }

  if (!strcmp(command, "quit")) {
    sqlite3_close(db);
    CLEAR_SCREEN;
    return 1;
  }

  CLEAR_SCREEN;
  printf("Comand not found, press any key to continue");
  CLEAR_INPUT;
  getchar();
  goto select_options;

  return 0;
}
