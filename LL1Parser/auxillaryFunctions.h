
bool isCapitalCharacter(char c)
{//tells if given character is a capital letter

    int val = (int)c;
    if((val>=65)&&(val<=90))
        return true;

    return false;
}

string getNthWord(string production,int n=1)
{
    int startMarker=0;
    int number_of_words = 0;
    for(int i=0;i<(int)production.length();i++)
    {
       if(production[i]=='.')
       {
           number_of_words+=1;
           if(number_of_words == n)
              return production.substr(startMarker,i-startMarker);
           else
               startMarker = i+1;
       }
    }
    if(number_of_words+1 == n)
    {
        return production.substr(startMarker);
    }
    if(number_of_words==0)
    {
        string return_string;
        return_string="";
        return return_string;
    }
}

int numberWords(string production)
{
    int number_of_words = 1;
    for(int i=0;i<(int)production.length();i++)
    {
        if(production[i]=='.')
        {
            number_of_words+=1;
        }
    }
    return number_of_words;
}

int productionHasWord(string production,string s)
{//checks if the given production has the word and returns the position of the word
 //returns -1 if the word is not present in the production
    int number_of_words = numberWords(production);
    for(int i=1;i<=number_of_words;i++)
    {
        if(s.compare(getNthWord(production,i))==0)
            return i;

    }
    return -1;
}

bool stringListHas(vector<string>inputStringList,string s)
{//tells if the given string list has the given string
    for(int i=0;i<(int)inputStringList.size();i++)
    {
        if(s.compare(inputStringList[i])==0)
            return true;
    }
    return false;
}

string getAfterNthWord(string production,int n=1)
{//gets the remaining production after the Nth Word
    int number_of_words = 0;
    for(int i=0;i<(int)production.length();i++)
    {
       if(production[i]=='.')
       {
           number_of_words+=1;
           if(number_of_words == n)
              return production.substr(i+1);
       }
    }
}

bool hasNthWord(string production,int n)
{//returns true if the given production has nth NonTerminal/Terminal
    int number_of_words = 1; //number of successes
    for(int i=0;i<(int)production.length();i++)
    {
        if(production[i]=='.')
        {
            number_of_words+=1;
        }
    }
    if(n<=number_of_words)
        return true;

    return false;
}

void printStringList(vector<string> input_string_list)
{//python style of printing string list
    cout<<"{ ";
    for(int i=0;i<(int)input_string_list.size();i++)
    {
//        cout<<input_string_list[i];
        if(i+1!=input_string_list.size())
        {
//            cout<<" , ";
        }
    }
//    cout<<" }";
}

void printStringList_spaceSeperated(vector<string> input_string_list,bool forward_traverse = true,bool left_align = true)
{
    if(!forward_traverse)
    {
        vector<string>temp;
        int size_list = input_string_list.size();
        for(int i=size_list-1;i>=0;i--)
        {
            temp.push_back(input_string_list[i]);
        }
        input_string_list = temp;
    }

    stringstream op_stream;
    for(int i=0;i<(int)input_string_list.size();i++)
    {
        op_stream<<input_string_list[i];
        if(i+1!=input_string_list.size())
        {
            op_stream<<" ";
        }
    }
    if(left_align)
    {
        cout<<op_stream.str();
    }
    else
    {
        cout<<op_stream.str();
    }
}

void printBox(string s,int left_indent_space = 10)
{
    int len = (int)s.length();

    cout<<endl;
    //line 1
        //left space;
        for(int i=0;i<left_indent_space;i++)
        {
            cout<<" ";
        }
        cout<<"+-";
        for(int i=0;i<len;i++)
        {
            cout<<"-";
        }
        cout<<"-+\n";

    //line 2
        //left space;
        for(int i=0;i<left_indent_space;i++)
        {
            cout<<" ";
        }
        cout<<"| ";
        cout<<s;
        cout<<" |\n";

    //line 3
        //left space;
        for(int i=0;i<left_indent_space;i++)
        {
            cout<<" ";
        }
        cout<<"+-";
        for(int i=0;i<len;i++)
        {
            cout<<"-";
        }
        cout<<"-+\n";

}
