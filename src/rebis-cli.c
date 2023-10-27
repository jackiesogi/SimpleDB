#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "../include/datastructure.h"
#include "../include/crud.h"
#include "../include/cli.h"
#include "../include/loadsave.h"
#include "../include/crud.h"
#include "../include/log.h"

#include "../include/hirebis.h"

// status code 之後擴充功能時應該用的到
enum query_string_status
{ 
	quit,		// 0
	nextline,	// 1
	set,		// 2
	get,		// 3
	del,		// 4
	help,		// 5
	flushdb,	// 6
	invalid,	// 7
	collision,	// 8
	set_ok,		// 9
	set_use,	// 10
	get_ok,		// 11
	get_use,	// 12
	del_ok,		// 13
	del_use	// 14
};

// 目前支援的指令：
// SET
// GET
// DEL
// FLUSHDB
// HELP
// 使用方式和開源專案 redis-cli 皆相同

/**********************************/
/** Entry point of the rebis-cli **/
/**********************************/
int main()
{
    printWelcomePage();

	// FILENAME 是預設的資料儲存檔案 定義在 datastructure.h
	const char* filename = FILENAME;

	// Connection 結構包含了 檔案路徑 檔案標示符 和從檔案讀取進來的 KeyValue_Table
	struct Connection *connection = load_data_from_file(filename);

    // query_string 是使用者將要在 cli 介面輸入的指令
	char query_string[QUERY_STRING_MAX];

    while (1)
    {	
		printNewLine();

        fgets(query_string, sizeof(query_string), stdin);

        // Remove newline character added by fgets
        size_t len = strlen( query_string );

        if (len > 0 && query_string[len - 1] == '\n')
        {
            query_string[len - 1] = '\0';
        }

		// type_command() 接收 使用者的 query 並對檔案內容進行搜尋
		// QueryObject 結構包含了 type_command()回傳回來的 query 結果
		struct QueryObject* qobj = type_command(query_string, connection);

		log_message( qobj->message );
		
		if ( qobj->status_code == 0 )
		{
			break;		// status_code 等於0 代表使用者輸入 EXIT 指令
		}
		else if ( qobj->status_code == 6 )
		{
			exit(0);	// status_code 等於6 代表使用者輸入 FLUSHDB（清空檔案內容），需要重啟一個新的 session
		}

		free(qobj);		// 釋放 QueryObject 結構物件所佔用的資源
    }

	// 將 Connection 結構內的 KeyValue_Table 寫回檔案
	save_data_to_file(connection);
	
	free_connection(connection); 
    
	return 0;
}


// switch ( qobj->status_code )
		// {
		// 	case quit:
		// 		loop_on = false;
		// 		break;
		// 	case nextline:
		// 		printf ( "%s", qobj->result );
		// 		break;
		// 	default:
		// 		printf ( "%s\n", qobj->result );
		// 		break;
		// }