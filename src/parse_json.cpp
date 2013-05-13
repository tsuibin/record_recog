#include "parse_speech.h"

#include <iostream>
#include <string>
#include <fstream>
#include <json/json.h>

using namespace std;

extern struct process_info cur_process;

int ParseJsonFromFile(const char *buf, char *exec_buf, 
				char *type_buf, const char *file)
{
	Json::Reader reader;
	Json::Value  index;
	std::string  str;
	int i;
	int flag = 0;
	int arraylen;
	char tmp[BUF_LEN];

	sys_says("ParseJsonFromFile : open %s\n", file);
	std::ifstream is;
	is.open(file, std::ios::binary);

	sys_says("ParseJsonFromFile : open successful\n");
	memset( exec_buf, 0, READ_LINE);
	memset( type_buf, 0, BUF_LEN);
	if ( reader.parse(is, index) ) {
		sys_says("ParseJsonFromFile : parse start...\n");
		arraylen = index["index"].size();
		std::cout<<"arraylen : "<<arraylen<<endl;

		for ( i = 0; i < arraylen; i++ ) {
			str = index["index"][i]["speech"].asString();
			memset(tmp, 0, BUF_LEN);
			strcpy(tmp, str.c_str());
			printf("tmp : %s\tlen : %lu\n", tmp, strlen(tmp));

			if ( memcmp(buf, tmp, strlen(buf)) == 0 ) {
				str = index["index"][i]["cmd"].asString();
				memset(tmp, 0, BUF_LEN);
				strcpy(tmp, str.c_str());
				memcpy(exec_buf, tmp, strlen(tmp));
				
				str = index["index"][i]["type"].asString();
				memset(tmp, 0, BUF_LEN);
				strcpy(tmp, str.c_str());
				memcpy(type_buf, tmp, strlen(tmp));
				
				flag = 1;
				sys_says("ParseJsonFromFile ret : %s\n", exec_buf);
				break;
			}
		}
	}
	is.close();
	
	if (!flag) {
		return 0;
	}

	return 1;
}

int ParseJsonIndex(const char *buf)
{
	Json::Reader reader;
	Json::Value  index;
	std::string  str;
	int i;
	int flag = 0;
	int arraylen;
	char tmp[BUF_LEN];

	printf("buf : %s\tlen : %lu\n", buf, strlen(buf));
	std::ifstream is;
	is.open(INDEX_JSON, std::ios::binary);

	memset( &cur_process, 0, sizeof(cur_process));
	if ( reader.parse(is, index) ) {
		arraylen = index["index"].size();
		std::cout<<"arraylen : "<<arraylen<<endl;

		for ( i = 0; i < arraylen; i++ ) {
			str = index["index"][i]["speech"].asString();
			//std::cout<<"speech : "<<str<<endl;
			
			memset(tmp, 0, BUF_LEN);
			strcpy(tmp, str.c_str());
			printf("tmp : %s\tlen : %lu\n", tmp, strlen(tmp));
			
			if ( memcmp(buf, tmp, strlen(buf)) == 0 ) {
				str = index["index"][i]["name"].asString();
				memset(tmp, 0, BUF_LEN);
				strcpy(tmp, str.c_str());
				memcpy(cur_process.name, tmp, strlen(tmp));
				
				str = index["index"][i]["title"].asString();
				memset(tmp, 0, BUF_LEN);
				strcpy(tmp, str.c_str());
				memcpy(cur_process.item, tmp, strlen(tmp));
				
				str = index["index"][i]["config"].asString();
				memset(tmp, 0, BUF_LEN);
				strcpy(tmp, str.c_str());
				memcpy(cur_process.config, tmp, strlen(tmp));
				
				flag = 1;
				break;
			}
		}
	}
	is.close();
	
	if (!flag) {
		return 0;
	}

	return 1;
}
