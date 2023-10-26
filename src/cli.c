#include "../include/cli.h"
#include <stdio.h>

void printWelcomePage()
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

void printNewLine()
{
    printf("127.0.0.1:8888 > ");
}

void printGoodBye()
{
    puts("Bye!");
}

void printSetUsage()
{
    puts("[Usage] SET <key> <value>");
}

void printGetUsage()
{
    puts("[Usage] GET <key>");
}

void printDelUsage()
{
    puts("[Usage] DEL <key>");
}

void printHelpPage()
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

void printCommandNotFound(char* query_string)
{
    printf("[Error] Command \"%s\" not found.\n", query_string);
}