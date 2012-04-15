#include<iostream>
#include<iomanip>
#include<fstream>
#include<string>
#include<stdlib.h>
#include<sstream>
#include<vector>
#include <stdio.h>

using namespace std;

vector<string> args;

#include "auxillaryFunctions.hpp"
#define GRAMMAR_FILE_NAME   (args[0].c_str())
#define EMPTY_ELEM_TABLE_INDICATOR "BLANK"
#define LEX_OUTPUT_FILE_NAME   (args[1].c_str())
#define END_OF_TOKENIZER_FILE   "EOF"


class LL1Table
{
public:
    string nonTerminal; //row
    string terminal; //column
    vector<string> productions; //elements
    //here we used vector as productions can be more than one incase of ambiguous grammars (NON LL1 Grammars)
};

class NonTerminalFirstFollowHolder
{
public:
    string nonTerminal;
    vector<string> first;
    vector<string> follow;
    vector<string> tempProductions;
    //The "tempNonTerminals" will be used to store intermediate nonterminals
    //before we further produce the terminals of those non terminals in a recursive fashion
    //and add them to the first and follow lists.
    vector<string> tempFollowNonTerminals;
    //Holds the list of non terminals whose FOLLOW terminals are a subset of the current nonTerminal's FOLLOW
    vector<string> productions;
    //productions store the list of all productions of the "nonTerminal"

    void print()
    {
        cout<<nonTerminal<<endl;
	cout<<"FIRST : ";
	for(int i = 0; i < first.size(); i++)
		cout<<first[i]<<" ";
	cout<<endl;
	cout<<"FOLLOW : ";
        for(int i = 0; i < follow.size(); i++)
		cout<<follow[i]<<" ";
	cout<<endl;
	return;
	cout<<nonTerminal<<"{ \n\t";
        printStringList(first);
        cout<<",\n\t";
        printStringList(follow);
        cout<<",\n\t";
        printStringList(tempProductions);
        cout<<",\n\t";
        printStringList(productions);
        cout<<"\n}";
    }
};

class ProductionFirst
{
public:
    string production;
    string nonTerminal;//The nonTerminal to which the production corresponds to
    string tempProduction;
    //tempProduction holds the temporary production for the calculation of first of the production
    vector<string> first;
};

class Tokenizer
{
public:
    ifstream fp;
    string token;
    int line_number; //Unused ?

    Tokenizer()
    {
        fp.open(LEX_OUTPUT_FILE_NAME);
        if(!fp.is_open())
        {
            cout<<"Unable to Open Lex Output File"<<endl;
            exit(0);
        }
    }

    string getToken()
    {
        if(fp>>token,fp.good())
        {
            return token;
        }
        else
        {//end of file is reached!!
            string temp;
            temp = END_OF_TOKENIZER_FILE;
            return temp;
        }
    }
};

class GrammarNonTerminals
{
public:
    vector<NonTerminalFirstFollowHolder> grammar;

    vector<LL1Table> table;
    vector<ProductionFirst> productionFirst;

    void populateProductionFirst()
    {//computes the FIRST list for each and every production and stores it in productionFirst list

bool DBG = false;

        for(int i=0;i<(int)grammar.size();i++)
        {
            for(int j=0;j<(int)grammar[i].productions.size();j++)
            {
                string production = grammar[i].productions[j];

                ProductionFirst new_production;
                new_production.production = production;
                new_production.nonTerminal = grammar[i].nonTerminal;

                if(DBG)
                {
                    cout<<"-- NONTERM:"<<new_production.nonTerminal<<endl;
                }

                if(isCapitalCharacter(production[0]))
                {
                    new_production.tempProduction = production;
                }
                else if(production[0]=='e')
                {//epsilon
                    if(!stringListHas(new_production.first,production))
                        new_production.first.push_back(production);

                }
                else
                {//other terminal
                    int pos = grammar[i].productions[j].find('.');
                    if(pos==-1)
                    {//the only terminal
                        if(!stringListHas(new_production.first,production))
                            new_production.first.push_back(production);
                    }
                    else
                    {
                        string first_terminal = production.substr(0,pos);
                        if(!stringListHas(new_production.first,first_terminal))
                            new_production.first.push_back(first_terminal);
                    }
                }

                productionFirst.push_back(new_production);

                if(DBG)
                {
                    cout<<"-- FIRST:";
                    printStringList(new_production.first);
                    cout<<"\n-- TEMPpRODUCTION:"<<new_production.tempProduction<<endl;
                }
            }
        }
        int number_of_unfinished_productions=0;//holds the number of unfinished productions
        for(int i=0;i<(int)productionFirst.size();i++)
        {
            number_of_unfinished_productions+=productionFirst[i].tempProduction.length();
        }

        if(DBG)
        {
           cout<<"\n----------\nPRODUCTION FIRST\n----------\n";
        }

        while(number_of_unfinished_productions!=0)
        {
            if(DBG)
            {
                cout<<"\n--------------\nRE ROUND\n---------------\n";
            }

            number_of_unfinished_productions = 0;//reinitializing it back to zero
            for(int i=0;i<(int)productionFirst.size();i++)
            {
                if((int)productionFirst[i].tempProduction.length()!=0)
                {
                    if(DBG)
                    {
                        cout<<"xxx YYY xxx"<<endl;
                        cout<<"NON: "<<productionFirst[i].nonTerminal<<endl;
                    }

                    string production = productionFirst[i].tempProduction;
                    //If the NonTerminal is already solved previously
                    if(DBG)
                    {
                        cout<<"T_PROD: "<<production<<endl;
                    }

                    vector<string> temp_first_list = getFirstList(getNthWord(production,1));
                    for(int k=0;k<(int)temp_first_list.size();k++)
                    {
                        if(DBG)
                        {
                            cout<<"LOOPIN..."<<endl;
                        }

                        if(temp_first_list[k].compare("e")==0)
                        {//if the terminal is 'epsilon'
                            if(hasNthWord(production,2))
                            {//there are multiple words and
                                string new_production = getAfterNthWord(production,1);
                                string new_first = getNthWord(new_production,1);
                                if(isCapitalCharacter(new_first[0]))
                                {//if the new production starts with a non-terminal
                                    number_of_unfinished_productions+=1;
                                    if(DBG)
                                    {
                                        cout<<"yyy XXX yyy"<<endl;
                                    }
                                    productionFirst[i].tempProduction = getAfterNthWord(production,1);
                                }
                                else
                                {//if the new production is a terminal
                                    if(!stringListHas(grammar[i].first,new_first))
                                        productionFirst[i].first.push_back(new_first);
                                }
                            }
                            else
                            {//epsilon is the only final word in the production so push it into the first list
                                if(!stringListHas(grammar[i].first,temp_first_list[k]))
                                    productionFirst[i].first.push_back(temp_first_list[k]);
                            }

                        }
                        else
                        {//for every other terminal
                            if(DBG)
                            {
                                cout<<"\tTerminal!!!"<<endl;
                            }
                            if(!stringListHas(productionFirst[i].first,temp_first_list[k]))
                            {
                                productionFirst[i].first.push_back(temp_first_list[k]);
                                if(DBG)
                                {
                                    cout<<"\tPushed!!"<<endl;
                                    cout<<"--FIRST: ";printStringList(productionFirst[i].first);cout<<endl;
                                }
                            }
                        }
                    }
                }
            }
        }

    }

    void populateProductions()
    {
        ifstream fp;
        fp.open(GRAMMAR_FILE_NAME);
        string line_buffer;
        // cout<<"Populating Productions..."<<endl;
        if(fp.is_open())
        {

            while(fp>>line_buffer,fp.good())
            {
                NonTerminalFirstFollowHolder nonTermHolder;
             //   cout<<"LINE: >>"<<line_buffer<<"<<"<<endl;
                int pos = line_buffer.find("->");
             //   cout<<"Position: "<<pos<<"\nNonTerminal(LHS): ";
                string temp_nonTerminal;
                temp_nonTerminal = line_buffer.substr(0,pos);
             //   cout<<temp_nonTerminal<<endl;
                nonTermHolder.nonTerminal = temp_nonTerminal;

                int tmpPos;
                tmpPos = pos = pos+2;//skipping the "->" string
             //   cout<<"value(tmpPos) = "<<tmpPos<<endl;
                pos = line_buffer.find('|',tmpPos);
                while(pos!=-1)
                {
             //       cout<<"value(pos) = "<<pos<<endl;
             //       cout<<"value(tmpPos) = "<<tmpPos<<endl;
                    string production;
                    production = line_buffer.substr(tmpPos,pos-tmpPos);
                    nonTermHolder.productions.push_back(production);
             //       cout<<"Production: "<<production<<endl;
                    tmpPos = pos = pos+1;
                    pos = line_buffer.find('|',tmpPos);
                }
                //last production
             //   cout<<"value(tmpPos) = "<<tmpPos<<endl;
                string production;
                production = line_buffer.substr(tmpPos);
                nonTermHolder.productions.push_back(production);
             //   cout<<"Production: "<<production<<endl;
                grammar.push_back(nonTermHolder);
            }
        }
        else
        {
            cout<<"Unable to open grammar file\n";
            perror("Unable to open grammar file!\n");
            exit(0);
        }
    }

    void print()
    {
        for(int i=0;i<(int)grammar.size();i++)
        {
            grammar[i].print();
            cout<<endl;
        }
    }

    vector<string> getFirstList(string nonTerminal)
    {
        for(int i=0;i<(int)grammar.size();i++)
        {
            if(grammar[i].nonTerminal.compare(nonTerminal)==0)
                return grammar[i].first;
        }
    }

    vector<string> getFollowList(string nonTerminal)
    {
        for(int i=0;i<(int)grammar.size();i++)
        {
            if(grammar[i].nonTerminal.compare(nonTerminal)==0)
                return grammar[i].follow;
        }
    }

    vector<string> getTempFollowNonTerminals(string nonTerminal)
    {
        for(int i=0;i<(int)grammar.size();i++)
        {
            if(grammar[i].nonTerminal.compare(nonTerminal)==0)
                return grammar[i].tempFollowNonTerminals;
        }
    }

    bool goesToEpsilon(string production)
    {//checks if a given production goes to epsilon by some first productions
        int number_of_words = numberWords(production);
        for(int i=1;i<=number_of_words;i++)
        {
            string word = getNthWord(production,i);
            if(!isCapitalCharacter(word[0]))
                return false;
            vector<string> firstListOfWord = getFirstList(word);
            if(!stringListHas(firstListOfWord,"e"))
            {
                return false;
            }
        }
        return true;
    }

    void populateFirst()
    {

        bool DBG = false;
        //set the above parameter as true for verbose processing

        for(int i=0;i<(int)grammar.size();i++)
        {

            if(DBG)
            {
                cout<<"Grmr Not Emty"<<endl;
            }

            for(int j=0;j<(int)grammar[i].productions.size();j++)
            {
                string production = grammar[i].productions[j];

                if(DBG)
                {
                    cout<<"Production: "<<production<<endl;
                }

                if(isCapitalCharacter(production[0]))
                {
                    grammar[i].tempProductions.push_back(production);
                }
                else if(production[0]=='e')
                {//epsilon
                    if(!stringListHas(grammar[i].first,production))
                        grammar[i].first.push_back(production);
                }
                else
                {//other terminal
                    int pos = grammar[i].productions[j].find('.');
                    if(pos==-1)
                    {//the only terminal
                        if(!stringListHas(grammar[i].first,production))
                            grammar[i].first.push_back(production);
                    }
                    else
                    {
                        string first_terminal = production.substr(0,pos);
                        if(!stringListHas(grammar[i].first,first_terminal))
                            grammar[i].first.push_back(first_terminal);
                    }
                }
            }
        }

        if(DBG)
        {
            cout<<"\n----------\nAfter First Round\n----------\n";
            print();
            cout<<"\n";
        }

        //end of initial round of "FIRST population"
        vector<string> list_of_finished_non_terminals;//holds the nonterminals whose "first" is known
        int number_of_unfinished_productions=0;//holds the number of unfinished productions
        for(int i=0;i<(int)grammar.size();i++)
        {
            if((int)grammar[i].tempProductions.size() == 0)
            {
                list_of_finished_non_terminals.push_back(grammar[i].nonTerminal);
            }
            else
            {
                number_of_unfinished_productions+=(int)grammar[i].tempProductions.size();
            }
        }
        while(number_of_unfinished_productions!=0)
        {
            int size_of_grammar = (int)grammar.size();
            number_of_unfinished_productions = 0;//reinitializing it back to zero
            for(int i=size_of_grammar-1;i>=0;i--)
            {
                if((int)grammar[i].tempProductions.size()!=0)
                {
                    vector<string>new_tempProductions;
                    for(int j=0;j<(int)grammar[i].tempProductions.size();j++)
                    {
                        string production = grammar[i].tempProductions[j];
                        if(stringListHas(list_of_finished_non_terminals,getNthWord(production,1)))
                        {//If the NonTerminal is already solved previously
                            vector<string> temp_first_list = getFirstList(getNthWord(production,1));
                            for(int k=0;k<(int)temp_first_list.size();k++)
                            {
                                if(temp_first_list[k].compare("e")==0)
                                {//if the terminal is 'epsilon'
                                    if(hasNthWord(production,2))
                                    {//there are multiple words and
                                        string new_production = getAfterNthWord(production,1);
                                        string new_first = getNthWord(new_production,1);
                                        if(isCapitalCharacter(new_first[0]))
                                        {//if the new production starts with a non-terminal
                                            number_of_unfinished_productions+=1;
                                            new_tempProductions.push_back(getAfterNthWord(production,1));
                                        }
                                        else
                                        {//if the new production is a terminal
                                            if(!stringListHas(grammar[i].first,new_first))
                                                grammar[i].first.push_back(new_first);
                                        }
                                    }
                                    else
                                    {//epsilon is the only final word in the production so push it into the first list
                                        if(!stringListHas(grammar[i].first,temp_first_list[k]))
                                            grammar[i].first.push_back(temp_first_list[k]);
                                    }

                                }
                                else
                                {//for every other terminal
                                    if(!stringListHas(grammar[i].first,temp_first_list[k]))
                                        grammar[i].first.push_back(temp_first_list[k]);
                                }
                            }
                        }
                        else
                        {//if NonTerminal is not solved, push the production into the new_tempProduction list
                         //and wait for solving it in next iteration of while loop after its respective NonTerminal is solved
                            number_of_unfinished_productions+=1;
                            new_tempProductions.push_back(production);
                        }
                    }
                    //Now, we replace the old temporary producions with the updated ones (which are partly solved)
                    grammar[i].tempProductions = new_tempProductions;
                    if((int)grammar[i].tempProductions.size()==0)
                    {//the current nonTerminal is fully solved so mark as solved in the list of solved nonTerminals
                        list_of_finished_non_terminals.push_back(grammar[i].nonTerminal);
                    }
                }
            }
            if(DBG)
            {
                cout<<"\n----------\nAfter Re-Round\n----------\n";
                print();
                cout<<"\n";
            }
        }

    }

    void populateFollow()
    {
        //the start nonTerminal must have $ in its FOLLOW
        {//braces are used here only to restrict the scope of "followElement" variable
            string followElement = "$";
            if(!stringListHas(grammar[0].follow,followElement))
                grammar[0].follow.push_back(followElement);
        }

        bool DBG = false;

        for(int i=0;i<(int)grammar.size();i++)
        {
            string nonTerminal = grammar[i].nonTerminal;
            for(int j=0;j<(int)grammar.size();j++)
            {
                for(int k=0;k<(int)grammar[j].productions.size();k++)
                {
                    int pos = productionHasWord(grammar[j].productions[k],nonTerminal);
                    if(pos != -1)
                    {//the nonTerminal is there in the production
                        //First check if the current nonTerminal is the last word in the production
                        if(!hasNthWord(grammar[j].productions[k],pos+1))
                        {//the nonTerminal is the last word in the production. So add the production's source nonTerminal into the
                         //current nonTerminal's tempFollowNonTerminal list.
                            string followElement;
                            followElement = "$"; //since its the last element in this production
                            if(!stringListHas(grammar[i].follow,followElement))
                                grammar[i].follow.push_back(followElement);

                            if(grammar[j].nonTerminal.compare(nonTerminal)!=0)
                            {//its not the same nonTerminal's production
                                //add the nonTermnial's follow list into the current nonTerminal's follow list
                                if(!stringListHas(grammar[i].tempFollowNonTerminals,grammar[j].nonTerminal))
                                    grammar[i].tempFollowNonTerminals.push_back(grammar[j].nonTerminal);
                            }
                        }
                        else
                        {//if the nonTerminal is not the last word in production
                            string nextWord = getNthWord(grammar[j].productions[k],pos+1);
                            if(isCapitalCharacter(nextWord[0]))
                            {//the nextWord is a nonTerminal
                                vector<string>firstListOfNextWord;
                                firstListOfNextWord = getFirstList(nextWord);
                                for(int l=0;l<(int)firstListOfNextWord.size();l++)
                                {
                                    if(firstListOfNextWord[l].compare("e")==0)
                                    {//if epsilon is in the FIRST list of the next nonTerminal
                                        if(!hasNthWord(grammar[j].productions[k],pos+2))
                                        {//if the epsilon going nonTerminal is the last word
                                            string followElement;
                                            followElement = "$"; //since the nonTerminal in question now becomes the last element in production
                                            if(!stringListHas(grammar[i].follow,followElement))
                                                grammar[i].follow.push_back(followElement);

                                            if(grammar[j].nonTerminal.compare(nonTerminal)!=0)
                                            {//its not the same nonTerminal's production
                                                //add the nonTerminal's follow List into the current nonTerminal's FOLLOW list
                                                if(!stringListHas(grammar[i].tempFollowNonTerminals,grammar[j].nonTerminal))
                                                    grammar[i].tempFollowNonTerminals.push_back(grammar[j].nonTerminal);
                                            }
                                        }
                                        else
                                        {//if the epsilon going nonTerminal is not the last word
                                           string new_production = getAfterNthWord(grammar[j].productions[k],pos+1);
                                           if(!stringListHas(grammar[i].tempProductions,new_production))
                                               grammar[i].tempProductions.push_back(new_production);

                                           //check if the new_production goes to epsilon and if yes, insert $ into follow
                                           if(goesToEpsilon(new_production))
                                           {
                                               string followElement;
                                               followElement = "$";
                                               if(!stringListHas(grammar[i].follow,followElement))
                                                   grammar[i].follow.push_back(followElement);

                                               if(grammar[j].nonTerminal.compare(nonTerminal)!=0)
                                               {//its not the same nonTerminal's production
                                                   //add the nonTerminal's follow List into the current nonTerminal's FOLLOW list
                                                   if(!stringListHas(grammar[i].tempFollowNonTerminals,grammar[j].nonTerminal))
                                                       grammar[i].tempFollowNonTerminals.push_back(grammar[j].nonTerminal);
                                               }
                                           }
                                        }
                                    }
                                    else
                                    {
                                        if(!stringListHas(grammar[i].follow,firstListOfNextWord[l]))
                                            grammar[i].follow.push_back(firstListOfNextWord[l]);
                                    }
                                }
                            }
                            else
                            {//the nextWord is a terminal and so directly add it into follow
                                if(!stringListHas(grammar[i].follow,nextWord))
                                    grammar[i].follow.push_back(nextWord);
                            }
                        }
                    }
                }
            }
        }

        if(DBG)
        {
            cout<<"\n----------\n[FOLLOW] After First Round\n----------\n";
            print();
        }

        //end of initial round of "FOLLOW population"
        int number_of_unfinished_productions=0;//holds the number of unfinished productions
        for(int i=0;i<(int)grammar.size();i++)
        {
            if((int)grammar[i].tempProductions.size() != 0)
            {
                number_of_unfinished_productions+=(int)grammar[i].tempProductions.size();
            }
        }
        while(number_of_unfinished_productions!=0)
        {
            number_of_unfinished_productions = 0;//reinitializing it back to zero
            for(int i=0;i<(int)grammar.size();i++)
            {
                vector<string> new_tempProductions;
                for(int j=0;j<(int)grammar[i].tempProductions.size();j++)
                {
                    string production = grammar[i].tempProductions[j];
                    string word = getNthWord(production,1);
                    if(!isCapitalCharacter(word[0]))
                    {//if the word is a terminal
                        if(!stringListHas(grammar[i].follow,word))
                            grammar[i].follow.push_back(word);
                    }
                    else
                    {//if the word is a non-terminal
                        vector<string> firstListOfWord;
                        firstListOfWord = getFirstList(word);
                        for(int k=0;k<(int)firstListOfWord.size();k++)
                        {
                            string firstTerminal = firstListOfWord[k];
                            if(firstTerminal.compare("e")==0)
                            {//if epsilon is in the FIRST list
                                if(hasNthWord(production,2))
                                {//if the production is having more than one word
                                    string new_production = getAfterNthWord(production,1);
                                    if(!stringListHas(new_tempProductions,new_production))
                                    {
                                        new_tempProductions.push_back(new_production);
                                        number_of_unfinished_productions+=1;
                                    }
                                }
                                else
                                {
                                    //DO NOTHING as $ is already inserted in the initial round itself
                                }
                            }
                            else
                            {
                                if(!stringListHas(grammar[i].follow,firstTerminal))
                                    grammar[i].follow.push_back(firstTerminal);
                            }
                        }
                    }
                }
                grammar[i].tempProductions = new_tempProductions;
            }
        }

        if(DBG)
        {
            cout<<"\n----------\n[FOLLOW] After Second Round\n----------\n";
            print();
        }

        //Now in the third round, we interpret each tempFollow lists
        vector<string> list_of_finished_non_terminals;//holds the nonterminals whose "follow" is known fully
        int number_of_unfinished_non_terminals = 0;
        for(int i=0;i<(int)grammar.size();i++)
        {
            number_of_unfinished_non_terminals+=(int)grammar[i].tempFollowNonTerminals.size();
            if(grammar[i].tempFollowNonTerminals.size()==0)
                list_of_finished_non_terminals.push_back(grammar[i].nonTerminal);
        }
        bool nonTerminalsFollowSaturated = false; //indicates if the
        int number_of_times_no_change_after_saturation = 0;
        while(number_of_unfinished_non_terminals!=0)
        {
            if(nonTerminalsFollowSaturated)
            {
                number_of_times_no_change_after_saturation+=1;
                if(number_of_times_no_change_after_saturation>4)
                 {//Saturation Detected DO SOMETHING

                     for(int i=0;i<(int)grammar.size();i++)
                     {
                         vector<string> new_tempFollowNonTerminals;
                         for(int j=0;j<(int)grammar[i].tempFollowNonTerminals.size();j++)
                         {
                             string followNonTerminal = grammar[i].tempFollowNonTerminals[j];
                             vector <string> FollowNonTerminalListOfFollowElement = getTempFollowNonTerminals(followNonTerminal);
                             if(stringListHas(FollowNonTerminalListOfFollowElement,grammar[i].nonTerminal))
                             {//{Follow of NonTerm} = {Follow of X} U {other stuff} && {Follow of X} = {Follow of X} U {other stuff}
                                 vector<string> temp1 = getTempFollowNonTerminals(followNonTerminal);
                                 for(int k=0;k<(int)temp1.size();k++)
                                 {
                                     if(temp1[k].compare(grammar[i].nonTerminal)!=0)
                                     //i.e., only if follow element is not the nonTerminal itself
                                         if(!stringListHas(new_tempFollowNonTerminals,temp1[k]))
                                             new_tempFollowNonTerminals.push_back(temp1[k]);
                                 }
                                 //after copying the nonTerminals yet to be resolved,
                                 //we now copy the terminals in follow of X that are resolved into the follow
                                 //of nonTerm
                                 temp1 = getFollowList(followNonTerminal);
                                 for(int k=0;k<(int)temp1.size();k++)
                                 {
                                     if(!stringListHas(grammar[i].follow,temp1[k]))
                                         grammar[i].follow.push_back(temp1[k]);
                                 }
                             }
                             else
                             {//Otherwise, do nothing. Here we attempt to remove any cycles only and so do nothing,
                                 //if there are no cycles
                                 new_tempFollowNonTerminals.push_back(followNonTerminal);
                             }
                         }
                         grammar[i].tempFollowNonTerminals = new_tempFollowNonTerminals;
                     }

                     number_of_times_no_change_after_saturation=0;
                 }
            }
            nonTerminalsFollowSaturated = true;
            number_of_unfinished_non_terminals = 0;
            for(int i=0;i<(int)grammar.size();i++)
            {
                vector<string> new_tempFollowNonTerminals;
                for(int j=0;j<(int)grammar[i].tempFollowNonTerminals.size();j++)
                {
                    if(stringListHas(list_of_finished_non_terminals,grammar[i].tempFollowNonTerminals[j]))
                    {//if the current nonTerminal's FOLLOW is fully known
                        vector<string> followList = getFollowList(grammar[i].tempFollowNonTerminals[j]);
                        for(int k=0;k<(int)followList.size();k++)
                        {
                            if(!stringListHas(grammar[i].follow,followList[k]))
                              {
                                  grammar[i].follow.push_back(followList[k]);
                                  nonTerminalsFollowSaturated = false;
                              }
                        }
                    }
                    else
                    {
                        if(!stringListHas(new_tempFollowNonTerminals,grammar[i].tempFollowNonTerminals[j]))
                        {
                            new_tempFollowNonTerminals.push_back(grammar[i].tempFollowNonTerminals[j]);
                            number_of_unfinished_non_terminals+=1;
                        }
                    }
                }
                grammar[i].tempFollowNonTerminals = new_tempFollowNonTerminals;
                if((int)grammar[i].tempFollowNonTerminals.size()==0)
                {
                    if(!stringListHas(list_of_finished_non_terminals,grammar[i].nonTerminal))
                     {
                        list_of_finished_non_terminals.push_back(grammar[i].nonTerminal);
                        nonTerminalsFollowSaturated = false;
                     }
                }
            }
        }
    }

    void _insertTable(string row_nonTerminal,string col_terminal,string elem_production)
    {

bool DBG = false;

        bool no_entry = true;
        for(int i=0;i<(int)table.size();i++)
        {
            if((table[i].nonTerminal.compare(row_nonTerminal)==0)&&(table[i].terminal.compare(col_terminal)==0))
            {
                no_entry = false;
                if(!stringListHas(table[i].productions,elem_production))
                {
                    table[i].productions.push_back(elem_production);
                    if(DBG)
                    {
                        cout<<"**** PUSHING ROW:"<<row_nonTerminal<<" COL:"<<col_terminal<<" ELEM:"<<elem_production<<endl;
                    }
                }
                else
                {
                    if(DBG)
                    {
                        cout<<"**** HAS ELEM:"<<elem_production<<" IN ";printStringList(table[i].productions);cout<<endl;
                    }
                }
            }
        }
        if(no_entry)
        {
            LL1Table new_element;
            new_element.nonTerminal = row_nonTerminal;
            new_element.terminal = col_terminal;
            new_element.productions.push_back(elem_production);

            table.push_back(new_element);
            if(DBG)
            {
                cout<<"**** PUSHING ROW:"<<row_nonTerminal<<" COL:"<<col_terminal<<" ELEM:"<<elem_production<<endl;
            }
        }
    }

    void generateTable()
    {
        populateProductionFirst();
        bool DBG = false;
        for(int i=0;i<(int)productionFirst.size();i++)
        {
            if(DBG)
            {
                cout<<"\tNON TERMINAL: "<<productionFirst[i].nonTerminal<<endl;
            }
            for(int j=0;j<(int)productionFirst[i].first.size();j++)
            {
                if(DBG)
                {
                    cout<<"\tX X X"<<endl;
                }
                if(productionFirst[i].first[j].compare("e")==0)
                {//epsilon case: USE Rule 2
                    vector<string> follow_list_of_nonTerminal = getFollowList(productionFirst[i].nonTerminal);
                    for(int k=0;k<(int)follow_list_of_nonTerminal.size();k++)
                    {
                        if(DBG)
                        {
                            cout<<"INSERTING ROW:"<<productionFirst[i].nonTerminal<<" COL:"<<follow_list_of_nonTerminal[k]<<" ELEM:"<<productionFirst[i].production<<endl;
                        }
                        _insertTable(productionFirst[i].nonTerminal,follow_list_of_nonTerminal[k],productionFirst[i].production);
                    }
                }
                else
                {
                    if(DBG)
                    {
                        cout<<"INSERTING ROW:"<<productionFirst[i].nonTerminal<<" COL:"<<productionFirst[i].first[j]<<" ELEM:"<<productionFirst[i].production<<endl;
                    }
                    _insertTable(productionFirst[i].nonTerminal,productionFirst[i].first[j],productionFirst[i].production);
                }
            }
        }
    }

    vector<string> _getElemTable(string row_nonTerminal,string col_terminal)
    {//returns list of productions at the corresponding row and column of the table
        for(int i=0;i<(int)table.size();i++)
        {
            if((table[i].nonTerminal.compare(row_nonTerminal)==0)&&(table[i].terminal.compare(col_terminal)==0))
                return table[i].productions;
        }

        vector<string> temp1;
        string temp2;
        temp2 = EMPTY_ELEM_TABLE_INDICATOR;
        temp1.push_back(temp2);
        return temp1;
    }

    bool printTable()
    {//prints and returns false if there is no ambuiguity

bool DBG = false;
        bool is_ambiguity = false;//is_ambiguity flag is set to true if ambuiguity is detected
        vector<string> row_nonTerminals;
        vector<string> col_terminals;
        for(int i=0;i<(int)table.size();i++)
        {
            if(!stringListHas(row_nonTerminals,table[i].nonTerminal))
                row_nonTerminals.push_back(table[i].nonTerminal);
            if(!stringListHas(col_terminals,table[i].terminal))
                col_terminals.push_back(table[i].terminal);
        }

        if(DBG)
        {
            cout<<"\n----------\nNON TERMINALS LIST\n-----------\n";
            printStringList(row_nonTerminals);
            cout<<"\n----------\nTERMINALS LIST\n-----------\n";
            printStringList(col_terminals);
            cout<<endl;
        }

        for(int i=0;i<(int)row_nonTerminals.size();i++)
        {
            cout<<"ROW: "<<row_nonTerminals[i]<<endl;
            for(int j=0;j<(int)col_terminals.size();j++)
            {

                vector<string> table_element = _getElemTable(row_nonTerminals[i],col_terminals[j]);

                if(table_element[0].compare(EMPTY_ELEM_TABLE_INDICATOR)!=0)
                {//if the production is not empty
                    cout<<"\t\tCOL: "<<col_terminals[j]<<endl;

                    for(int k=0;k<(int)table_element.size();k++)
                    {
                        cout<<"\t\t\t\t"<<row_nonTerminals[i]<<"->"<<table_element[k]<<endl;

                        if(k>0)
                            is_ambiguity = true;
                    }
                }
            }
        }

        return is_ambiguity;
    }

    void parser()
    {
        Tokenizer tokenizer;

        vector<string> input_buffer;

        while(true)
        {//populating input_buffer
            string token = tokenizer.getToken();
            if(token.compare(END_OF_TOKENIZER_FILE)==0)
            {//end of tokenizing
                token = "$";
                input_buffer.push_back(token);
                break;
            }
            input_buffer.push_back(token);
        }
        {//reversing the input_buffer
            vector<string> temp_input_buffer;
            int input_buffer_size = (int)input_buffer.size();
            for(int i=input_buffer_size-1;i>=0;i--)
            {
                temp_input_buffer.push_back(input_buffer[i]);
            }
            input_buffer = temp_input_buffer;
        }

        vector<string> computation_Stack;//the computation stack

        {//populate computation_Stack;
            //insert $
            string temp;
            temp = "$";
            computation_Stack.push_back(temp);
            //insert start state;
            temp = grammar[0].nonTerminal;
            computation_Stack.push_back(temp);
        }
        //starting the algorithm
        //REF: Book Pg.227 Compilers(Principles,Techniques & Tools) - 2nd Edn, 2007
        while(computation_Stack.back().compare("$")!=0)
        {
                    printStringList_spaceSeperated(computation_Stack,false,false);
                    printStringList_spaceSeperated(input_buffer,false,false);
                    cout<<"\t\t";

            if(computation_Stack.back().compare(input_buffer.back())==0)
            {
                input_buffer.pop_back();//advancing input
                computation_Stack.pop_back();//popping the stack
                cout<<"[MATCH]"<<endl;
            }
            else if(!isCapitalCharacter(computation_Stack.back()[0]))
            {//if its a terminal
                printBox("ERROR IN GIVEN CODE");exit(0);
            }
            else if(_getElemTable(computation_Stack.back(),input_buffer.back())[0].compare(EMPTY_ELEM_TABLE_INDICATOR)==0)
            {//if M[X,a] is blank entry
                printBox("ERROR IN GIVEN CODE");exit(0);
            }
            else
            {

                string production = _getElemTable(computation_Stack.back(),input_buffer.back())[0];
                cout<<computation_Stack.back()<<"->"<<production<<endl;
                computation_Stack.pop_back();
                int num_words_in_production = numberWords(production);
                for(int i=num_words_in_production;i>0;i--)
                {
                    string word = getNthWord(production,i);
                    if(word.compare("e")!=0)
                    {
                        computation_Stack.push_back(word);
                        //cout<<endl<<"PUSHED"<<i<<" "<<word<<endl;
                    }
                }
            }
        }
        printStringList_spaceSeperated(computation_Stack,false,false);
        printStringList_spaceSeperated(input_buffer,false,false);
        cout<<endl;
    }
};

