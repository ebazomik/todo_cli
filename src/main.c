#include "sqlite/sqlite3.h"
#include "stdio.h"
#include <string.h>

#define CLEAR_SCREEN printf("\033c");
#define CLEAR_INPUT while (getchar() != '\n')

int main() {
  sqlite3 *db;
  char *err_msg = 0;
  char command[100];

  int oc = sqlite3_open("todo.db", &db);

  if (oc != SQLITE_OK) {
    printf("Error on opening db %s \n", sqlite3_errmsg(db));
    return 1;
  }

  const char create_todo_table[] = "CREATE TABLE IF NOT EXISTS todos( "
                                   "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                   "description TEXT,"
                                   "status TEXT);";

  oc = sqlite3_exec(db, create_todo_table, 0, 0, &err_msg);

  if (oc != SQLITE_OK) {
    printf("Error on create TODO table");
    return 1;
  }

select_options:
  CLEAR_SCREEN;

  printf("\n");

  printf("Help -- [create] | [edit] | [delete] | [complete] | [reset] | [quit] \n\n");

  const char all_todo[] = "SELECT * FROM todos WHERE status != '❌';";
  int res = 0;
  sqlite3_stmt *stmt;
  res = sqlite3_prepare_v2(db, all_todo, -1, &stmt, 0);
  printf("TODOS 🔍 \n \n");
  while (sqlite3_step(stmt) == SQLITE_ROW) {
    printf("| #%s | %s | %s \n", sqlite3_column_text(stmt, 0),
           sqlite3_column_text(stmt, 2), sqlite3_column_text(stmt, 1));
  }
  sqlite3_finalize(stmt);


  printf("\n\n");
  scanf("%s", command);

  if (!strcmp(command, "create")) {

    char todo[255];
    CLEAR_SCREEN;

    printf("Write your TODO: \n \n");
    // read up to newline
    scanf(" %[^\n]", todo);

    const char create_todo[] =
        "INSERT INTO todos (description, status) values (?, '⬛');";
    sqlite3_stmt *stmt;
    oc = sqlite3_prepare(db, create_todo, -1, &stmt, 0);

    if (oc != SQLITE_OK) {
      printf("Error on insert todo %s \n", sqlite3_errmsg(db));
      CLEAR_INPUT;
      getchar();
      goto select_options;
    }

    sqlite3_bind_text(stmt, 1, todo, -1, 0);

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
    int id;
    char description[255];
    CLEAR_SCREEN;
    printf("Enter the #ID of the todo you want to edit \n");
    printf("#");
    scanf("%i", &id);
    printf("Enter new description for todo nr %i: \n", id);
    // read up to newline
    scanf(" %[^\n]", description);

    const char delete_todo[] = "UPDATE todos SET description = ? WHERE id = ?;";
    sqlite3_stmt *stmt;
    oc = sqlite3_prepare(db, delete_todo, -1, &stmt, 0);

    if (oc != SQLITE_OK) {
      printf("Error on updating todo %s \n", sqlite3_errmsg(db));
      CLEAR_INPUT;
      getchar();
      goto select_options;
    }

    sqlite3_bind_text(stmt, 1, description, -1, 0);
    sqlite3_bind_int(stmt, 1, id);

    oc = sqlite3_step(stmt);
    if (oc != SQLITE_DONE) {
      printf("Error during updating %s \n", sqlite3_errmsg(db));
      CLEAR_INPUT;
      getchar();
      goto select_options;
    } else {
      sqlite3_finalize(stmt);
      CLEAR_SCREEN;
      printf("TODO updated correctly , press any key to continue");
      CLEAR_INPUT;
      getchar();
      goto select_options;
    }
  }

  if (!strcmp(command, "delete")) {
    int id;
    CLEAR_SCREEN;
    printf("Enter the #ID of the todo you want to delete \n");
    printf("#");
    scanf("%i", &id);

    const char delete_todo[] = "UPDATE todos SET status = '❌' WHERE id = ?;";
    sqlite3_stmt *stmt;
    oc = sqlite3_prepare(db, delete_todo, -1, &stmt, 0);

    if (oc != SQLITE_OK) {
      printf("Error on delete todo %s \n", sqlite3_errmsg(db));
      CLEAR_INPUT;
      getchar();
      goto select_options;
    }

    sqlite3_bind_int(stmt, 1, id);

    oc = sqlite3_step(stmt);
    if (oc != SQLITE_DONE) {
      printf("Error during deleting %s \n", sqlite3_errmsg(db));
      CLEAR_INPUT;
      getchar();
      goto select_options;
    } else {
      sqlite3_finalize(stmt);
      CLEAR_SCREEN;
      printf("TODO deleted correctly, press any key to continue");
      CLEAR_INPUT;
      getchar();
      goto select_options;
    }
  }

  if (!strcmp(command, "complete")) {
    int id;
    CLEAR_SCREEN;
    printf("Enter the #ID of the todo you want to complete \n");
    printf("#");
    scanf("%i", &id);

    const char delete_todo[] = "UPDATE todos SET status = '✅' WHERE id = ?;";
    sqlite3_stmt *stmt;
    oc = sqlite3_prepare(db, delete_todo, -1, &stmt, 0);

    if (oc != SQLITE_OK) {
      printf("Error on update todo %s \n", sqlite3_errmsg(db));
      CLEAR_INPUT;
      getchar();
      goto select_options;
    }

    sqlite3_bind_int(stmt, 1, id);

    oc = sqlite3_step(stmt);
    if (oc != SQLITE_DONE) {
      printf("Error during updating %s \n", sqlite3_errmsg(db));
      CLEAR_INPUT;
      getchar();
      goto select_options;
    } else {
      sqlite3_finalize(stmt);
      CLEAR_SCREEN;
      printf("CONGRATULATION this tood is done, press any key to continue");
      CLEAR_INPUT;
      getchar();
      goto select_options;
    }
  }

  if (!strcmp(command, "reset")) {
    int id;
    CLEAR_SCREEN;
    printf("Enter the #ID of the todo you want to reset \n");
    printf("#");
    scanf("%i", &id);

    const char delete_todo[] = "UPDATE todos SET status = '⬛' WHERE id = ?;";
    sqlite3_stmt *stmt;
    oc = sqlite3_prepare(db, delete_todo, -1, &stmt, 0);

    if (oc != SQLITE_OK) {
      printf("Error on delete todo %s \n", sqlite3_errmsg(db));
      CLEAR_INPUT;
      getchar();
      goto select_options;
    }

    sqlite3_bind_int(stmt, 1, id);

    oc = sqlite3_step(stmt);
    if (oc != SQLITE_DONE) {
      printf("Error during deleting %s \n", sqlite3_errmsg(db));
      CLEAR_INPUT;
      getchar();
      goto select_options;
    } else {
      sqlite3_finalize(stmt);
      CLEAR_SCREEN;
      printf("TODO deleted correctly, press any key to continue");
      CLEAR_INPUT;
      getchar();
      goto select_options;
    }
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
