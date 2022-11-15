#ifndef PARSER_H
#define PARSER_H
#include "../../!includes/bptmmap_class/bptmmap_class.h"
#include "../../!includes/bptmap_class/bptmap_class.h"
#include "../../!includes/string_tokenizer_class/stokenizer.h"
#include "parser_helpers.h"
#include <algorithm>
class Parser{
public:
    enum KEYS{ZERO,SELECT,FROM,WHERE,STAR,RELOP,LOGOP,COMMA,
              INSERT,INTO,VALUES,MAKE,
              TABLE,FIELDS,LPAR,RPAR,DELETE,DROP,RUN,EXT,SYM=30};

    Parser();                  //Sets up init matrix,make_matrix,_keywords,
    Parser(char* s);           //Sets up init matrix,make_matrix,_keywords,sets string

    void set_string(char* s);  //Tokenize buffer into input q

    int get_column(const string& token); //Maps Tokens to Keywords

    bool get_parse_tree();   //Sets p tree returns true if success/fail if not

    void build_keyword_list(); //builds map used to map keyworrds

    bool is_success();         //Return if parsing was successful

    MMap<string,string> parse_tree();

private:    
    void init_matrix(int matrix[][COLUMNS]);   //Initialize entire matrix to -1

    void make_matrix(int matrix[][COLUMNS]);   //Set up state machines

    MMap<string,string> ptree; //Parse tree

    Map<string,int> _keywords; //Keywords map

    static int _matrix[ROWS][COLUMNS]; //Adjacency Matrix

    Vector<string> input_q;           //Vector for tokenized tokens

    bool success;
};

int Parser::_matrix[ROWS][COLUMNS];

Parser::Parser():success(false){
    /**
       * @brief DEBUG
       * function:Parser CTOR
       * purpose:Build Keyword List,Init Matrix,Make Matrix
       * parameters: none
       */

    //set up keyword list
    build_keyword_list();

    //Initialize entire matrix to -1
    init_matrix(_matrix);

    //Set up matrix
    make_matrix(_matrix);
}

Parser::Parser(char* s){
    /**
       * @brief DEBUG
       * function: Parser CTOR
       * purpose: Build Keyword List,Init Matrix,Make Matrix,Set String
       * parameters: char * s
       */

    //set up keyword list
    build_keyword_list();

    //Initialize entire matrix to -1
    init_matrix(_matrix);

    //Set up matrix
    make_matrix(_matrix);

    //Tokenize input into input Queue and set PTree
    set_string(s);



}

void Parser::set_string(char* s){
    /**
       * @brief DEBUG
       * function:Set String
       * purpose:Tokenize input into Vector of Strings
       * Call get_parsetree to initialize parse tree
       * parameters: char* s
       */
    bool const DEBUG=false;


    //STokenizer object
    STokenizer stk(s);

    Token t;

    //Grab Token
    stk>>t;

    //Grab tokens and add them as strings into Vector of strings
    while(stk.more()){
        if(t.type()!=SPACE&&t.type()!=UNKNOWN){
            input_q+=t.token_str();
        }

        t=Token();

        stk>>t;
    }

    if(DEBUG){
        cout<<"INPUT Q:"<<endl;
        cout<<input_q<<endl;

    }

    //Clear Parse Tree before going to process another one
    ptree.clear();

    //Get Parse Tree sets up the parse tree and returns if it succeeded or failed
    //Set success state to if the parse tree was successful
    success=get_parse_tree();
}

int Parser::get_column(const string& token){
    /**
       * @brief DEBUG
       * function: get column
       * purpose: Map each string to its value
       * Uses the map _keywords
       * Return its integer value
       * parameters:
       */
    string lower_token=token;

    transform(lower_token.begin(),lower_token.end(),lower_token.begin(),::tolower);

    //Check if the keywords contains the string
    if(_keywords.contains(lower_token)){
        //If it does,return its value
        return _keywords[lower_token];
    }
    //If not,return symbol enum
    else{
        return SYM;
    }
}

bool Parser::get_parse_tree(){
    /**
       * @brief DEBUG
       * function:Get Parse Tree
       * purpose: Set up parse tree with appropriate keys/values
       * parameters: none
       */
    //State and Column
    int state=0;
    int column=0;

    //LPar and RPar Count
    int lpar_count=0;
    int rpar_count=0;

    //String to hold each token from input q
    string token;

    //Loop Integer
    int i=0;

    //Boolean to stop loop when a fail occurs
    bool fail_state=false;
    bool valid_parentheses=true;

    //Loop while there are still strings in input q and if you do not reach a failstate
    while((i<input_q.size())&&(fail_state!=true)){
        //Grab next string/token
        token=input_q[i];

        //Find it's column number
        column=get_column(token);

        //Get the next state
        state=_matrix[state][column];

        switch(state){
        //Command for select
        case 1:
            ptree["command"]+="select";
            break;
        //Fields, can not be a value with "
        case 2:
            if(token.at(0)=='"'){
                fail_state=true;
            }
            else{
                ptree["fields"]+=token;
            }
            break;
        //For Star
        case 3:
            if(token.at(0)=='"'){
                fail_state=true;
            }
            else{
                ptree["fields"]+=token;
            }
            break;
        //Table Name, can not have ""
        case 6:
            if(token.at(0)=='"'){
                fail_state=true;
            }
            else{
                ptree["table"]+=token;
            }
            break;
        //Marker for Where being true
        case 7:
            ptree["where"]+="yes";
            break;
        //Fields, can not be a value with ""
        case 8:
            if(token.at(0)=='"'){
                fail_state=true;
            }
            else{
                ptree["conditions"]+=token;
            }
            break;
        //Adding RelOp and Symbol
        case 9:
        case 10:
        case 11:
            if(token==")"){
                rpar_count++;
            }

            //Remove all quotes before inserting
            if(token.at(0)=='"'){
                string quotes="\"";
                for(char c:quotes){
                   token.erase(remove(token.begin(),token.end(),c),token.end());
                }
            }

            ptree["conditions"]+=token;
            break;
        //Left Parenthesis
        case 12:
            ptree["conditions"]+=token;
            lpar_count++;
            break;
        //For Insert
        case 20:
            ptree["command"]+="insert";
            break;
        //Insert Table
        //Do not allow quotations
        case 22:
            if(token.at(0)!='"'){
                ptree["table"]+=token;
            }
            else{
                fail_state=true;
            }
            break;
        //For Insert fields
        case 24:
            //Remove all quotations
            if(token.at(0)=='"'){
                string quotes="\"";
                for(char c:quotes){
                   token.erase(remove(token.begin(),token.end(),c),token.end());
                }
            }
            ptree["fields"]+=token;
            break;
        //For Creating/Make Command
        case 30:
            ptree["command"]+="make";
            break;
        //Add Table field for creating
        case 32:
            if(token.at(0)!='"'){
                ptree["table"]+=token;
            }
            else{
                fail_state=true;
            }
            break;
        //Add Fields for fields for creating
        case 34:
            if(token.at(0)!='"'){
                ptree["fields"]+=token;
            }
            else{
                fail_state=true;
            }
            break;
        //Delete Where
        case 40:
            ptree["command"]+="delete";
            break;
        //Table that is being deleted
        case 42:
            if(token.at(0)!='"'){
                ptree["table"]+=token;
            }
            else{
                fail_state=true;
            }
            break;
        case 43:
            ptree["where"]+="yes";
            break;
        //Fields for Deleting, No Quotations allowed
        case 44:
            if(token.at(0)!='"'){
                ptree["conditions"]+=token;
            }
            else{
                fail_state=true;
            }
            break;
        //Symbols and Right Parenthesis
        //Remove all quotations
        case 45:
        case 46:
            if(token==")"){
                rpar_count++;
            }

            if(token.at(0)=='"'){
                string quotes="\"";
                for(char c:quotes){
                   token.erase(remove(token.begin(),token.end(),c),token.end());
                }

            }
            ptree["conditions"]+=token;
            break;
        //Adding Logical Operator to Conditions
        case 47:
            ptree["conditions"]+=token;
            break;
        //Left Parenthesis
        case 51:
            ptree["conditions"]+=token;
            lpar_count++;
            break;
        //Drop Command
        case 52:
            ptree["command"]+="drop";
            break;
        //Adding table name, not allowing Quotations
        case 54:
            if(token.at(0)!='"'){
                ptree["table"]+=token;
            }
            else{
                cout<<"IN HERE AT THE FAIL"<<endl;
                fail_state=true;
            }

            break;
        //Run batch command
        case 55:
            ptree["command"]+="run";
            break;
        //Adding to the file name
        case 56:
            ptree["file"]+=token;
            break;
        case 57:
            ptree["file"][0]+=token;
            break;
        case 58:
            ptree["file"][0]+=token;
            break;
        case 59:
            ptree["file"][0]+=token;
            break;
        //If state is -1,fail state and set fail_state=true
        case -1:
            fail_state=true;
            break;
        default:
            break;
        }

        i++;

    }

    valid_parentheses=(lpar_count==rpar_count);


    //Check ending state for fail or success
    if(fail_or_success(_matrix,state)&&(fail_state!=true)&&
            valid_parentheses){
        //Empty input q afterwards
        input_q=Vector<string>();

        //PTree succeeded
        return true;

    }
    else{
        //Clear Ptree
        ptree.clear();

        //Empty input q afterwards
        input_q=Vector<string>();

        //PTree failed
        return false;
    }


}

void Parser::build_keyword_list(){
    /**
       * @brief DEBUG
       * function:build keyword list
       * purpose:Insert keywords into map
       * Set keywords=ENUM
       * parameters: none
       */
    _keywords["select"]=SELECT;
    _keywords["from"]=FROM;
    _keywords["where"]=WHERE;
    _keywords["*"]=STAR;
    _keywords["="]=RELOP;
    _keywords[">"]=RELOP;
    _keywords["<"]=RELOP;
    _keywords[">="]=RELOP;
    _keywords["<="]=RELOP;
    _keywords["and"]=LOGOP;
    _keywords["or"]=LOGOP;
    _keywords[","]=COMMA;
    _keywords["insert"]=INSERT;
    _keywords["into"]=INTO;
    _keywords["values"]=VALUES;
    _keywords["create"]=MAKE;
    _keywords["make"]=MAKE;
    _keywords["table"]=TABLE;
    _keywords["fields"]=FIELDS;
    _keywords["delete"]=DELETE;
    _keywords["drop"]=DROP;
    _keywords["run"]=RUN;
    _keywords["."]=EXT;
    _keywords["("]=LPAR;
    _keywords[")"]=RPAR;

}

bool Parser::is_success(){
    /**
       * @brief DEBUG
       * function: is success
       * purpose: return success
       * parameters: none
       */
    return success;
}

MMap<string,string> Parser::parse_tree(){
    /**
       * @brief DEBUG
       * function: Parse Tree
       * purpose: Return ptree
       * parameters: none
       */

    return ptree;

}

void Parser::init_matrix(int matrix[][COLUMNS]){
    /**
       * @brief DEBUG
       * function: init matrix
       * purpose: initialize everything in the matrix to -1
       * parameters: int matrix[][COLUMN]
       */
    //Initialize Rows
    for(int i=0;i<ROWS;i++){
        //Initialize Columns
        for(int j=0;j<COLUMNS;j++){
            matrix[i][j]=-1;
        }
    }
}

void Parser::make_matrix(int matrix[][COLUMNS]){
    /**
       * @brief DEBUG
       * function: Make Matrix
       * purpose: Setup state machines
       * Currently have:
       * Select
       * Insert
       * Create/Make
       * parameters:int matrix[][COLUMNS]
       */

    //Select State Machine
    mark_cells(matrix,0,SELECT,SELECT,1);

    mark_cells(matrix,1,SYM,SYM,2);

    mark_cells(matrix,1,STAR,STAR,3);

    mark_cells(matrix,2,COMMA,COMMA,4);

    mark_cells(matrix,2,FROM,FROM,5);

    mark_cells(matrix,4,SYM,SYM,2);

    mark_cells(matrix,3,FROM,FROM,5);

    mark_cells(matrix,5,SYM,SYM,6);

    mark_cells(matrix,6,WHERE,WHERE,7);

    mark_cells(matrix,7,SYM,SYM,8);

    mark_cells(matrix,7,LPAR,LPAR,12);

    mark_cells(matrix,12,LPAR,LPAR,12);

    mark_cells(matrix,12,SYM,SYM,8);

    mark_cells(matrix,8,RELOP,RELOP,9);

    mark_cells(matrix,9,SYM,SYM,10);

    mark_cells(matrix,10,LOGOP,LOGOP,11);

    mark_cells(matrix,10,RPAR,RPAR,10);

    mark_cells(matrix,11,SYM,SYM,8);

    mark_cells(matrix,11,LPAR,LPAR,12);

    mark_success(matrix,6);

    mark_success(matrix,10);



    //Insert State Machine
    mark_cells(matrix,0,INSERT,INSERT,20);

    mark_cells(matrix,20,INTO,INTO,21);

    mark_cells(matrix,21,SYM,SYM,22);

    mark_cells(matrix,22,VALUES,VALUES,23);

    mark_cells(matrix,23,SYM,SYM,24);

    mark_cells(matrix,24,COMMA,COMMA,25);

    mark_cells(matrix,25,SYM,SYM,24);

    mark_success(matrix,24);


    //Create/Make State Machine
    mark_cells(matrix,0,MAKE,MAKE,30);

    mark_cells(matrix,30,TABLE,TABLE,31);

    mark_cells(matrix,31,SYM,SYM,32);

    mark_cells(matrix,32,FIELDS,FIELDS,33);

    mark_cells(matrix,33,SYM,SYM,34);

    mark_cells(matrix,34,COMMA,COMMA,35);

    mark_cells(matrix,35,SYM,SYM,34);

    mark_success(matrix,34);

    //Delete Where State Machine
    mark_cells(matrix,0,DELETE,DELETE,40);

    mark_cells(matrix,40,FROM,FROM,41);

    mark_cells(matrix,41,SYM,SYM,42); //Table Name

    mark_cells(matrix,42,WHERE,WHERE,43);

    mark_cells(matrix,43,SYM,SYM,44); //Field Name

    mark_cells(matrix,43,LPAR,LPAR,51);

    mark_cells(matrix,51,LPAR,LPAR,51);

    mark_cells(matrix,51,SYM,SYM,44);

    mark_cells(matrix,44,RELOP,RELOP,45);

    mark_cells(matrix,45,SYM,SYM,46);

    mark_cells(matrix,46,LOGOP,LOGOP,47);

    mark_cells(matrix,46,RPAR,RPAR,46);

    mark_cells(matrix,47,SYM,SYM,44);

    mark_cells(matrix,47,LPAR,LPAR,51);

    mark_success(matrix,46);

    //Drop Table State Machine
    mark_cells(matrix,0,DROP,DROP,52);

    mark_cells(matrix,52,TABLE,TABLE,53);

    mark_cells(matrix,53,SYM,SYM,54);

    mark_success(matrix,54);


    //Run Batch State Machine
    mark_cells(matrix,0,RUN,RUN,55);

    mark_cells(matrix,55,SYM,SYM,56);

    mark_cells(matrix,56,SYM,SYM,57);

    mark_cells(matrix,57,SYM,SYM,57);

    mark_cells(matrix,57,EXT,EXT,58);

    mark_cells(matrix,58,SYM,SYM,59);

    mark_success(matrix,59);





}




#endif // PARSER_H
