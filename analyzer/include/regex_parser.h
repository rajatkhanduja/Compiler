/* File declares structures and functions required to parse regular expressions */

#ifndef REGEX_PARSER_H_INCLUDED
#define REGEX_PARSER_H_INCLUDED

#define MAX_REGEX_LEN 100



void infix2postfix (char *original_regex, char *final_regex);	// Converts original_regex to postfix format.

#endif // End of file
