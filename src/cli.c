#include <stdio.h>
#include "../include/cli.h"

void printWelcomePage()
{
    printf(ANSI_COLOR_CYAN);
	printf("###################################################### \n");
	printf("###################################################### \n");
	printf("##                                                  ## \n");
	printf("##              SimpleDB by jackiesogi              ## \n");
	printf("##    (Sorry, it's not listening on any port :D)    ## \n");
	printf("##                                                  ## \n");
	printf("###################################################### \n");
	printf("###################################################### \n");
	printf("Type \"HELP\" to see the usage...\n");
	printf(ANSI_COLOR_RESET);
}

void printNewLine()
{
    printf("127.0.0.1:6379 > ");
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

void printCommandNotFound(char* query_string)
{
    printf("[Error] Command \"%s\" not found.\n", query_string);
}