#include "my_common.h"
#include "parse_speech.h"
#include <sqlite3.h>

extern struct process_info cur_process;

static int search_table_callback(void *data, int col_num, 
							char **col_value, char **col_name);

static int search_index_table_callback(void *data, int col_num, 
							char **col_value, char **col_name);

void search_table(char *cmd_buf, const char *table_name)
{
	sqlite3 *db = NULL;
	char *err_msg = NULL;
	char sql[READ_LINE];

	/* 打开内存数据库 */
	if ( SQLITE_OK != sqlite3_open(DATABASE_PATH, &db) ) {
		sys_err("search_table sqlite3_open failed...\n");
	}
	
	memset(sql, 0, READ_LINE);
	sprintf( sql, "select cmd, type from %s where speech=\'%s\'", 
				table_name, cmd_buf );
	sys_says("\nsql : %s\n\n", sql);
	
	/* 查询， 并调用回调函数 */
	if ( SQLITE_OK != sqlite3_exec( db, sql, search_table_callback, 
								0, &err_msg ) ) {
		sys_err("search_table sqlite3_exec failed...\n");						
	}
	
	if ( SQLITE_OK != sqlite3_close(db) ) {
		sys_err("search_table sqlite3_close failed...\n");	
	}

	//printf("\ncmd : %s\n", cur_process.exec_buf);
	//printf("type : %s\n", cur_process.type);

	return ;
}

/* 回调函数 */
static int search_table_callback(void *data, int col_num, 
							char **col_value, char **col_name)
{
	int i;

	for ( i = 0; i < col_num; i++ ) {
		printf( "%s = %s \n", col_name[i], col_value[i] ? col_value[i] : "NULL" );
		if ( strcmp("cmd", col_name[i]) == 0 ) {
			memset(cur_process.exec_buf, 0, EXEC_BUF);
			strcpy(cur_process.exec_buf, col_value[i]);
		} else if ( strcmp("type", col_name[i]) == 0 ) {
			memset(cur_process.type, 0, BUF_LEN);
			strcpy(cur_process.type, col_value[i]);
		}
	}

	return 0;
}

void search_index_table()
{
	sqlite3 *db = NULL;
	char *err_msg = NULL;
	char sql[READ_LINE];

	/* 打开内存数据库 */
	if ( SQLITE_OK != sqlite3_open(DATABASE_PATH, &db) ) {
		sys_err("search_index_table sqlite3_open failed...\n");
	}
	
	memset(sql, 0, READ_LINE);
	sprintf( sql, "select ProcessName, TableName from %s where speech=\'%s\'", 
					INDEX_TABLE, cur_process.cmd_buf );
	sys_says("\nsql : %s\n\n", sql);
	
	/* 查询， 并调用回调函数 */
	if ( SQLITE_OK != sqlite3_exec( db, sql, search_index_table_callback, 
								0, &err_msg) ) {
		sys_err("search_table sqlite3_exec failed...\n");						
	}
	
	if ( SQLITE_OK != sqlite3_close(db) ) {
		sys_err("search_table sqlite3_close failed...\n");	
	}

	//printf("\nname : %s\n", cur_process.name);
	//printf("table : %s\n\n", cur_process.table);

	return ;
}

/* 回调函数 */
static int search_index_table_callback(void *data, int col_num, 
								char **col_value, char **col_name)
{
	int i;

	for ( i = 0; i < col_num; i++ ) {
		printf( "%s = %s \n", col_name[i], col_value[i] ? col_value[i] : "NULL" );
		if ( strcmp("ProcessName", col_name[i]) == 0 ) {
			memset(cur_process.name, 0, BUF_LEN);
			strcpy(cur_process.name, col_value[i]);
		} else if ( strcmp("TableName", col_name[i]) == 0 ) {
			memset(cur_process.table, 0, BUF_LEN);
			strcpy(cur_process.table, col_value[i]);
		}
	}

	return 0;
}
