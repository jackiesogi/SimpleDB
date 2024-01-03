#include "cli.h"
#include <stdio.h>

void print_welcome_page()
{
    printf(ANSI_COLOR_CYAN);
	printf("###################################################### \n");
	printf("###################################################### \n");
	printf("##                                                  ## \n");
	printf("##           Rebis Database by jackiesogi           ## \n");
	printf("##    (Sorry, it's not listening on any port :D)    ## \n");
	printf("##                                                  ## \n");
	printf("###################################################### \n");
	printf("###################################################### \n");
	printf("Type \"HELP\" to see the usage...\n");
	printf(ANSI_COLOR_RESET);
}

void print_new_line()
{
    printf("127.0.0.1:8888 > ");
}

void print_goodbye()
{
    puts("Bye!");
}

void print_set_usage()
{
    puts("[Usage] SET <key> <value>");
}

void print_get_usage()
{
    puts("[Usage] GET <key>");
}

void print_del_usage()
{
    puts("[Usage] DEL <key>");
}

void print_help_page()
{
    puts("");
	puts("SET <key> <value>");
	puts("    example : SET name Caroline");
	puts("    output  : OK");
	puts("    example : SET height 168");
	puts("    output  : OK");
                puts("");
	puts("GET <key>");
	puts("    example : GET name");
	puts("    output  : \"Caroline\"");
	puts("    example : GET weight");
	puts("    output  : (nil)");
	puts("");
	puts("DEL <key>");
	puts("    example : DEL height");
	puts("    output  : OK");
	puts("    example : DEL helloworld");
	puts("    output  : (nil)");
	puts("");
	puts("EXIT");
	puts("    example : EXIT");
	puts("    output  : Bye!");
	puts("");
}

// printf("\nSET <key> <value>\n    example : SET name Caroline\n    output  : OK\n    example : SET height 168\n    output  : OK\n\nGET <key>\n    example : GET name\n    output  : \"Caroline\"\n    example : GET weight\n    output  : (nil)\n\nDEL <key>\n    example : DEL height\n    output  : OK\n    example : DEL helloworld\n    output  : (nil)\n\nEXIT\n    example : EXIT\n    output  : Bye!\n\n")

void print_command_not_found(char* query_string)
{
    printf("[Error] Command \"%s\" not found.\n", query_string);
}