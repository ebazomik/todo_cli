char create_todo_table[] = "CREATE TABLE todos (" 
	"id INTEGER PRIMARY KEY," 
	"description TEXT,"
	"status TEXT "
	"date DATE "
	"end_date DATE "  
");";

char check_table[] = "SELECT name FROM sqlite_master WHERE type='table' AND name='{todos}';";

int insert_todo(){
	return 0;
};
