/*
    Enzo Arata
    Zachary Mcilwain
    CS 457
    Project 2
    October 21st
*/
#include <string>
//#include <stdlib.h>
#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <stdio.h>
#include <unistd.h>
#include <functional>
#include <cctype>
#include <algorithm> 

using namespace std;



int useIndex = -1;


vector<string> commandParse(string commandLine);
bool createDatabase(string name);
bool createTable(vector <string> tokens);
bool argumentParse(string arguments);
//bool selectFrom(string name);
bool dropDatabase(string name);
bool dropTable(string name);
bool use(string name);
bool clearData();
bool add(vector <string> tokens);
bool insert(vector <string> tokens);
bool selectFrom(vector <string> tokens);
bool deleteFrom(string tableName, vector <string> tokens );
bool deleteQuery(int tableSelected, vector <string> tokens);
bool update(string tableName, vector <string> tokens, vector <string> queries );
bool updateQuery(int tableSelected, vector <string> tokens, vector <string> queries);
bool changeData(int tableSelected, int tableEntry, int dataEntry, vector <string> queries);




string removeFrontSpaces(string cleanString);
vector <string> splitString(string splitString);
string removeEnd(string cleanString);

string setUppercase(std::string token);
bool selectFromQuery(vector <string> Atributes, string Name, vector <string> tokens);




//Data type for values inserted into tables
struct values{
	vector <string> dataMembers;
	
};
//Data type for our tables
struct table{
    string name;
    //int args=0;
    vector <string> arguments;
    vector <values> tableValues;
    int valuesInserted = 0;

};
//Data tyoe for our databases
struct dataBase{
    string name;
    vector <table> tables;
    int tableIndex = -1;
};

void printTable(table currentTable);
void printCutTable(table currentTable, vector <string> Atributes);
void printQueryTable(values currentEntry, std::vector <string> Attributes, int tableSelected);

vector <dataBase> databaseList;




int main( int argc, char *argv[] ){


	vector <string> commands;
	vector <string> commandQuery;
	vector <string> nameSelection;
	vector <string> input;
	string::size_type sz; 
    ifstream inFile;
	//inFile = fopen(argv[2],"r");
    inFile.open(argv[1]);

   // File input into a vector of strings, igoring lines with "--"
	string currentLine;
	while(getline(inFile, currentLine)){
       // cout << currentLine << endl;
        if (currentLine.substr(0,2)=="--")
            continue;
        input.push_back(setUppercase(currentLine));
	}



    //loop that runs on number of strings collected from file
	for(int inputLine = 0; inputLine < input.size(); inputLine++)
	{

        //Calls on parsing function for current input line
		commands = commandParse(input[inputLine]);
		nameSelection.clear();


        //Checks tokens of string to decipher command
		if (commands[0] == "CREATE")
		{
			if (commands[1] == "DATABASE")
				createDatabase(commands[2]);

			if (commands[1] == "TABLE")
                createTable(commands);
		} 


		else if (commands[0] == "DROP")
		{
			if (commands[1] == "DATABASE")
                dropDatabase(commands[2]);

			if (commands[1] == "TABLE")
                dropTable(commands[2]);
		} 


		else if (commands[0] == "USE") 
		{
			use(commands[1]);
		} 


		else if (commands[0] == "ALTER")
		{
            add(commands);
		}


		else if (commands[0] == "INSERT")
		{
			 if (commands[1] == "INTO"){
			 	insert(commands);
			 }	
			 	 
		}
		

		else if (commands[0] == "SELECT")
		{
			if (commands[1] == "*"){
            	if (commands[2] == "FROM"){
            		selectFrom(commands);
            		continue;
            	}

           		
            }
            for (int names = 1; names<commands.size();names++){
            	nameSelection.push_back(commands[names]);
            }
            nameSelection.push_back(commands[2]);
            inputLine++;
            commands = commandParse(input[inputLine]);
            if (commands[0] == "FROM"){
            	inputLine++;
            	selectFromQuery(nameSelection,commands[1],commandParse(input[inputLine]));
            }
		}


		else if (commands[0] == "DELETE")
		{
			if (commands[1] == "FROM")
            {
            	inputLine++;
            	nameSelection.push_back(commands[2]);
            	commands = commandParse(input[inputLine]);
            	deleteFrom(nameSelection[0], commands);
            }
		}


		else if (commands[0] == "UPDATE")
		{
            	inputLine++;
            	nameSelection.push_back(commands[1]);
            	commands = commandParse(input[inputLine]);
            	inputLine++;
            	commandQuery = commandParse(input[inputLine]);
            	update(nameSelection[0], commands, commandQuery);
  
		}


		else if (commands[0] == "EXIT")
		{
            clearData();
            cout << "-- Exiting program" << endl;
			return 0;
		}


		else if (commands[0] == "" |commands[0] == " " | commands[0] == "/n"){
			continue;
		}

		else {
			cout << "-- Command not recognized" << endl;
		}
	}
	/*string s = "149.99";
	float tempFloat;
	//setUppercase()
	//transform(s.begin(), s.end(), s.begin(), std::ptr_fun<int, int>(std::toupper));
	tempFloat = stof (s ,&sz);
	cout << s << endl;
	cout << tempFloat << endl;*/
	

}
//changes the command line string into a vector of the separated tokens
vector<string> commandParse(string commandLine){
	vector <string> commands;
	string token = "";
	int i;
	int cursor = 0;
	
 	

	for(i=0; i<commandLine.size(); i++){
		if (commandLine.at(i)==' '){
			token = commandLine.substr(cursor,i-cursor);
			//token = setUppercase(token);
			commands.push_back(token);
			cursor=i+1;

		}
		if (commandLine.at(i)==';')
		{
            break;
		}
		if (commandLine.at(i)=='(')
		{
            cursor--;
            break;
		}

	}
	token = commandLine.substr(cursor);
	token = token.substr(0, token.size()-1);
	commands.push_back(token);


	return commands;
}
//Makes a new database if one with same name doesnt exist
bool createDatabase(string name){
    vector <table> emptyVec;

	for (int i=0; i<databaseList.size(); i++){
		if (name == databaseList[i].name){
			cout << "-- !Failed to create datatabase " << name << " because it already exists" << endl;
			return 0;
		}
	}
	cout << "-- Database " << name << " created." << endl;
    dataBase tempData = {name, emptyVec, -1};
	databaseList.push_back(tempData);

    std::string directoryName = "DataBases/";
	directoryName.append(name);

	mkdir(directoryName.c_str(), S_IRUSR | S_IWUSR | S_IXUSR);
	return 1;
}
//Sets database for creating and deleting and altering tables
bool use(string name){
	for (int i =0; i<databaseList.size(); i++){
		if (name == databaseList[i].name){
			useIndex = i;
			databaseList[useIndex].tableIndex = -1;
			cout << "-- Using Database " << name << "." << endl;
			return 1;
		}
	}
	cout << "-- !Failed to use datatabase " << name << " because it dosent exist" << endl;
	return 0;
}
//Deletes data base if it exists
bool dropDatabase(string name)
{
    std::string directoryName = "DataBases/";

    for (int i=0; i<databaseList.size(); i++){
		if (name == databaseList[i].name){
            directoryName.append(name);
            databaseList.erase(databaseList.begin()+i);
            rmdir(directoryName.c_str());
			cout << "-- Database " << name << " deleted." << endl;
			useIndex = -1;
			return 0;
		}
	}
	cout << "-- !Failed to drop datatabase " << name << " because it does not exist" << endl;
    return 0;
}

//Clears directories
bool clearData()
{
    cout << "-- Deleting Databases....." << endl;
    std::string directoryName = "DataBases/";
    for (int i=0; i<databaseList.size(); i++){
        directoryName.append(databaseList[i].name);
        rmdir(directoryName.c_str());
        directoryName = "DataBases/";
    }
    return 1;
}
//Makes a new table if one with same name doesnt exist
bool createTable(vector <string> tokens)
{
    vector <string> t_string;
    if(useIndex == -1 )
    {
        cout << "-- !Failed to create table because no database has been selected" << endl;
        return 0 ;
    }
    for (int i=0; i<databaseList[useIndex].tables.size(); i++){
        if(tokens[2] == databaseList[useIndex].tables[i].name)
        {
            cout << "-- !Failed to create table " << tokens[2] << " because it already exists" << endl;
			return 0;
        }
    }
    table tempTable = {tokens[2], t_string};
	databaseList[useIndex].tables.push_back(tempTable);
    databaseList[useIndex].tableIndex = databaseList[useIndex].tables.size()-1;
    argumentParse(tokens[3]);
    return 1;
}
//Parses string into arguments for tables
bool argumentParse(string t_arg)
{
   // cout << "In Argument Parser" << endl;
    int i=1;
    string completedArg;
    string chopped;
	int cursor = 1;

    for(i; i < t_arg.size();i++)
    {

        if(t_arg.at(i) == ',' )
        {
            //cout << "assigning arugment " << endl;
            completedArg = t_arg.substr(cursor+1, i-cursor-1);
            //cout << useIndex << " " << tableIndex << endl;
            completedArg = removeFrontSpaces(completedArg);
			//databaseList[useIndex].tables[databaseList[useIndex].tableIndex].args++;
            databaseList[useIndex].tables[databaseList[useIndex].tableIndex].arguments.push_back(completedArg);
            completedArg = "";
            cursor=i++;

        }

	}

	//cout << "assinging last argument" << endl;
	if(cursor == 1)
	{
        chopped = t_arg.substr(cursor+1);
        chopped = chopped.substr(0, chopped.size()-1);
	}
	else{
        chopped = t_arg.substr(cursor+2);
        chopped = chopped.substr(0, chopped.size()-2);
	}
    databaseList[useIndex].tables[databaseList[useIndex].tableIndex].arguments.push_back(chopped);

    cout << "-- Table " << databaseList[useIndex].tables[databaseList[useIndex].tableIndex].name << " created." << endl;

    return 1;

}
//deletes table if it exists
bool dropTable(string name)
{
    for (int i=0; i<databaseList[useIndex].tables.size(); i++){
		if (name == databaseList[useIndex].tables[i].name){
            databaseList[useIndex].tables.erase(databaseList[useIndex].tables.begin()+i);

			cout << "-- Table " << name << " deleted." << endl;
			return 0;
		}
	}
	databaseList[useIndex].tableIndex = -1;
	cout << "-- !Failed to drop table " << name << " because it does not exist" << endl;
    return 0;
}
//Prints out data that exists in given table
/*bool selectFrom(string name)
{
    for(int j =0;j< databaseList[useIndex].tables.size();j++)
    {
    	name = removeEnd(name);

    	cout << name << " != " << databaseList[useIndex].tables[j].name << endl;

        if(name == databaseList[useIndex].tables[j].name)
        {
            cout << "-- " ;
            for(int k =0;k<databaseList[useIndex].tables[j].arguments.size(); k++ )
            {
                if(k>0)
                    cout << " | ";

                cout << databaseList[useIndex].tables[j].arguments[k];
            }
            cout << endl;
            return 1;
        }
    }
    cout << "-- !Failed to query table " << name << " because it does not exist" << endl;
    return 0;
}*/

//adds a new data type to given table
bool add(vector <string> tokens)
{
     string joinedString;
     for (int i=0; i<databaseList[useIndex].tables.size(); i++){
        if(tokens[2] == databaseList[useIndex].tables[i].name)
        {
            joinedString = tokens[4] + tokens[5];
            //databaseList[useIndex].tables[i].args++;
            databaseList[useIndex].tables[i].arguments.push_back(joinedString);
             cout << "-- Table " << tokens[2] << " modified." << endl;
            return 1;

        }


    }
    cout << "-- !Failed to alter table " << tokens[2] << " because it does not exist" << endl;
    return 0;


}
//Inserts data segment into table
bool insert(vector <string> tokens)
{
	int k =0;
	int spaceChopper=0;
	string completedArg;
    string chopped;
	int cursor = 1;
	values tempValue;
    for (int i=0; i<databaseList[useIndex].tables.size(); i++)
    {
        //Finds current table
        if(tokens[2] == databaseList[useIndex].tables[i].name)
        {

		    for(k; k < tokens[3].size();k++)
		    {

		        if(tokens[3].at(k) == ',' )
		        {

		            if(cursor == 1)
		            {
		            	completedArg = tokens[3].substr(cursor+7, k-7-cursor);
		            	//setUppercase()
		            	//transform(completedArg.begin(), completedArg.end(), completedArg.begin(), std::ptr_fun<int, int>(std::toupper));	            	
		            	tempValue.dataMembers.push_back(completedArg);
		            }
		            else
		            {
		            	completedArg = tokens[3].substr(cursor+2, k-cursor-2);
		            	//setUppercase()
		            	//transform(completedArg.begin(), completedArg.end(), completedArg.begin(), std::ptr_fun<int, int>(std::toupper));
		          	 	tempValue.dataMembers.push_back(completedArg);

		            }
		            cursor=k++;
		        }

			}	
			completedArg = tokens[3].substr(cursor+1);
			k=0;
			cursor = 0;
			for(k; k < completedArg.size();k++)
			{
				if( isdigit(completedArg.at(k)) )
				{
					cursor++;
				}
				if(completedArg.at(k)==';')
				{
					completedArg = completedArg.substr(completedArg.size() - (cursor+3), cursor+1);
				}
			}	
			//setUppercase()
			//transform(completedArg.begin(), completedArg.end(), completedArg.begin(), std::ptr_fun<int, int>(std::toupper));	
    		tempValue.dataMembers.push_back(completedArg);
            databaseList[useIndex].tables[i].tableValues.push_back(tempValue);
            databaseList[useIndex].tables[i].valuesInserted++;
            cout << "-- 1 new record inserted " << endl;
            return 1;
        }
    }
    cout << "-- !Failed to alter table " << tokens[2] << " because it does not exist" << endl;
    return 0;

}
//Prints values from table
bool selectFrom(vector <string> tokens)
{
	string tempString;
	tempString = tokens[3].substr(0,tokens[3].size() -1);
	tempString = removeEnd(tempString);
	//setUppercase()
	//transform(tempString.begin(), tempString.end(), tempString.begin(), std::ptr_fun<int, int>(std::toupper));
	for (int i=0; i<databaseList[useIndex].tables.size(); i++)
    {
        if(tempString == databaseList[useIndex].tables[i].name)
        {
        	printTable(databaseList[useIndex].tables[i]);
			return 1;
        }
    }
    cout << "-- !Failed to print table " << tokens[3] << " because it does not exist" << endl;
    return 0;

}

//Prints the variable names of the table 
//Then it finds what table entries are valid to print and calls on the printquery function
bool selectFromQuery(vector <string> Atributes, string Name, vector <string> tokens)
{
	string tempString;
	vector<string> cutString;
	//Atributes.erase(Atributes.begin() + (Atributes.size()-1) );
	Atributes[0] = removeEnd(Atributes[0]);
	int tracker = 0;

	Atributes.pop_back();
	Atributes.pop_back();
	
	//Atributes.pop_back();
	//cout << Atributes[0] << " "<< Atributes[1] << endl;
	//cout << tokens[0] << " "<< tokens[1] << endl;
	tokens[3] = removeEnd(tokens[3]);
	for (int tableSelected=0; tableSelected<databaseList[useIndex].tables.size(); tableSelected++)
    {
    	//IF table exists then we start by printing the variable names and type
        if(Name == databaseList[useIndex].tables[tableSelected].name)
        {
        	for(int currentArg=0; currentArg<databaseList[useIndex].tables[tableSelected].arguments.size(); currentArg++)
			{
				for(int constraints=0;constraints<Atributes.size();constraints++)
				{
					cutString = splitString(databaseList[useIndex].tables[tableSelected].arguments[currentArg]);
					cutString[0] = setUppercase(cutString[0]);

					//cout << cutString[0] << "-" << Atributes[constraints] << endl;	
					if(cutString[0] == Atributes[constraints])
					{
						
						if(tracker>0)
							cout << " | ";	

						tracker=1;
						cout <<	databaseList[useIndex].tables[tableSelected].arguments[currentArg] ;
						
								
					}
				}
				
			}
			cout << endl;
			//After printing the table variable we begin the querying process
        	for(int argumentSelected=0; argumentSelected<databaseList[useIndex].tables[tableSelected].arguments.size();argumentSelected++)
			{

				cutString = splitString(databaseList[useIndex].tables[tableSelected].arguments[argumentSelected]);
				cutString[0] = setUppercase(cutString[0]);
				tokens[3] = setUppercase(tokens[3]);

				if(tokens[1] == cutString[0])
				{

					if(tokens[2].at(0) == '=' )
					{

						for(int tableEntry=0; tableEntry<databaseList[useIndex].tables[tableSelected].tableValues.size(); tableEntry++)
						{
							//cout << tokens[3] << "-" << databaseList[useIndex].tables[tableSelected].tableValues[g].dataMembers[j] << endl;
							if(tokens[3] == databaseList[useIndex].tables[tableSelected].tableValues[tableEntry].dataMembers[argumentSelected])
							{	
								printQueryTable(databaseList[useIndex].tables[tableSelected].tableValues[tableEntry], Atributes, tableSelected);
							}
						}
					}
					if(tokens[2].at(0) == '<' )
					{

						for(int tableEntry=0; tableEntry<databaseList[useIndex].tables[tableSelected].tableValues.size(); tableEntry++)
						{
							//cout << tokens[3] << "-" << databaseList[useIndex].tables[tableSelected].tableValues[g].dataMembers[j] << endl;
							if(tokens[3] > databaseList[useIndex].tables[tableSelected].tableValues[tableEntry].dataMembers[argumentSelected])
							{	
								printQueryTable(databaseList[useIndex].tables[tableSelected].tableValues[tableEntry], Atributes, tableSelected);
							}
						}
					}
					if(tokens[2].at(0) == '>' )
					{

						for(int tableEntry=0; tableEntry<databaseList[useIndex].tables[tableSelected].tableValues.size(); tableEntry++)
						{
							//cout << tokens[3] << "-" << databaseList[useIndex].tables[tableSelected].tableValues[g].dataMembers[j] << endl;
							if(tokens[3] < databaseList[useIndex].tables[tableSelected].tableValues[tableEntry].dataMembers[argumentSelected])
							{	
								printQueryTable(databaseList[useIndex].tables[tableSelected].tableValues[tableEntry], Atributes, tableSelected);
							}
						}
					}
					if(tokens[2].at(0) == '!' )
					{

						for(int tableEntry=0; tableEntry<databaseList[useIndex].tables[tableSelected].tableValues.size(); tableEntry++)
						{
							//cout << tokens[3] << "-" << databaseList[useIndex].tables[tableSelected].tableValues[tableEntry].dataMembers[argumentSelected] << endl;
							if(tokens[3] != databaseList[useIndex].tables[tableSelected].tableValues[tableEntry].dataMembers[argumentSelected])
							{	
								printQueryTable(databaseList[useIndex].tables[tableSelected].tableValues[tableEntry], Atributes, tableSelected);
							}
						}
					}

				}

			}
			return 1;
        }
    }
    cout << "-- !Failed to print table " << tokens[3] << " because it does not exist" << endl;
    return 0;

}

//Functions to prints entries in tables
//Only prints arguments that matcht the query that is recieved
void printQueryTable(values currentEntry, std::vector <string> Atributes, int tableSelected)
{
	vector <string> cutString;
	
	int tracker=0;
	//Atributes.erase(Atributes.begin() + (Atributes.size()-3) );
	for(int currentArg=0; currentArg<currentEntry.dataMembers.size(); currentArg++)
	{
		for(int constraints=0;constraints<Atributes.size();constraints++)
		{
			cutString = splitString(databaseList[useIndex].tables[tableSelected].arguments[currentArg]);
			cutString[0] = setUppercase(cutString[0]);

			//cout << cutString[0] << " " << Atributes[constraints] << endl;
			if(cutString[0] == Atributes[constraints])
			{
				if(tracker>0)
					cout << " | ";	

				cout << currentEntry.dataMembers[currentArg] ;
				tracker=1;
				
			}
		}
		
	}
	if(tracker>0)
	{
		cout << endl;
	}
	

}
// prints the inputed table
void printTable(table currentTable){
	for(int p=0; p<currentTable.arguments.size(); p++)
	{
		if(p == currentTable.arguments.size() - 1)
			cout << currentTable.arguments[p] << endl;
		else
			cout << currentTable.arguments[p] << " | ";
	}
	for(int j=0; j<currentTable.valuesInserted;j++)
	{

		
		for(int z=0; z<currentTable.tableValues[j].dataMembers.size(); z++)
		{
				cout << currentTable.tableValues[j].dataMembers[z];
				if(z!=currentTable.tableValues[j].dataMembers.size()-1)	
				{
					cout << " | ";
				}
		}
		cout << endl;
	}
}

//Unimplemented just prints table
/*void printQueryTable(table currentTable, std::vector<string> Atributes){
	std::vector <string> argumentAndValue;
	for(int i=0; i< currentTable.arguments.size(); i++)
	{
		argumentAndValue = splitString(currentTable.arguments[i]);
		for (int j=0; j< Atributes.size(); j++){

			// Only printing one attribute right now ???
			if (argumentAndValue[0] == Atributes[j]){
				if(j == Atributes.size() - 1)
					cout << Atributes[j];
				else
					cout << Atributes[j] << " | ";
			}
		}
		
	}
	cout << endl;

	for(int i=0; i<currentTable.valuesInserted;i++)
	{

		
		for(int j=0; j<currentTable.tableValues[i].dataMembers.size(); j++)
		{
				cout << currentTable.tableValues[i].dataMembers[j];
				if(j!=currentTable.tableValues[i].dataMembers.size()-1)	
				{
					cout << " | ";
				}
		}
		cout << endl;
	}
}*/


//Checks if table exists and then calls on deleteQuery
bool deleteFrom(string tableName, vector <string> tokens )
{
	for (int i=0; i<databaseList[useIndex].tables.size(); i++)
    {
		
		if(tableName == databaseList[useIndex].tables[i].name)
        {
        	deleteQuery(i, tokens );
        	return 1;
        }

    }
    cout << "-- !Failed to delete from table " << tableName << " because it does not exist" << endl;
    return 0;


}
//Deletes based on query
bool deleteQuery(int tableSelected, vector <string> tokens)
{
	vector<string> newString;
	int changes= 0;
	string::size_type sz; 
	float tempFloat;
	float tempFloat2;

	if(tokens[0] == "WHERE")
	{
		//for(int k=0;k<tokens.size())
		
		for(int j=0; j<databaseList[useIndex].tables[tableSelected].arguments.size();j++)
		{

			newString = splitString(databaseList[useIndex].tables[tableSelected].arguments[j]);
			

			////setUppercase()
			//transform(newString[0].begin(), newString[0].end(), newString[0].begin(), std::ptr_fun<int, int>(std::toupper));
			if(tokens[1] == newString[0])
			{
				//cout << "argument recognized " << endl;
				tokens[3] = removeEnd(tokens[3]);
				tokens[3] = removeFrontSpaces(tokens[3]);
				
				if(tokens[2].at(0) == '=' )
				{

					for(int g=0; g<databaseList[useIndex].tables[tableSelected].tableValues.size(); g++)
					{
						
						if(tokens[3] == databaseList[useIndex].tables[tableSelected].tableValues[g].dataMembers[j])
						{
							
							changes++;
							databaseList[useIndex].tables[tableSelected].tableValues.erase(databaseList[useIndex].tables[tableSelected].tableValues.begin()+g);
							databaseList[useIndex].tables[tableSelected].valuesInserted--;
							g=-1;
						}
					}
				}
				//cout << tokens[2].at(0) << endl;
				if(tokens[2].at(0) == '>' )
				{
					
					for(int g=0; g<databaseList[useIndex].tables[tableSelected].tableValues.size(); g++)
					{

						
						databaseList[useIndex].tables[tableSelected].tableValues[g].dataMembers[j] = removeFrontSpaces(databaseList[useIndex].tables[tableSelected].tableValues[g].dataMembers[j]);

						//cout << databaseList[useIndex].tables[tableSelected].tableValues[g].dataMembers[j] << endl;
						//cout << tokens[3] << endl;
						tempFloat = stof (tokens[3] ,&sz);
						tempFloat2 = stof (databaseList[useIndex].tables[tableSelected].tableValues[g].dataMembers[j] ,&sz);
						if(tempFloat < tempFloat2)
						{
							
							changes++;
							databaseList[useIndex].tables[tableSelected].tableValues.erase(databaseList[useIndex].tables[tableSelected].tableValues.begin()+g);
							databaseList[useIndex].tables[tableSelected].valuesInserted--;
							g=-1;
						}
						else
						{
							//cout << databaseList[useIndex].tables[tableSelected].tableValues[g].dataMembers[j] << endl;
							//cout << tokens[3] << endl;
						}
					}
				}
				;
				if(tokens[2].at(0) == '<' )
				{
					for(int g=0; g< databaseList[useIndex].tables[tableSelected].tableValues.size(); g++)
					{

						databaseList[useIndex].tables[tableSelected].tableValues[g].dataMembers[j] = removeFrontSpaces(databaseList[useIndex].tables[tableSelected].tableValues[g].dataMembers[j]);
						//cout << databaseList[useIndex].tables[tableSelected].tableValues[g].dataMembers[j] << endl;
						tempFloat = stof (tokens[3] ,&sz);
						tempFloat2 = stof (databaseList[useIndex].tables[tableSelected].tableValues[g].dataMembers[j] ,&sz);
						cout << tempFloat << "-" << tempFloat2 << endl;
						if(tempFloat > tempFloat2)
						{
							
							changes++;
							databaseList[useIndex].tables[tableSelected].tableValues.erase(databaseList[useIndex].tables[tableSelected].tableValues.begin()+g);
							databaseList[useIndex].tables[tableSelected].valuesInserted--;
							g=-1;
						}
						else
						{
							//cout<< databaseList[useIndex].tables[tableSelected].name << endl;
							//cout << tempFloat << "=" << tempFloat2 << endl;
						}
						
					}
				}
				
				

			}
		}
		if(changes == 1)
			cout << "--" << changes << " record deleted" << endl;
		else
			cout << "--" << changes << " records deleted" << endl;

		return 1;


	}
	cout << "-- !Failed to delete from table " << databaseList[useIndex].tables[tableSelected].name << " because no matching query" << endl;
    return 0;
}
//Removes spaces from beginning of strings
string removeFrontSpaces(string cleanString)
{
	int count = 0;
	
	for(int i=0; i<cleanString.size();i++)
	{
		if(cleanString.at(i) == ' ' || cleanString.at(i) == '\t'/*|| cleanString.at(i) == NULL */)
			count++;
		else
			break;

	}
	cleanString = cleanString.substr(count, cleanString.size()- count);
	
	return cleanString;
}
//Returns a vector of strings by splitting up the input string where spaces are
vector <string> splitString(string inputString)
{
	vector <string> newString;
	int cursor =0;
	string chop;
	int i=0;
	for(i;i<inputString.size();i++)
	{
		if(inputString.at(i) == ' ')
		{
			chop = inputString.substr(0, i);
			break;
		}
	}
	newString.push_back(chop);
	chop = inputString.substr(i, inputString.size()-i);
	chop = removeFrontSpaces(chop);
	newString.push_back(chop);
	return newString;
}
//Checks if the last character is a ';' or ',' and if it is removes it
string removeEnd(string cleanString)
{
	if(cleanString.at(cleanString.size()-1) == ';' || cleanString.at(cleanString.size()-1) == ',')
	cleanString = cleanString.substr(0, cleanString.size()-1);

	return cleanString;
}
//CHecks if table exist and if it does calls on update query
bool update(string tableName, vector <string> tokens, vector <string> queries )
{
	for (int i=0; i<databaseList[useIndex].tables.size(); i++)
    {
		tableName = setUppercase(tableName);
		//transform(tableName.begin(), tableName.end(), tableName.begin(), std::ptr_fun<int, int>(std::toupper));
		//databaseList[useIndex].tables[i].name = setUppercase(databaseList[useIndex].tables[i].name);
		//transform(databaseList[useIndex].tables[i].name.begin(), databaseList[useIndex].tables[i].name.end(), databaseList[useIndex].tables[i].name.begin(), std::ptr_fun<int, int>(std::toupper));
		//cout << tableName << endl;
        if(tableName == databaseList[useIndex].tables[i].name)
        {
        	updateQuery(i, tokens, queries );
        	return 1;
        }

    }
    cout << "-- !Failed to update table " << tableName << " because it does not exist" << endl;
    return 0;
	
}
//USes query to find where data needs to be changed and calls change Data
bool updateQuery(int tableSelected,  vector <string> queries, vector <string> tokens)
{
	vector<string> newString;
	int changes= 0;
	string::size_type sz; 
	float tempFloat;
	float tempFloat2;

	if(tokens[0] == "WHERE")
	{
		
		for(int j=0; j<databaseList[useIndex].tables[tableSelected].arguments.size();j++)
		{

			newString = splitString(databaseList[useIndex].tables[tableSelected].arguments[j]);
			newString[0] = setUppercase(newString[0]);
			//transform(newString[0].begin(), newString[0].end(), newString[0].begin(), std::ptr_fun<int, int>(std::toupper));
			tokens[3] = setUppercase(tokens[3]);
			//transform(tokens[3].begin(), tokens[3].end(), tokens[3].begin(), std::ptr_fun<int, int>(std::toupper));
			if(tokens[1] == newString[0])
			{

				//cout << "argument recognized " << endl;
				tokens[3] = removeEnd(tokens[3]);
				tokens[3] = removeFrontSpaces(tokens[3]);
				
				if(tokens[2].at(0) == '=' )
				{

					for(int g=0; g<databaseList[useIndex].tables[tableSelected].tableValues.size(); g++)
					{
						//cout << tokens[3] << "-" << databaseList[useIndex].tables[tableSelected].tableValues[g].dataMembers[j] << endl;
						if(tokens[3] == databaseList[useIndex].tables[tableSelected].tableValues[g].dataMembers[j])
						{
							
							changes++;
							changeData(tableSelected, g, j, queries);
							//databaseList[useIndex].tables[tableSelected].tableValues[g].dataMembers[j] = 
					
						}
					}
				}
				//cout << tokens[2].at(0) << endl;
				if(tokens[2].at(0) == '>' )
				{
					
					for(int g=0; g<databaseList[useIndex].tables[tableSelected].tableValues.size(); g++)
					{

						
						databaseList[useIndex].tables[tableSelected].tableValues[g].dataMembers[j] = removeFrontSpaces(databaseList[useIndex].tables[tableSelected].tableValues[g].dataMembers[j]);

						//cout << databaseList[useIndex].tables[tableSelected].tableValues[g].dataMembers[j] << endl;
						//cout << tokens[3] << endl;
						tempFloat = stof (tokens[3] ,&sz);
						tempFloat2 = stof (databaseList[useIndex].tables[tableSelected].tableValues[g].dataMembers[j] ,&sz);
						if(tempFloat < tempFloat2)
						{
							
							changes++;
							changeData(tableSelected, g, j, queries);
						}

					}
				}
				;
				if(tokens[2].at(0) == '<' )
				{
					for(int g=0; g< databaseList[useIndex].tables[tableSelected].tableValues.size(); g++)
					{

						databaseList[useIndex].tables[tableSelected].tableValues[g].dataMembers[j] = removeFrontSpaces(databaseList[useIndex].tables[tableSelected].tableValues[g].dataMembers[j]);
						//cout << databaseList[useIndex].tables[tableSelected].tableValues[g].dataMembers[j] << endl;
						tempFloat = stof (tokens[3] ,&sz);
						tempFloat2 = stof (databaseList[useIndex].tables[tableSelected].tableValues[g].dataMembers[j] ,&sz);
						//cout << tempFloat << "-" << tempFloat2 << endl;
						if(tempFloat > tempFloat2)
						{
							
							changes++;
							changeData(tableSelected, g, j, queries);
						}
	
					}
				}
				
				

			}
		}
		if(changes == 1)
			cout << "--" << changes << " record modified" << endl;
		else
			cout << "--" << changes << " records modified" << endl;

		return 1;


	}
	cout << "-- !Failed to update from table " << databaseList[useIndex].tables[tableSelected].name << " because no matching query" << endl;
    return 0;
	return 0;
}
//Updates data in  a tuple based of query selection from updatequery
bool changeData(int tableSelected, int tableEntry, int dataEntry, vector <string> queries)
{
	vector<string> newString;
	for(int j=0; j<databaseList[useIndex].tables[tableSelected].arguments.size();j++)
	{

		newString = splitString(databaseList[useIndex].tables[tableSelected].arguments[j]);
		//newString[0] = setUppercase(newString[0]);
		//transform(newString[0].begin(), newString[0].end(), newString[0].begin(), std::ptr_fun<int, int>(std::toupper));
		if(queries[1] == newString[0])
		{
			//cout << queries[1] << "==" << newString[0] << endl;
			//cout << databaseList[useIndex].tables[tableSelected].tableValues[tableEntry].dataMembers[j] << "---->" << queries[3] << endl;
			databaseList[useIndex].tables[tableSelected].tableValues[tableEntry].dataMembers[j] = queries[3];
		}
	}



	return 0;
}
//Sets string to all uppercase when it is applicable
string setUppercase(std::string token){
	
	transform(token.begin(), token.end(), token.begin(), std::ptr_fun<int, int>(std::toupper));
	return token;
}