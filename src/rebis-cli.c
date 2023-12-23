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

// developing...

// const char* check_config_exist(char* config_path)
// {
// 	const char* checkResult;

// 	// generate a complete search path for storing the config file.
// 	strncat(config_path, "RebisDB/rebis.conf", 19);
	
// 	if( access(config_path, F_OK) == 0 )
// 	{
// 		checkResult = config_path;
// 	}
// 	else
// 	{
// 		checkResult = "NOT FOUND";
// 	}

// 	return checkResult;
// }

// // Todo : create_config_setting is used to create a new key value in config file
// void create_config_setting(char* config_path, const char* key, char* value)
// {
// 	struct Connection *cconnect = load_table_from_file(config_path, 25);
	
// 	if(cconnect->fd == -1)
// 	{
// 		log_message("[Error] Unable to create table_connected to config file");
// 		return;
// 	}

// 	char query_string[100];
// 	sprintf(query_string, "SET %s %s", key, value);

// 	struct QueryObject *qobj = type_command(query_string, cconnect);
	
// 	free(qobj);
// 	free(cconnect);
// }

// // Todo : create a config file by a givem config_path,
// //        and call initConfigTable().
// //        note that create_config is a type of "SET <key> <value>"
// int create_config_file(char* config_path)
// {
// 	strncat(config_path, "RebisDB/rebis.conf", 19);

// 	int fd = open(config_path, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	
// 	if(fd == -1)
// 	{
// 		perror("open");
// 		log_message("Cannot create new config file");
// 	}
// 	close(fd);

// 	create_config_setting(config_path, "dir", "~/data/kvstore.dat");

// 	return fd;
// }

// // Todo : add machanism of adding a new table_connected to configFile,
// // 		  and then retrieve the database file path by calling "(config) GET dir"
// void run_application(const char* configFile)
// {
// 	struct Connection *cconfig = load_table_from_file(configFile, 1);
// 	struct QueryObject *qconfig = type_command(strdup("GET dir"), cconfig);

// 	// Todo : 需要一些額外檢查 看databaseFile合不合法
// 	const char *databaseFile = qconfig->val;
// 	free(qconfig);
// 	free(cconfig);

// 	printWelcomePage();

// 	// Connection 結構包含了 檔案路徑 檔案標示符 和從檔案讀取進來的 KeyValue_Table
// 	struct Connection *table_connected = load_table_from_file(databaseFile, MAX_ENTRIES);

//     // query_string 是使用者將要在 cli 介面輸入的指令
// 	char query_string[QUERY_STRING_MAX];

//     while (1)
//     {	
// 		printNewLine();

//         fgets(query_string, sizeof(query_string), stdin);

//         // Remove newline character added by fgets
//         size_t len = strlen( query_string );

//         if (len > 0 && query_string[len - 1] == '\n')
//         {
//             query_string[len - 1] = '\0';
//         }

// 		// type_command() 接收 使用者的 query 並對檔案內容進行搜尋
// 		// QueryObject 結構包含了 type_command()回傳回來的 query 結果
// 		struct QueryObject* qobj = type_command(query_string, table_connected);

// 		log_message( qobj->message );
		
// 		if ( qobj->status_code == 0 )
// 		{
// 			break;		// status_code 等於0 代表使用者輸入 EXIT 指令
// 		}
// 		else if ( qobj->status_code == 6 )
// 		{
// 			exit(0);	// status_code 等於6 代表使用者輸入 FLUSHDB（清空檔案內容），需要重啟一個新的 session
// 		}

// 		free(qobj);		// 釋放 QueryObject 結構物件所佔用的資源
//     }

// 	// 將 Connection 結構內的 KeyValue_Table 寫回檔案
// 	save_table_to_file(table_connected);
	
// 	free_table_connected(table_connected); 

// }

// int main()
// {
// 	const char* config_path = check_config_exist( strdup("~/.config/") );

// 	if( strncmp(config_path, "NOT FOUND", 9) == 0 )
// 	{
// 		create_config_file( strdup("~/.config/") );
// 	}
// 	else
// 	{
// 		run_application(config_path);
// 	}

// 	return 0;
// }
