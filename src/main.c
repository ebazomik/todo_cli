#include "sqlite/sqlite3.h"
#include "stdio.h"
#include <string.h>

#define CLEAR_SCREEN printf("\033c");
#define CLEAR_INPUT while (getchar() != '\n')

int check_query_ready(sqlite3 *db, int res_from_prepare);
void check_query_done(int res_finalize, sqlite3 *db);
void show_all_todo(sqlite3 *db, char query[]);
void run_query_routine(sqlite3 *db, sqlite3_stmt* stmt, char query[], int id);

int main() {
  sqlite3 *db;
  char *err_msg = 0;
  char command[100];
  int err_check = 0;

  // DEFINE ALL QUERIES
  char create_todo_table[] = "CREATE TABLE IF NOT EXISTS todos( "
                                   "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                   "description TEXT,"
                                   "status TEXT);";
  char all_todo[] = "SELECT * FROM todos WHERE status != '❌';";
  char create_todo[] =
      "INSERT INTO todos (description, status) values (?, '⬛');";
  char edit_todo[] = "UPDATE todos SET description = ? WHERE id = ?;";
  char delete_todo[] = "UPDATE todos SET status = '❌' WHERE id = ?;";
  char complete_todo[] = "UPDATE todos SET status = '✅' WHERE id = ?;";
  char reset_todo[] = "UPDATE todos SET status = '⬛' WHERE id = ?;";

  int oc = sqlite3_open("todo.db", &db);

  if (oc != SQLITE_OK) {
    printf("Error on opening db %s \n", sqlite3_errmsg(db));
    return 1;
  }

  oc = sqlite3_exec(db, create_todo_table, 0, 0, &err_msg);

  if (oc != SQLITE_OK) {
    printf("Error on create TODO table");
    return 1;
  }

start_point:
  if (err_check)
    err_check = 0;
  CLEAR_SCREEN;

  printf("\n");

  printf(" Help -- [create] | [edit] | [delete] | [complete] | [reset] | "
         "[quit] \n\n");

  show_all_todo(db, all_todo);

  printf("\n\n");
  scanf("%s", command);

  if (!strcmp(command, "create")) {
    char todo[255];
    printf("Write your TODO: \n \n");
    // read up to newline
    scanf(" %[^\n]", todo);

    sqlite3_stmt *stmt;
    oc = sqlite3_prepare(db, create_todo, -1, &stmt, 0);

    err_check = check_query_ready(db, oc);
    if (err_check)
      goto start_point;

    sqlite3_bind_text(stmt, 1, todo, -1, 0);
    oc = sqlite3_step(stmt);

    check_query_done(oc, db);
    sqlite3_finalize(stmt);
    goto start_point;
  }

  if (!strcmp(command, "edit")) {
    int id;
    char description[255];
    printf("Enter the #ID of the todo you want to edit \n");
    printf("#");
    scanf("%i", &id);
    printf("Enter new description for todo nr %i: \n", id);
    // read up to newline
    scanf(" %[^\n]", description);

    sqlite3_stmt *stmt;
    oc = sqlite3_prepare(db, edit_todo, -1, &stmt, 0);

    err_check = check_query_ready(db, oc);
    if (err_check)
      goto start_point;

    sqlite3_bind_text(stmt, 1, description, -1, 0);
    sqlite3_bind_int(stmt, 1, id);

    oc = sqlite3_step(stmt);
    check_query_done(oc, db);
    sqlite3_finalize(stmt);
    goto start_point;
  }

  if (!strcmp(command, "delete")) {
    int id;
    printf("Enter the #ID of the todo you want to delete \n");
    printf("#");
    scanf("%i", &id);

    sqlite3_stmt *stmt;
    run_query_routine(db, stmt, delete_todo, id);
    goto start_point;
  }

  if (!strcmp(command, "complete")) {
    int id;
    printf("Enter the #ID of the todo you want to complete \n");
    printf("#");
    scanf("%i", &id);

    sqlite3_stmt *stmt;
    run_query_routine(db, stmt, complete_todo, id);
    goto start_point;
  }

  if (!strcmp(command, "reset")) {
    int id;
    printf("Enter the #ID of the todo you want to reset \n");
    printf("#");
    scanf("%i", &id);

    sqlite3_stmt *stmt;
    run_query_routine(db, stmt, reset_todo, id);
    goto start_point;
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
  goto start_point;

  return 0;
}

void show_all_todo(sqlite3 *db, char query[]) {
  int res = 0;
  int err_check = 0;
  sqlite3_stmt *stmt;
  res = sqlite3_prepare_v2(db, query, -1, &stmt, 0);
  printf(" TODOS 🔍 \n \n");
  while (sqlite3_step(stmt) == SQLITE_ROW) {
    printf(" | #%s | %s | %s \n", sqlite3_column_text(stmt, 0),
           sqlite3_column_text(stmt, 2), sqlite3_column_text(stmt, 1));
  }
  sqlite3_finalize(stmt);
}

int check_query_ready(sqlite3 *db, int res_from_prepare) {
  if (res_from_prepare != SQLITE_OK) {
    printf("Error on prepare query %s \n", sqlite3_errmsg(db));
    CLEAR_INPUT;
    getchar();
    return 1;
  }
  return 0;
}

void check_query_done(int res_finalize, sqlite3 *db) {
  if (res_finalize != SQLITE_DONE) {
    printf("Error during finalize query %s \n", sqlite3_errmsg(db));
    CLEAR_INPUT;
    getchar();
  }
}

void run_query_routine(sqlite3 *db, sqlite3_stmt* stmt, char query[], int id){
  int qp = sqlite3_prepare(db, query, -1, &stmt, 0);

    int err_check = check_query_ready(db, qp);
    if (err_check) 
      return;

    sqlite3_bind_int(stmt, 1, id);

    int stp = sqlite3_step(stmt);
    check_query_done(stp, db);
    sqlite3_finalize(stmt);
    return;
}
