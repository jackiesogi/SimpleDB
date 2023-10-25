#ifndef CLI_H

#define CLI_H
extern void printWelcomePage();
extern void printNewLine();
extern void printGoodBye();
extern void printSetUsage();
extern void printGetUsage();
extern void printDelUsage();
extern void printHelpPage();
extern void printCommandNotFound();


// Color Macros
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#endif