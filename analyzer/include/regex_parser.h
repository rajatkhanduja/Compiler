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

/* Supported REGEX operations with priority. Lower the 'value', higher the priority */
enum operation		
{
	STAR,
	CONCAT,
	OR,
	LB,
	RB
	// Keep LB and RB at the lowest priority
};

/* Define literals for the operators */
char operators[] = { '*', '@', '|', '(', ')' };


int is_operator (char c);	// Function returns -1 if it is not an operator, otherwise returns 'enum operation' value for the operator
void infix2postfix (char *original_regex, char *final_regex);	// Converts original_regex to postfix format.

#endif // End of file
