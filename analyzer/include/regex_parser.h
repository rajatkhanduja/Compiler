/* This file contains the declarations for parsing regular expressions 
 * Author : Rajat Khanduja
 *
 * Currently supported expressions :-
 *      - Simple strings ( all characters except |, \, -, (,),[,]) are accepted as such
 *      
 */

#ifndef REGEX_PARSER_H_INCLUDED
#define REGEX_PARSER_H_INCLUDED

#define MAX_REGEX_LEN 100



void infix2postfix (char *original_regex, char *final_regex);	// Converts original_regex to postfix format.

#endif // End of file
