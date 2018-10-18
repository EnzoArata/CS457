/*
    Enzo Arata
    Zachary Mcilwain
    CS 457
    Project 1
    September 27th
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

using namespace std;



int useIndex = -1;


vector<string> commandParse(string commandLine);
bool createDatabase(string name);
bool createTable(vector <string> tokens);
bool argumentParse(string arguments);
bool selectFrom(string name);
bool dropDatabase(string name);
bool dropTable(string name);
bool use(string name);
bool clearData();
bool add(vector <string> tokens);

//Data type for our tables
struct table{
    string name;
    int args=0;
    vector <string> arguments;

};
//Data tyoe for our databases
struct dataBase{
    string name;
    vector <table> tables;
    int tableIndex = -1;
};

vector <dataBase> databaseList;




int main( int argc, char *argv[] ){


	vector <string> commands;

	vector <string> input;

    ifstream inFile;
	//inFile = fopen(argv[2],"r");
    inFile.open(argv[1]);

   // File input into a vector of strings, igoring lines with "--"
	string currentLine;
	while(getline(inFile, currentLine)){
       // cout << currentLine << endl;
        if (currentLine.substr(0,2)=="--")
            continue;
        input.push_back(currentLine);
	}



    //loop that runs on number of strings collected from file
	for(int i = 0; i< input.size();i++){

        //Calls on parsing function
		commands = commandParse(input[i]);

        //Checks tokens of string to decipher command
		if (commands[0] == "CREATE"){
			if (commands[1] == "DATABASE"){
				createDatabase(commands[2]);
			}
			if (commands[1] == "TABLE"){
                createTable(commands);
			}
		}
		else if (commands[0] == "DROP")
		{
			if (commands[1] == "DATABASE"){
                dropDatabase(commands[2]);
			}
			if (commands[1] == "TABLE"){
                dropTable(commands[2]);
			}

		}
		else if (commands[0] == "USE")
		{
			use(commands[1]);
		}
		else if (commands[0] == "SELECT")
		{
            selectFrom(commands[3]);
		}
		else if (commands[0] == "ALTER")
		{
            add(commands);
		}
		else if (commands[0] == "EXIT")
		{
            clearData();
            cout << "-- Exiting program" << endl;
			return 0;
		}
		else {
			//cout << "-- Command not recognized" << endl;
		}
	}



}
//Parses string by making new strings where there are spaces
vector<string> commandParse(string commandLine){
	vector <string> commands;
	string chopped;
	int i;
	int cursor = 0;
	for(i=0; i<commandLine.size(); i++){
		if (commandLine.at(i)==' '){
			commands.push_back(commandLine.substr(cursor,i-cursor));
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
	chopped = commandLine.substr(cursor);
	chopped = chopped.substr(0, chopped.size()-1);
	commands.push_back(chopped);


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


    table tempTable = {tokens[2], 0, t_string  };
	databaseList[useIndex].tables.push_back(tempTable);
    databaseList[useIndex].tableIndex = databaseList[useIndex].tables.size()-1;
    //dataBase tempData = {name, emptyVec};
    //cout << "token 3 = ";
    //cout << tokens[3] << endl;
    argumentParse(tokens[3]);






}
//Parses string into arguments for tables
bool argumentParse(string t_arg)
{
   // cout << "In Argument Parser" << endl;
    int i=1;
    string completedArg;
    string chopped;
	int cursor = 1;
   // cout << t_arg << endl;
    for(i; i < t_arg.size();i++)
    {
        if(t_arg.at(i) == ',' )
        {
            //cout << "assigning arugment " << endl;
            completedArg = t_arg.substr(cursor+1, i-cursor-1);
            //cout << useIndex << " " << tableIndex << endl;
            databaseList[useIndex].tables[databaseList[useIndex].tableIndex].args++;
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
        chopped = chopped.substr(0, chopped.size()-1);
        //completedArg = t_arg.substr(cursor+2);


	}
    databaseList[useIndex].tables[databaseList[useIndex].tableIndex].args++;
    databaseList[useIndex].tables[databaseList[useIndex].tableIndex].arguments.push_back(chopped);

    cout << "-- Table " << databaseList[useIndex].tables[databaseList[useIndex].tableIndex].name << " created." << endl;

   /* for(int j = 0; j< databaseList[useIndex].tables[tableIndex].args;j++)
    {
        cout << databaseList[useIndex].tables[tableIndex].arguments[j] << " ";
    }
    cout << endl;*/

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
bool selectFrom(string name)
{
    for(int j =0;j< databaseList[useIndex].tables.size();j++)
    {
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
}
//adds a new data type to given table
bool add(vector <string> tokens)
{
     string joinedString;
     for (int i=0; i<databaseList[useIndex].tables.size(); i++){
        if(tokens[2] == databaseList[useIndex].tables[i].name)
        {
            joinedString = tokens[4] + tokens[5];
            databaseList[useIndex].tables[i].args++;
            databaseList[useIndex].tables[i].arguments.push_back(joinedString);
             cout << "-- Table " << tokens[2] << " modified." << endl;
            return 1;

        }


    }
    cout << "-- !Failed to alter table " << tokens[2] << " because it does not exist" << endl;
    return 0;


}

