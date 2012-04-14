#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char** argv)
{
	ifstream in;
	string line;
	in.open(argv[1]);

	while ( in.good() )
	{
		getline(in, line);	
		std::cerr << "At line ::" << line << "\n";
	}

}
