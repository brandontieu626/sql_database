#ifndef SQL_H
#define SQL_H
#include "../../!includes/parser_class/parser.h"
#include "../../!includes/table_class/table.h"
class SQL{
public:
    SQL(char filename[]);  //Batch Mode
    SQL();                 //Regular Mode

    void run();           //Run calls run command line

    void run_command_line(char command_line[]); //Processes each input
    void run_batch(char file_name[]);           //Run Batch, reads lines from input, passes to
                                                //run command line
    //Get Command from parse tree
    void get_sql_command(MMap<string,string>& ptree,string& command);

    //Get Fields from parse tree
    void get_sql_fields(MMap<string,string>& ptree,vectorstr& fields);

    //Get Conditions from parse tree
    void get_sql_conditions(MMap<string,string>& ptree,vectorstr& conditions);

    //Get Table name from Parse tree
    void get_sql_table_name(MMap<string,string>& ptree,string& tablename);

    //Get File name from SQL
    void get_sql_file(MMap<string,string>& ptree,string& filename);

    //Delete File
    void delete_file(const string& filename);

private:
    Parser p;         //Parse Tree

    Map<string,Table> _tables; //Map of Tables

    fstream b;                //Fstream object read output to a text file

    bool batch_mode;          //Batch Mode=True? Write to File

    int command_num;          //Command Number


};

SQL::SQL(char filename[]):p(),_tables(true),batch_mode(true),command_num(0){
    /**
       * @brief DEBUG
       * function:SQL CTOR Batch mode
       * purpose: Command Line is in batch mode, so it reads to a textfile called output
       * Starts counting command numbers too
       * parameters:char filename[]
       */

    fstream f;
    string table_name;

    //Open Tables Txt File
    open_fileRW(f,"_tables.txt");

    //Read in tables
    cout<<"Tables I'm Managing:"<<endl;
    while(f>>table_name){

        cout<<table_name<<endl;
        //Save table name with Table object reopening the table
        _tables[table_name]=Table(table_name);
    }

    cout<<"----------------------------"<<endl<<endl;

    //Close File
    f.close();

    //Open text file
    open_fileW(b,"_output.txt");

    //Run Batch
    run_batch(filename);

    //Close file
    b.close();


}

SQL::SQL():p(),_tables(true),batch_mode(false),command_num(0){
    /**
       * @brief DEBUG
       * function:SQL CTOR
       * purpose:Default CTOR for SQL
       * Default CTOR for parser
       * parameters:none
       */
    fstream f;
    string table_name;

    //Open Tables Txt File
    open_fileRW(f,"_tables.txt");

    //Read in tables
    cout<<"Tables I'm Managing:"<<endl;
    while(f>>table_name){

        cout<<table_name<<endl;
        //Save table name with Table object reopening the table
        _tables[table_name]=Table(table_name);
    }

    cout<<"----------------------------"<<endl<<endl;

    //Close File
    f.close();



}

void SQL::run(){
    /**
       * @brief DEBUG
       * function:Run
       * purpose:Loop processing input
       * Repeatedly call command line until input="X"
       * parameters:none
       */

    //Cstr to hold input
    char cstr[200];

    //String to hold input
    string input;

    do{
        //Prompt
        cout<<">";

        //Grab input
        getline(cin,input);


        //Change to CStr
        strcpy(cstr,input.c_str());

        //Pass it to command line to process
        run_command_line(cstr);



    }while(input!="X");


}


void SQL::run_command_line(char command_line[]){
    /**
       * @brief DEBUG
       * function:Run Command Line
       * purpose:Process command line using parse tree
       * To perform whatever command is needed
       * parameters:char command_line[]
       */

    //Vector for fields
    vectorstr fields;

    //Vector for conditions
    vectorstr conditions;

    //Filename
    string file_name;

    //Tablename
    string table_name;

    //Command
    string command;

    string input=command_line;

    //Parse command
    p.set_string(command_line);

    //Save Parsetree
    MMap<string,string> ptree=p.parse_tree();

    //If the parsetree parsed successfully
    if(p.is_success()){

        //Grab command
        get_sql_command(ptree,command);

        //If it is in batch mode, print each command with command num
        if(batch_mode){
            cout<<"["<<command_num<<"]"<<input<<endl<<endl;
            b<<"["<<command_num<<"]"<<input<<endl<<endl;
            command_num++;
        }

        //If the command was select
        if(command=="select"){

            //Grab table name
            get_sql_table_name(ptree,table_name);

            //If table exists
            if(_tables.contains(table_name)){

                //If it is a where
                if(ptree.contains("where")){

                    //Grab both fields + conditions
                    get_sql_fields(ptree,fields);
                    get_sql_conditions(ptree,conditions);

                    //Save table
                    Table selected=_tables[table_name].select(fields,conditions);

                    //Print table
                    cout<<selected;

                    //Write into text file when in batch mode
                    if(batch_mode){
                       b<<selected<<endl;
                    }


                }
                //If it is only fields
                else{
                    //Grab fields
                    get_sql_fields(ptree,fields);

                    //Save table
                    Table selected=_tables[table_name].select(fields);

                    //Print table
                    cout<<selected;

                    //Write into text file when in batch mode
                    if(batch_mode){
                        b<<selected<<endl;
                    }
                }

            }
            //Table doesn't exist
            else{
                cout<<"NO SUCH TABLE EXISTS"<<endl<<endl;
            }

        }
        //Insert Command
        else if(command=="insert"){

            //Get Table Name
            get_sql_table_name(ptree,table_name);

            //If the table exists
            if(_tables.contains(table_name)){

                //Grab fields
                get_sql_fields(ptree,fields);

                //SQL Prompt for Insert
                cout<<"SQL::Inserting "<<fields<<" into Table "<<table_name<<endl<<endl;

                //Insert into table
                _tables[table_name].insert_into(fields);

            }
            //Table doesn't exist
            else{
                cout<<"NO SUCH TABLE EXISTS"<<endl<<endl;
            }

        }
        //Make/Creating command
        else if(command=="make"){

            //Get table name
            get_sql_table_name(ptree,table_name);

            //If _tables doesn't already contain the table
            if(!_tables.contains(table_name)){

                //Reset Serial
                Table::serial=100;

                //Grab fields
                get_sql_fields(ptree,fields);

                //SQL Prompt for Creating/Making
                cout<<"SQL::Table "<<table_name<<" created: fields:"<<fields<<endl<<endl;

                //Create Table
                Table t(table_name,fields);

                //Adding to tables list
                _tables[table_name]=t;

                //Print Table
                cout<<_tables[table_name]<<endl;

                //Save Table name into file
                fstream f;

                //Open File
                open_fileRW(f,"_tables.txt");

                //Write to end of file
                f.seekg(0,ios_base::end);

                //Write into file
                f<<table_name<<endl;

                //Close it
                f.close();

            }
            //Table exists already
            else{
                cout<<"TABLE IN NAME EXISTS ALREADY"<<endl<<endl;
            }



        }
        //Command is Delete
        else if(command=="delete"){

            //Grab table name
            get_sql_table_name(ptree,table_name);

            //If table exists
            if(_tables.contains(table_name)){
                //Get conditions
                get_sql_conditions(ptree,conditions);

                //SQL Prompt for Deleting
                cout<<"SQL::Deleting "<<conditions<<" from "<<table_name<<endl<<endl;

                //Delete from Table
                _tables[table_name].delete_where(conditions);

                //Print Table
                cout<<_tables[table_name];
            }
            //Table doesn't exist
            else{
                cout<<"NO SUCH TABLE EXISTS"<<endl<<endl;
            }


        }
        //Drop Table
        else if(command=="drop"){

            //Get Table name
            get_sql_table_name(ptree,table_name);

            //If table exists
            if(_tables.contains(table_name)){
                //SQL Prompt for Deleting
                cout<<"SQL::Dropping Table "<<table_name<<endl<<endl;

                //Remove from map of tables
                _tables.erase(table_name);

                //Delete File
                delete_file(table_name);
            }
            else{
                cout<<"NO SUCH TABLE EXISTS"<<endl<<endl;
            }
        }
        //Run command
        else if(command=="run"){

            //Get File name
            get_sql_file(ptree,file_name);


            //Hold for file name
            char hold_cstr[50];

            //Change string to cstr
            strcpy(hold_cstr,file_name.c_str());

            //Run batch
            run_batch(hold_cstr);


        }

        cout<<endl<<endl;

    }
    else{
        //If in batch mode, print each input
        if(batch_mode){
            cout<<input<<endl;
            b<<input<<endl;
        }
        //If not, print invalid inputs
        else{
            cout<<"INVALID INPUT"<<endl<<endl;
            cout<<endl<<endl;

        }
    }

}

void SQL::run_batch(char *file_name){
    /**
       * @brief DEBUG
       * function:run batch
       * purpose: Call Run command line until end of file
       * parameters:char *file_name
       */

    //Fstream object
    fstream f;

    //Input from file
    string input;

    //Cstr for input
    char command[400];

    //Open File
    open_fileRW(f,file_name);


    //Get line from file
    while(getline(f,input)){

        //Copy to CStr
        strcpy(command,input.c_str());

        //Pass to run command line
        run_command_line(command);

    }

    //Close File
    f.close();
}

void SQL::get_sql_command(MMap<string, string> &ptree,string &command){
    /**
       * @brief DEBUG
       * function:get sql command
       * purpose: set command=parse tree command
       * parameters:MMap<string, string> &ptree,string &command
       */
    command=ptree["command"][0];
}

void SQL::get_sql_fields(MMap<string, string> &ptree, vectorstr &fields){
    /**
       * @brief DEBUG
       * function:get sql fields
       * purpose: set fields=parse tree fields
       * parameters:MMap<string, string> &ptree, vectorstr &fields
       */
    fields=ptree["fields"];
}

void SQL::get_sql_conditions(MMap<string, string> &ptree, vectorstr &where){
    /**
       * @brief DEBUG
       * function:Get sql conditions
       * purpose:set conditions= ptree conditions
       * parameters:MMap<string, string> &ptree, vectorstr &where
       */
    where=ptree["conditions"];
}

void SQL::get_sql_table_name(MMap<string,string>& ptree,string& table_name){
    /**
       * @brief DEBUG
       * function:get sql table name
       * purpose:set table_name=ptreep["table"]
       * parameters:MMap<string,string>& ptree,string& table_name
       */
    table_name=ptree["table"][0];
}

void SQL::get_sql_file(MMap<string, string> &ptree, string &filename){
    /**
       * @brief DEBUG
       * function:get sql file
       * purpose:set filename=ptree["file"]
       * parameters:MMap<string, string> &ptree, string &filename
       */
    filename=ptree["file"][0];
}

void SQL::delete_file(const string& deleted){
    /**
       * @brief DEBUG
       * function:delete file
       * purpose:Delete specific table from _tables.txt
       * parameters:const string& deleted
       */

    //Serial Number for all tables
    string file_serial="_100.bin";

    //Get file with serial
    file_serial=deleted+file_serial;

    //Each Line
    string line;

    //Fstream object for _tables.txt
    fstream f;

    //Fstream object for copy
    fstream o;

    //Open Both Files
    open_fileRW(f,"_tables.txt");
    open_fileRW(o,"copy.txt");

    //Get line from _tables
    while(getline(f,line)){
        //If the table is not the line to be deleted
        //Write it to the copy
        if(line!=deleted){
            o<<line<<endl;
        }
    }

    //Close both file
    o.close();
    f.close();

    //Remove tables txt
    remove("_tables.txt");

    //Delete binary file of table
    remove(file_serial.c_str());

    //Rename txt
    rename("copy.txt","_tables.txt");




}
#endif // SQL_H
