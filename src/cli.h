#ifndef CLI_H

#define CLI_H
extern void print_welcome_page();
extern void print_new_line();
extern void print_goodbye();
extern void print_set_usage();
extern void print_get_usage();
extern void print_del_usage();
extern void print_help_page();
extern void print_command_not_found();


// Color Macros
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#endif