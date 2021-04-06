#ifndef __GRAMMAR_H
#define __GRAMMAR_H

/* structural characters */
#define BEGIN_ARRAY   '\x5B'  /* [ */
#define BEGIN_OBJECT  '\x7B'  /* { */
#define END_ARRAY     '\x5D'  /* ] */
#define END_OBJECT    '\x7D'  /* } */
#define NAME_SEP      '\x3A'  /* : */
#define VALUE_SEP     '\x2C'  /* , */
#define WHITESPACE    "\x20\x09\x0A\x0D"

/* literal names */
#define LITERAL_FALSE "false"
#define LITERAL_NULL  "null"
#define LITERAL_TRUE  "true"

/* string characters */
#define QUOTE_MARK    '\x22'  /* " */
#define ESCAPE        '\x5C'  /* \ */

#endif  /* __GRAMMAR_H */