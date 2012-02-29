/* This program declares the functions and files required for converting
 * regular expressiosn to fsm
 * Author : Rajat Khanduja
 *
 * 
 */

#ifndef REGEX_2_FSM_H_INCLUDED
#define REGEX_2_FSM_H_INCLUDED

#include <fsm.h>
#include <debug.h>

#define MAX_STATES 150


fsm_t *stack_fsm[MAX_STATES];

/* Function to create an NFA given a regular expression. The constructed
 * NFA is returned in 'fsm' pointer, which is passed as an argument.
 * Returns the size (number of states) of the NFA constructed. 
 */
unsigned int create_NFA (fsm_t *fsm, char *regular_expression);

#endif
