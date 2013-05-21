#include "my_common.h"
#include "parse_speech.h"
#include <sqlite3.h>

extern struct process_info cur_process;

static int search_zhcn_callback(void *data, int col_num, 
							char **col_value, char **col_name);
							
static int search_table_callback(void *data, int col_num, 
							char **col_value, char **col_name);

static int search_index_table_callback(void *data, int col_num, 
							char **col_value, char **col_name);

void get_pinyin(char *cmd_buf)
{
	char tmp[4];
	char cmd[READ_LINE];
	char buf[EXEC_BUF];
	sqlite3 *db = NULL;
	char *err_msg = NULL;
	char sql[READ_LINE];
	char *bakup = cmd_buf;

	/* 打开内存数据库 */
	if ( SQLITE_OK != sqlite3_open(DATABASE_PATH, &db) ) {
		sys_err("search_table sqlite3_open failed...\n");
	}
	
	memset(cmd, 0, READ_LINE);
	
	while ( *cmd_buf != '\0' ) {
		memset(tmp, 0, 4);
		memset(buf, 0, EXEC_BUF);
		memset(sql, 0, READ_LINE);
		
		memcpy(tmp, cmd_buf, 3);
		sys_says("tmp : %s\n", tmp);
		sprintf( sql, "select pinyin from zhIndex where zhcn=\'%s\'", tmp );
		/* 查询， 并调用回调函数 */
		if ( SQLITE_OK != sqlite3_exec( db, sql, search_zhcn_callback, 
								buf, &err_msg ) ) {
			sys_err("search_table sqlite3_exec failed...\n");			
		}
		sys_says("buf : %s\n", buf);
		if ( buf[0] != '\0' )
			strcat(cmd, buf);
		sys_says("cmd_buf : %s\n", cmd_buf);
		cmd_buf += 3;
	}
	
	if ( SQLITE_OK != sqlite3_close(db) ) {
		sys_err("search_table sqlite3_close failed...\n");	
	}
	
	sys_says("cmd : %s\n", cmd);
	if ( cmd[0] != '\0' ) {
		memset(bakup, 0, READ_LINE);
		memcpy(bakup, cmd, strlen(cmd));
	}
	
	return ;
}

static int search_zhcn_callback(void *data, int col_num, 
							char **col_value, char **col_name)
{
	int i;
	char *buf = (char*)data;

	for ( i = 0; i < col_num; i++ ) {
		printf( "%s = %s \n", col_name[i], col_value[i] ? col_value[i] : "NULL" );
		if ( strcmp("pinyin", col_name[i]) == 0 ) {
			//memset(buf, 0, EXEC_BUF);
			strcpy(buf, col_value[i]);
		}
	}
	sys_says("buf : %s\n", buf);

	return 0;
}

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
