#ifndef TABLE_H
#define TABLE_H
#include "../../!includes/record_class/record.h"
#include "../../!includes/bptmap_class/bptmap_class.h"
#include "../../!includes/bptmmap_class/bptmmap_class.h"
#include "../../!includes/shunting_yard/Shunting_yard.h"
#include "../../!includes/rpn/rpn.h"
class Table{
public:
    Table();             //Default CTOR
    Table(const string &fname,vectorstr fieldlist); //Creating new Table
    Table(const string &fname);                     //Opening rexisting table/file

    void insert_into(const vectorstr& fieldvalues);  //Insert these fields into
    void delete_where(const vectorstr& conditions);  //Delete these rows
    Table select_all();                              //Insert everything in the table
    Table select(const vectorstr& fields);           //Filtering columns only
    Table select(const vectorstr& fields,const vectorstr& conditions); //Filtering both columns and rows


    void reindex();                                    //Open Files and Reindex structures
    void print_table(ostream& outs) const;            //Print entire table


    //Ostream operator
    friend ostream& operator <<(ostream& outs,const Table& print_me){
        /**
           * @brief DEBUG
           * function:Ostream operator
           * purpose:Call print table from table
           * parameters:ostream& outs,const Table& print_me
           */

        print_me.print_table(outs);

        return outs;



    }

    static int serial;  //Starting serial
private:

    string _name;               //Name of table
    string _file_serial;
    string _filename;           //Filename

    vectorstr _fieldlist;       //List of fields

    int total_records;          //total records in this tanle

    Map<string,int> positions;  //Holds positions of each field in vector of multimaps

    //Each multimap holds all specific keys for fields
    Vector<MMap<string,long> > indices;
};

//Default Serial
int Table::serial=100;

Table::Table():positions(true),indices(true),total_records(0){
    /**
       * @brief DEBUG
       * function: Table CTOR
       * purpose: Default CTOR for everything
       * parameters: none
       */
    _name="";
    _filename="";
    _file_serial="";
}

Table::Table(const string &fname,vectorstr fieldlist):
    positions(true),indices(true){
    /**
       * @brief DEBUG
       * function: Table CTOR
       * purpose: Create a new file with given fieldlist and filename
       * Creates maps and multimaps
       * parameters: const string &fname,vectorstr fieldlist
       */

    //Total records starting at 0
    total_records=0;

    //Copy fieldlist into _fieldlist
    _fieldlist=fieldlist;

    //_name=filename
    _name=fname;

    //Name and Serial
    _file_serial=fname+"_"+to_string(serial);

    //Save filename and add extension .bin
    _filename=fname+"_"+to_string(serial)+".bin";

    //Create map of keys
    for(int i=0;i<fieldlist.size();i++){
        //Add an index number for each field
        positions[fieldlist[i]]+=i;

        //Add a multimap to the vector for each field
        indices.push_back(MMap<string,long>());
    }


    //Create fstream object
    fstream f;

    //Create/Open file
    open_fileW(f,_filename.c_str());

    //Create record
    Record rec(fieldlist);

    //Write fieldlists as record 0 into file
    rec.write(f);

    //Close file
    f.close();

}

Table::Table(const string &fname):positions(true),indices(true){
    /**
       * @brief DEBUG
       * function:Table CTOR
       * purpose:Open an existing file
       * Recreate map and vector of multimap objects
       * parameters:const string &fname
       */

    const bool DEBUG=false;
    //Save filename
    _name=fname;

    //Name and Serial
    _file_serial=fname+"_"+to_string(serial);

    //Always open original table
    _filename=fname+"_100"+".bin";

    //Total records starts at 0
    total_records=0;

    //Reindex structures and table
    reindex();

    if(DEBUG){
        cout<<"POSITIONS:"<<endl;
        cout<<positions<<endl;

        cout<<endl<<endl;
        cout<<"INDICES:"<<endl;
        cout<<indices<<endl;

    }

}

void Table::insert_into(const vectorstr& fieldvalues){
    /**
       * @brief DEBUG
       * function:insert into
       * purpose:Insert field values into database
       * Add each field value into its appropriate fields
       * parameters:vectorstr fieldvalues
       */
    const bool DEBUG=false;
    if(DEBUG){
        cout<<"Adding: "<<fieldvalues<<endl;
    }

    //If there are not enough field values for fields, insufficient fields
    if(fieldvalues.size()!=_fieldlist.size()){
        cout<<"INSUFFICIENT FIELDS"<<endl;
    }
    else{
        //Create record with field values
        Record r(fieldvalues);

        //Create fstream object
        fstream f;

        //Record number
        long recordno;

        //Open file for reading/writing
        open_fileRW(f,_filename.c_str());

        //Get its record number and write into the file
        recordno=r.write(f);

        //Close file
        f.close();

        //Loop through the entire field values
        for(int i=0;i<fieldvalues.size();i++){
            //Get the position of the field from map structure
            //Insert the fieldvalue
            //Then add the recno to it
            indices[positions[_fieldlist[i]]][fieldvalues[i]]+=recordno;
        }

        //Increment total records
        total_records++;
    }





}

void Table::delete_where(const vectorstr &conditions){
    /**
       * @brief DEBUG
       * function:delete where
       * purpose:Delete rows from table
       * Given conditions
       * parameters:const vectorstr &conditions
       */
    //Shunting Yard object
    Shunting_yard yard;

    //Change conditions to postfix
    Vector<string> postfix=yard.to_postfix(conditions);

    //Declare record object
    Record r;


    //FStream Object
    fstream f;

    //RPN evaluator
    RPN evaluator(postfix);

    //Hold all record numbers with conditions
    Vector<long> allrecords=evaluator.evaluate(positions,indices);

    //Default Value for Recno
    int recno=0;

    //Open File
    open_fileRW(f,_filename.c_str());

    for(int i=0;i<allrecords.size();i++){
        //Grab Record Number
        recno=allrecords[i];

        //Read record from file
        r.read(f,recno);

        //Set the field to Yes for deleted
        r.set_deleted();

        //Write it back  into the file
        r.write(f,recno);

        //Decrease total records
        total_records--;

    }

    //Close File
    f.close();




}

Table Table::select_all(){
    /**
       * @brief DEBUG
       * function:select all
       * purpose:Create another table containing records in this table
       * Then return that table
       * parameters:none
       */

    //Create record object
    Record r;

    //Create fstream object
    fstream f;

    //Record number starts at 0
    int recno=0;

    //Open file for reading/writing
    open_fileRW(f,_filename.c_str());

    //Read record 0 from file
    r.read(f,recno);

    //Increment serial
    serial++;

    //Create New Table with Fieldlists
    Table other(_name,r.get_record());

    //Move to first record, already have written fieldlists(record 0)
    recno++;
    r.read(f,recno);

    //While not the end of file, read and print each record
    while(!f.eof()){
        //Do not add anything that was marked deleted
        if(!r.is_deleted()){
            other.insert_into(r.get_record());
        }
        recno++;
        r.read(f,recno);
    }

    //Close file
    f.close();


    //Return table
    return other;

}

Table Table::select(const vectorstr& fields){
    /**
       * @brief DEBUG
       * function:Select
       * purpose: Filters Only Columns
       * Return a table containing all records with given columns/fields
       * parameters: vectorstr fields
       */
    const bool DEBUG=false;


    //If the field is a *, just select all
    if(fields[0]=="*"){
        return select_all();
    }
    else{
        //Make sure each fields provided is in the list of fields
        for(int i=0;i<fields.size();i++){
            //If the map doesn't have a field of that name
            //Return empty table with fieldlist
            if(!positions.contains(fields[i])){
                serial++;
                return Table (_name,_fieldlist);
            }
        }

        //Hold which columns to select
        Vector<int> keys;

        //Create New Table with new serial
        serial++;
        Table other(_name,fields);  

        //Grab the positions for each field
        for(int i=0;i<fields.size();i++){
            keys+=positions[fields[i]];
        }

        if(DEBUG){
            cout<<"KEYS:"<<endl;
            cout<<keys<<endl;

        }


        //Create record object
        Record r;

        //Create fstream object
        fstream f;

        //Record number starts at 0
        int recno=0;

        //Open file for reading/writing
        open_fileRW(f,_filename.c_str());


        //Read record 0 from file
        r.read(f,recno);

        //Move to first record, already have written fieldlist
        recno++;
        r.read(f,recno);

        //Loop to end of file
        while(!f.eof()){
            //Grab vector, pick the columns and insert it
            if(!r.is_deleted()){
                other.insert_into(r.get_record().pick(keys));

            }
            //Continue reading records
            recno++;
            r.read(f,recno);
        }

        //Close File
        f.close();

        //Return table
        return other;

    }


}

Table Table::select(const vectorstr& fields,const vectorstr& conditions){
    /**
       * @brief DEBUG
       * function:Select
       * purpose:Return a table given fields/conditions
       * Filter Columns and Rows
       * parameters:vectorstr fields,vectorstr conditions
       */

    //Shunting Yard
    Shunting_yard yard;

    //Turn Conditions into postfix
    Vector<string> postfix=yard.to_postfix(conditions);

    const bool DEBUG=false;

    //Create New Table with new serial
    serial++;

    Table other(_name,fields);

    //Create Record Object
    Record r;

    //FStream Object
    fstream f;

    //RPN
    RPN evaluator(postfix);

    //Evaluate the conditions and return records containing it
    Vector<long> allrecords=evaluator.evaluate(positions,indices);

    //Record number starts at 0
    int recno=0;


    if(DEBUG){
        cout<<"Conditions: "<<conditions<<endl;


        cout<<"All Records With that Conditions:"<<allrecords<<endl;

    }

    //If fields is *, just filter rows
    if(fields[0]=="*"){
        //Create Table
        Table other(_name,_fieldlist);

        //Open File for Reading/Writing
        open_fileRW(f,_filename.c_str());

        //Grab each record and insert it
        for(int i=0;i<allrecords.size();i++){
            //Grab record number from vector
            recno=allrecords[i];
            //Read it into the record
            r.read(f,recno);

            //Insert it into other
            if(!r.is_deleted()){
                other.insert_into(r.get_record());
            }



        }

        //Close File
        f.close();

        //Return Table
        return other;


    }
    //If fields was not *
    else{
        //Check if fields are valid
        for(int i=0;i<fields.size();i++){
            //If the map doesn't have a field of that name
            //Return empty table with fieldlist
            if(!positions.contains(fields[i])){
                serial++;
                return Table (_name,_fieldlist);
            }
        }

        //Create Table with given fields
        Table other(_name,fields);

        //Hold the positions of each field
        Vector<int> keys;

        //Add fields to keys/indices to know which ones to pick
        for(int i=0;i<fields.size();i++){
            keys+=positions[fields[i]];
        }

        //Open File for Reading/Writing
        open_fileRW(f,_filename.c_str());

        //Loop through each record with given condition
        for(int i=0;i<allrecords.size();i++){
            //Grab the record
            recno=allrecords[i];

            //Read it into record
            r.read(f,recno);

            //If not deleted, insert to new table
            if(!r.is_deleted()){
                //Grab record,pick columns,insert it
                other.insert_into(r.get_record().pick(keys));

            }
        }

        //Close File
        f.close();

        //Return Table
        return other;

    }



}


void Table::reindex(){
    /**
       * @brief DEBUG
       * function: Reindex
       * purpose: Reindex and setup structures
       * parameters: none
       */
    const bool DEBUG=false;

    //Create fstream object
    fstream f;

    //Create record object
    Record r;

    //Starting record number
    int recno=0;

    //To save each all records
    vectorstr fieldvalues;

    //Open file for reading/writing
    open_fileRW(f,_filename.c_str());

    //Read the fieldlist from record 0
    r.read(f,recno);

    //Save it into _fieldlist
    _fieldlist=r.get_record();

    //Create map of keys
    for(int i=0;i<_fieldlist.size();i++){
        //Add positions for each field in map
        positions[_fieldlist[i]]+=i;

        //Add a multimap to the vector for each field
        indices.push_back(MMap<string,long>());
    }

    if(DEBUG){
        cout<<"POSITIONS: "<<endl;
        cout<<positions<<endl;

    }

    //While you are not at the end of the file
    //Read records from file and store them in multimaps
    while(!f.eof()){

        recno++;

        //Read the record
        r.read(f,recno);

        if(!r.is_deleted()){
            //Save the vector
            fieldvalues=r.get_record();

            if(f.eof()){
                break;
            }

            //Save the record into each multimap
            for(int i=0;i<fieldvalues.size();i++){
                //Get the position from map structure
                //Insert the specific key using fieldvalues
                //Then add the recno to it
                indices[positions[_fieldlist[i]]][fieldvalues[i]]+=recno;
            }

            //Increment total records
            total_records++;

        }

    }
    if(DEBUG){
        cout<<"POSITIONS: "<<endl;
        cout<<positions<<endl;


        cout<<"INDICES:"<<endl;
        cout<<indices<<endl;

    }



    //Close file
    f.close();

}

void Table::print_table(ostream &outs) const{
    /**
       * @brief DEBUG
       * function:Print Table
       * purpose: Print the entire table
       * parameters: none
       */
    //Create record object
    Record r;

    //Create fstream object
    fstream f;

    //Record number starts at 0
    int recno=0;
    int recno_num=0;

    //Open file for reading/writing
    open_fileRW(f,_filename.c_str());

    //Read fields from file
    r.read(f,recno);

    //Printing Table Name
    outs<<"Table name:"<<_file_serial<<", records:"<<total_records<<endl;
    //Printing Fields
    outs<<setw(7)<<"record"<<r<<endl<<endl;
    recno++;
    r.read(f,recno);
    //While not the end of file, read and print each record
    while(!f.eof()){
        if(!r.is_deleted()){
            outs<<setw(5)<<"["<<recno_num<<"]"<<r<<endl;
            recno_num++;

        }
        recno++;
        r.read(f,recno);
    }

    //Close file
    f.close();

}

#endif // TABLE_H
