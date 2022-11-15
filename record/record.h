#ifndef RECORD_H
#define RECORD_H
#include <fstream>
#include <cstring>
#include <iomanip>
#include "../../../CS3A/!includes/vector_class_includes/vector.h"

bool file_exists(const char filename[]);

void open_fileRW(fstream& f, const char filename[]) throw(char*);

void open_fileW(fstream& f, const char filename[]);


typedef Vector<string> vectorstr;
class Record{
public:
    Record(){
        /**
           * @brief DEBUG
           * function:Record Default CTOR
           * purpose:Set each record[i][0]=NULL
           * recno=-1
           * _num_of_fields=0
           * parameters:none
           */
        //Set each record[i][0]=NULL
        for(int i=0;i<MAX_ROWS;i++){
            record[i][0]=NULL;
        }

        //Initialize Row 20 as the marker for being deleted
        string yes_or_no="NO";
        strcpy(record[MAX_ROWS_NO],yes_or_no.c_str());

        //Set default values
        recno=-1;
        _num_of_fields=0;
    }

    Record(const vectorstr& v){
        /**
           * @brief DEBUG
           * function:Record CTOR
           * purpose:copy all strings into
           * 2d array record
           * Set _num_of_fields=v.size();
           * parameters:const vectorstr& v
           */

        //Initialize each row to NULL
        for(int i=0;i<MAX_ROWS;i++){
            record[i][0]=NULL;
        }

        //Bring vector into record
        for(int i=0;i<v.size();i++){
            if(i==MAX_ROWS_NO){
                break;
            }
            strcpy(record[i],v[i].c_str());
        }

        //Initialize row 20 being marked as NO
        string yes_or_no="NO";
        strcpy(record[MAX_ROWS_NO],yes_or_no.c_str());


        _num_of_fields=v.size();
    }


    vectorstr get_record(){
        /**
           * @brief DEBUG
           * function:Get Record
           * purpose:Return a vectorstr of each string record[i]
           * parameters:none
           */
        vectorstr v;

        //Loop through all rows
        for(int i=0;i<MAX_ROWS_NO;i++){
            //Once you hit a NULL, break out
            //Don't push nulls
            if(record[i][0]==NULL){
                break;
            }
            //Push back a string of record[i] into vectorstr
            v.push_back(string(record[i]));
        }

        return v;

    }

    void update_num_of_fields(){
        /**
           * @brief DEBUG
           * function:Update num of fields
           * purpose:Update number of fields
           * So I can print the record using ostream operator
           * parameters:none
           */
        //Reset num of fields
        _num_of_fields=0;

        //Loop until you reach a NULL
        for(int i=0;i<MAX_ROWS_NO;i++){

            if(record[i][0]==NULL){
                break;
            }
            _num_of_fields++;

        }
    }

    bool is_deleted(){
        /**
           * @brief DEBUG
           * function:Is deleted
           * purpose:Return true if record is deleted
           * Return false if not deleted
           * parameters:none
           */

        //Get the string from Row 20
        string yes_or_no=record[MAX_ROWS_NO];

        //Check for NO or YES
        if(yes_or_no=="NO"){
           return false;
        }
        else{
            return true;
        }
    }

    void set_deleted(){
        /**
           * @brief DEBUG
           * function:set deleted
           * purpose:set row 20=YES
           * parameters:none
           */

        //Setting row 20 to YES
        string yes_or_no="YES";

        strcpy(record[MAX_ROWS_NO],yes_or_no.c_str());

    }

    friend ostream& operator<<(ostream& outs,
                               const Record& r){
        /**
           * @brief DEBUG
           * function:ostream object
           * purpose:Print each string in record
           * parameters:ostream& outs, const Record& r
           */
        //Print all of record
        for(int i=0;i<r._num_of_fields;i++){
            outs<<setw(WIDTH)<<r.record[i]<<"|";
        }

        return outs;

    }

    long write(fstream& outs);            //Write into file
    long write(fstream& outs,long recno); //Write to specific field
    long read(fstream& ins,long recno);   //Read from file into this

private:
    static const int MAX_ROWS=21;    //Row of 21
    static const int MAX_ROWS_NO=20;
    static const int MAX_COLS=50;    //Each row of 20 can hold 50 characters
    static const int WIDTH=15;
    int recno;                       //This record's number
    int _num_of_fields;              //num of fields of this record
    char record[MAX_ROWS][MAX_COLS]; //2D Array of Characters
};

long Record::write(fstream& outs){
    /**
       * @brief DEBUG
       * function:Write
       * purpose:Write this record into file
       * parameters:fstream& outs
       */

    //Seek next available position
    outs.seekg(0,outs.end);

    //Tell position in file
    long pos=outs.tellp();

    //Write into file
    outs.write(&record[0][0],sizeof(record));

    //Return recordnumber
    return pos/sizeof(record);
}

long Record::write(fstream& outs,long recno){
    long pos=recno*sizeof(record);

    //Seek
    outs.seekg(pos,ios_base::beg);

    pos=outs.tellp();

    //Write into file
    outs.write(&record[0][0],sizeof(record));

    return pos/sizeof(record);
}



long Record::read(fstream& ins,long recno){
    /**
       * @brief DEBUG
       * function:Read
       * purpose:Read from file given record number
       * Read it into this record object
       * parameters:fstream& ins,long recno
       */
    //Find position
    long pos=recno*sizeof(record);

    //Go to the position in file
    ins.seekg(pos,ios_base::beg);

    //Read it into this record
    ins.read(&record[0][0],sizeof(record));

    //Update number of fields after reading
    update_num_of_fields();

    //Return amount of characters read
    return ins.gcount();
}

bool file_exists(const char filename[]){
    /**
       * @brief DEBUG
       * function:File exists
       * purpose:Return true if file does exist
       * Return false if file doesn't exist
       * parameters:
       */
    const bool debug = false;

    //Create fstream object
    fstream ff;

    //Open it as a binary file
    ff.open (filename,
        std::fstream::in | std::fstream::binary );
    //If it failed print debug and return false
    if (ff.fail()){
        if (debug) cout<<"file_exists(): File does NOT exist: "<<filename<<endl;
        return false;
    }

    //If it didn't fail, print it exists
    if (debug) cout<<"file_exists(): File DOES exist: "<<filename<<endl;

    //Close file
    ff.close();

    //Return true
    return true;
}

void open_fileRW(fstream& f, const char filename[]) throw(char*){
    /**
       * @brief DEBUG
       * function:Open File for Read and Writing
       * purpose:Open file for reading and writing
       * Checks if it exists first
       * parameters:fstream& f, const char filename[]
       */
    const bool debug = false;

    //Check if file exists first
    if (!file_exists(filename)){

        //Open the file
        f.open(filename, std::fstream::out|std::fstream::binary);

        //Check if it failed or not
        if (f.fail()){
            cout<<"file open (RW) failed: with out|"<<filename<<"]"<<endl;
            throw("file RW failed  ");
        }
        else{
            if (debug) cout<<"open_fileRW: file created successfully: "<<filename<<endl;
        }
    }
    //If it does exist
    else{
        //Open the file for reading out and writing in
        f.open (filename,
            std::fstream::in | std::fstream::out| std::fstream::binary );
        if (f.fail()){
            cout<<"file open (RW) failed. ["<<filename<<"]"<<endl;
            throw("file failed to open.");
        }
    }

}

void open_fileW(fstream& f, const char filename[]){
    /**
       * @brief DEBUG
       * function:Open File Write
       * purpose:Creates a file
       * parameters:fstream& f, const char filename[]
       */

    //Open file/Create it
    f.open (filename,
            std::fstream::out| std::fstream::binary );

    //If it failed, throw error message
    if (f.fail()){
        cout<<"file open failed: "<<filename<<endl;
        throw("file failed to open.");
    }

}

#endif // RECORD_H
