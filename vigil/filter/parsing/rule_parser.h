#ifndef RULE_PARSER_H
#define RULE_PARSER_H
#include "rule.h"
// find paths for rules in the config file
void rule_library_parser(const char *);
// find rules in the rule file
void rule_parser(const char *);
void deny_conf_parser(char *);
void host_mon_parser();
// void test_main();
void line_parser(const char *);
// static void get_protocol(const char *, struct rule * );
// static bool is_rule(const char *);
// static bool is_comment(const char * line);
// static void rstrip(char * );
// static void syntax_error(const char * line, int line_no);
// static void get_action(const char * , struct rule *);

void set_gam(const char *);

#endif /* RULE_PARSER_H */ 