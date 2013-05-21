#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

#define BUF_LEN 80

int calltimes = 0;
char col_cmd[BUF_LEN];
char col_type[BUF_LEN];

static int callback( void *notused, int argc, char **argv, char **azColName );

int main( int argc, char *argv[] )
{
	sqlite3 *db = NULL;
	char *zErrMsg = NULL;
	//char *statement = NULL;
	int rc;
	//int i;

	/* 打开内存数据库 */
	sqlite3_open("DeepinSpeech.db", &db);
	/* 创建表 */
	/*rc = sqlite3_exec(db, "create table employee(id integer primary key, \
	   age integer);", NULL, 0, &zErrMsg );

	for ( i = 0; i < 10; i++ ) {
		// 插入数据
		statement = sqlite3_mprintf( "insert into employee values( %d, %d);", 
				rand()%65535, rand()%65535 );

		rc = sqlite3_exec( db, statement, NULL, 0, 0 );
		sqlite3_free( statement );
	}*/
	/* 查询， 并调用回调函数 */
	rc = sqlite3_exec( db, "select cmd, type from firefox where speech=\'新建标签\'", callback, 0, &zErrMsg );
	sqlite3_close(db);

	printf("\ncmd : %s\n", col_cmd);
	printf("type : %s\n", col_type);

	return 0;
}

/* 回调函数 */
static int callback( void *notused, int argc, char **argv, char **azColName )
{
	int i;

	calltimes++;
	for ( i = 0; i < argc; i++ ) {
		printf( "%s = %s \n", azColName[i], argv[i] ? argv[i] : "NULL" );
		if ( strcmp("cmd", azColName[i]) == 0 ) {
			strcpy(col_cmd, argv[i]);
		} else if ( strcmp("type", azColName[i]) == 0 ) {
			strcpy(col_type, argv[i]);
		}
	}

	printf("number %d callback\n", calltimes);

	return 0;
}
