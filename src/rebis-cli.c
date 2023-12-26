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
	del_use,	// 14
	lpush,		// 15
	lpop,		// 16
	rpush,		// 17
	rpop,		// 18
	llen,		// 19
	lrange		// 20
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
int main(int argc, char* argv[])
{

  // TABLE_FILE 是預設的資料儲存檔案 定義在 datastructure.h
  const char* tablefile = TABLE_FILE;
  const char* listfile = LIST_FILE;

  // Connection 結構包含了 檔案路徑 檔案標示符 和從檔案讀取進來的 KeyValue_Table
  struct Table_Connection *table_connected = load_table_from_file(tablefile, MAX_ENTRIES);
  struct List_Connection *list_connected  = load_list_from_file(listfile);
  struct Connection *connection = initConnection(table_connected, list_connected);

  if(argc == 1)
  {
    print_welcome_page();

      // query_string 是使用者將要在 cli 介面輸入的指令
    char query_string[QUERY_STRING_MAX];

      while (1)
      {	
        print_new_line();

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
  }
  else
  {
    char* clarg = (char*)malloc(1); // Allocate memory for an empty string
    clarg[0] = '\0'; // Ensure the string is properly terminated

    for(int i = 1; i < argc; i++) {
        if(strnlen(argv[i], 100) > 90) {
            fprintf(stderr, "The argument length exceeded\n");
        }
        size_t clarg_len = strlen(clarg);
        clarg = (char*)realloc(clarg, clarg_len + strlen(argv[i]) + 1); // Reallocate memory
        strncat(clarg, argv[i], 90); // Concatenate argument to clarg
        strncat(clarg, " ", 2);
    }
    
    size_t len = strlen(clarg);
    if (len > 0 && clarg[len - 1] == ' ') {
        clarg[len - 1] = '\0';
    }
    
    struct QueryObject* qobj = type_command(clarg, connection);

    log_message( qobj->message );

    free(qobj);
    free(clarg); // Free allocated memory when done
  }

	// 將 Connection 結構內的 KeyValue_Table 寫回檔案
	save_table_to_file(table_connected);
	save_list_to_file(list_connected);
	
	free_connection(connection); 
    
	return 0;
}
