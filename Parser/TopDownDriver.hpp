#ifndef TOP_DOWN_DRIVER_HPP
#define TOP_DOWN_DRIVER_HPP

#include <commons.hpp>
#include <fstream>
#include <ParserFunctions.hpp>
#include <Grammar.hpp>
#include <Terminal_NonTerminal.hpp>
#include <NonRecursivePredictiveParser.hpp>

class TopDownDriver
{
	private:
				
	public:
		TopDownDriver();
		void Drive(Grammar&, char*);


};

#endif
