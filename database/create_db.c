#include <stdio.h>
#include <string.h>
#include <sqlite3.h>

#define ZH_LEN 4
#define EN_LEN 7
#define BUF_LEN 100

#define sys_says(arg ...) { fprintf(stderr, arg); }

int main(int argc, char **argv)
{
	char zh[ZH_LEN];
	char en[EN_LEN];
	FILE *fp = NULL;
	sqlite3 *db = NULL;
	char *err_msg = NULL;
	char sql[BUF_LEN];
	
	if ( argc != 2 ) {
		sys_says("Usage : %s <file>\n", argv[0]);
		return -1;
	}

	if ( (fp = fopen(argv[1], "r")) == NULL ) {
		sys_says("open file failed...\n");
		return -1;
	}
	
	if ( SQLITE_OK != sqlite3_open("DeepinSpeech.db", &db) ) {
		sys_says("search_table sqlite3_open failed...\n");
	}
	
	while ( !feof(fp) ) {
		memset(zh, 0, ZH_LEN);
		memset(en, 0, EN_LEN);
		memset(sql, 0, BUF_LEN);
		
		fscanf(fp, "%s %s", zh, en);
		//sys_says("zh : %s\ten : %s\n", zh, en);
		
		if ( zh[0] == '\0' ) {
			continue;
		}
		sprintf(sql, "insert into zhIndex(zhcn, pinyin) \
				values(\'%s\', \'%s\')", zh, en);
		if ( SQLITE_OK != sqlite3_exec( db, sql, 0, 0, &err_msg ) ) {
			sys_says("zh : %s\ten : %s\n", zh, en);
			sys_says("sqlite3_exec failed...\n");
			break;				
		}
	}
	fclose(fp);
	if ( SQLITE_OK != sqlite3_close(db) ) {
		sys_says("sqlite3_close failed...\n");	
	}
	
	return 0;
}
