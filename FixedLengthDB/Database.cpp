//
//  Database.cpp
//  FixedLengthDB
//
//  Created by Jan Timpe on 1/22/17.
//  Copyright © 2017 Jan Timpe. All rights reserved.
//
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <sstream>
#include "Record.cpp"

using namespace std;

class Database {
    
#pragma mark - Private Properties
private:
    string dbName, dbFileName, configFileName;
    int numRecords, recordSize;
    vector<string> fields;
    bool isOpen;
    
    // thrown when the database is not open
    // useful for instantiated Database() objects without valid file names
    struct DatabaseNotOpenException: public exception {
        const char * what() const throw() {
            return "Database not open";
        }
    };
    
    // search through an open database file
    bool binarySearch(fstream &db, string pk, Record &rec) {
        int low = 0;
        int high = numRecords-1;
        int mid;
        
        // recursive?
        while(high >= low) {
            mid = (low+high)/2;
            rec = getRecord(db, mid, true);
            
            if(rec == pk) {
                return true;
            }
            else if(rec < pk) {
                low = mid+1;
            }
            else {
                high = mid-1;
            }
        }
        
        return false;
    }
    
    // returns an empty record (all spaces) representing a blank line in the database
    // includes endline character
    string emptyRecord() {
        string empty = " ";
        empty.insert(empty.end(), (recordSize-1)-empty.size(), ' ');
        empty.push_back('\n');
        
        return empty;
    }
    
    // helper function
    // assigns a variable to the contents of a config param based on line number
    void getConfigParam(int lineNumber, string lineText) {
        if(lineNumber == 1) {
            numRecords = atoi(lineText.c_str());
        }
        else if(lineNumber == 2) {
            recordSize = atoi(lineText.c_str());
        }
        else {
            fields.push_back(lineText);
        }
    }
    
    // returns the structured record at lineNum
    // if ignoreBlanks is set to true, no empty records will be returned (the next non-empty record is used) - lineNum is not updated in this case (performance hit, line could be re-searched)
    // fixme: find a way to update lineNum to make this more efficient
    // throws out_of_range if lineNum is not within the number of lines in the file
    Record getRecord(fstream &db, const int lineNum, bool ignoreBlanks) {
        if(lineNum < 0 || lineNum > numRecords) {
            throw out_of_range("Requested index out of range");
        }
        
        db.seekg(lineNum*recordSize, ios::beg);
        
        string line;
        getline(db, line);
        
        // if we hit a blank line on search, check the next record, since we don't know what to do
        // this is a performance hit (on two sides since we can't update lineNum)
        if(ignoreBlanks && lineIsEmpty(line)) {
            return getRecord(db, lineNum+1, ignoreBlanks);
        }
        
        return recordFromFileLine(line, lineNum);
    }
    
    // returns true if a line from the database file is empty (university-name/pk not set)
    bool lineIsEmpty(string line) {
        string univName;
        int satVerb25, satVerb75, satMath25, satMath75, satSub, numEnrl;
        
        stringstream linestream = stringstream(line);
        linestream >> univName >> satVerb25 >> satVerb75 >> satMath25 >> satMath75 >> satSub >> numEnrl;
        
        return (univName.length() == 0);
    }
    
    // export new database settings/info
    void writeConfig() {
        ofstream config(configFileName);
        
        config << numRecords << endl;
        config << recordSize << endl;
        
        for(string field: fields) {
            config << field << endl;
        }
        config.close();
    }
    
    // import database settings/info
    void readConfig() {
        ifstream config(configFileName);
        isOpen = false;
        
        if(config.is_open()) {
            int lineNumber = 1;
            string line;
            fields.clear();
            
            while(getline(config, line)) {
                getConfigParam(lineNumber, line);
                lineNumber++;
            }
            isOpen = true;
        }
        
        config.close();
    }
    
    // returns a structured record given by a line in the database file
    Record recordFromFileLine(string line, int lineNum) {
        string univName;
        int satVerb25, satVerb75, satMath25, satMath75, satSub, numEnrl;
        
        stringstream linestream = stringstream(line);
        linestream >> univName >> satVerb25 >> satVerb75 >> satMath25 >> satMath75 >> satSub >> numEnrl;
        
        return Record(lineNum, univName, satVerb25, satVerb75, satMath25, satMath75, satSub, numEnrl);
    }
    
    // rebuilds the database file by inserting blank lines between each populated (non-empty) record
    void rewriteFile(fstream &db) {
        ofstream newFile("tmp.db");
        
        int readLine = 0, writeLine = 0;
        while(db.seekg(readLine*recordSize, ios::beg)) {
            string line;
            getline(db, line);
            line.push_back('\n');
            
            if(!lineIsEmpty(line)) {
                newFile.seekp(writeLine*recordSize, ios::beg);
                newFile << line;
                newFile << emptyRecord();
                writeLine += 2;
            }
            readLine++;
        }
        
        newFile.close();
        remove(dbFileName.c_str());
        rename("tmp.db", dbFileName.c_str());
    }
    
    // rebuilds the database file while inserting a new record at insertLine
    void rewriteWithInsert(fstream &db, Record ins, int insertLine) {
        ofstream newfile("tmp.db");
        
        int readLine = 0, writeLine = 0;
        while(db.seekg(readLine*recordSize, ios::beg)) {
            string line;
            getline(db, line);
            line.push_back('\n');
            
            if(readLine == insertLine) {
                newfile << ins.dbRecord();
                newfile << emptyRecord();
                writeLine += 2;
            }
            
            if(!lineIsEmpty(line)) {
                newfile << line;
                newfile << emptyRecord();
                writeLine += 2;
            }
            
            readLine++;
        }
        
        newfile.close();
        remove(dbFileName.c_str());
        rename("tmp.db", dbFileName.c_str());
        numRecords = writeLine;
    }
    
    // make sure the database can be opened
    void validateFileOperations() {
        if(!databaseIsOpen()) {
            throw DatabaseNotOpenException();
        }
    }
    
    
    

#pragma mark - Public Properties
public:
    
    // public constuctor (overrides default)
    Database() {
        dbName = "";
        dbFileName = "";
        configFileName = "";
        isOpen = false;
    }
    
    // public constructor
    // validates database name and reads configuration parameters
    Database(string name) {
        string fileName = name+".db";
        string configName = name+".config";
        
        if(fileExists(fileName) && fileExists(configName)) {
            setDatabaseFileNames(name);
            isOpen = false; // database is not open until we read the config file
            readConfig();
        }
        else {
            throw invalid_argument(fileName+" does not exist");
        }
    }
    
    // adds a new record in the database
    // adds into the blank line between records
    // if no blank line available, triggers a file rewrite
    void addRecord(Record rec) {
        validateFileOperations();
        
        fstream db(dbFileName);
        
        Record insertionPoint = Record();
        bool found = binarySearch(db, rec.getInstitutionName(), insertionPoint);
        
        if(found) {
            throw invalid_argument("Record already exists");
        }
        
        int lineNum = insertionPoint.getRecordNumber();
        if(!insertionPoint.isBlank()) {
            
            // there _shouldn't_ be an == case...
            if(rec < insertionPoint) {
                lineNum--;
            }
            else {
                lineNum++;
            }
            
            insertionPoint = getRecord(db, lineNum, false);
            
            // not guaranteed to be a blank line. if the new point is occupied, we need to rewrite the whole file (overflow)
            if(!insertionPoint.isBlank()) {
                rewriteWithInsert(db, rec, insertionPoint.getRecordNumber());
            }
        }
        
        db.seekp(lineNum*recordSize, ios::beg);
        db << rec.dbRecord();
        db.close();
    }
    
    // closes the database
    // rewrites the config file and blocks access for future calls
    void closeDatabase() {
        writeConfig();
        isOpen = false;
    }
    
    // deletes the record (previously found via search) in the database
    void deleteRecord(Record rec) {
        validateFileOperations();
        
        int lineNum = rec.getRecordNumber();
        
        fstream db(dbFileName);
        db.seekp(lineNum*recordSize, ios::beg);
        db << emptyRecord();
        db.close();
    }
    
    // returns true if the database is/can-be opened
    bool databaseIsOpen() {
        return isOpen;
    }
    
    // returns true if the specified file already exists
    static bool fileExists(string fileName) {
        ifstream file(fileName.c_str());
        return file.good();
    }
    
    // returns the record matching universityName in the database
    // throws an invalid argument if the record is not found
    Record findRecord(string universityName) {
        validateFileOperations();
        
        fstream db(dbFileName);
        Record rec = Record();
        bool found = binarySearch(db, universityName, rec);
        db.close();
        
        if(!found) {
            throw invalid_argument(universityName+" not found");
        }
        
        return rec;
    }
    
    // returns the database name
    string getDatabaseName() {
        return dbName;
    }
    
    // prints fields (from config file) to the screen
    void printFields() {
        for(string field: fields) {
            cout << field << "\t";
        }
        cout << endl;
    }
    
    
    // fixme: not sure this even works
    void printReport() {
        validateFileOperations();
        
        printFields();
        cout << endl;
        
        fstream db(dbFileName);
        
        int records = 0;
        
        for(int line = 0; (line < numRecords && records < 10); line++) {
            Record rec = getRecord(db, line, true);
            
            if(!rec.isBlank()) {
                records++;
                cout << records << ". ";
                rec.display();
            }
        }
        
        db.close();
        
        cout << endl;
    }
    
    // sets the database file names (have already been validated)
    // called from constructor
    void setDatabaseFileNames(string name) {
        dbName = name;
        dbFileName = name+".db";
        configFileName = name+".config";
    }
    
    // updates the record (previously found via search) in the database
    void updateRecord(Record rec) {
        validateFileOperations();
        
        int lineNum = rec.getRecordNumber();
        
        fstream db(dbFileName);
        db.seekp(lineNum*recordSize, ios::beg);
        db << rec.dbRecord();
        db.close();
    }
    
};
