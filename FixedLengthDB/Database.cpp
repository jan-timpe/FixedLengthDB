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
public:
    static bool fileExists(string fileName) {
        ifstream file(fileName.c_str());
        return file.good();
    }
    
    bool databaseIsOpen() {
        return isOpen;
    }
    
    Database() {
        dbName = "";
        dbFileName = "";
        configFileName = "";
        isOpen = false;
    }
    
    Database(string name) {
        string fileName = name+".db";
        string configName = name+".config";
        
        if(fileExists(fileName) && fileExists(configName)) {
            dbName = name;
            dbFileName = fileName;
            configFileName = name+".config";
            readConfig();
            isOpen = true;
        }
        else {
            throw invalid_argument(fileName+" does not exist");
        }
    }
    
    string getDatabaseName() {
        return dbName;
    }
    
    void printFields() {
        for(string field: fields) {
            cout << field << "\t";
        }
        cout << endl;
    }
    
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
    
    void updateRecord(Record rec) {
        validateFileOperations();
        
        fstream db(dbFileName);
        
        int lineNum = rec.getRecordNumber();
        db.seekp(lineNum*recordSize, ios::beg);
        
        db << rec.dbRecord();
        
        db.close();
    }
    
    void deleteRecord(Record rec) {
        validateFileOperations();
        
        fstream db(dbFileName);
        
        int lineNum = rec.getRecordNumber();
        db.seekp(lineNum*recordSize, ios::beg);
        
        db << emptyRecord();
        db.close();
    }
    
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
            int res = rec.getInstitutionName().compare(insertionPoint.getInstitutionName());
            
            if(res == 0) {
                // record exists?
                throw invalid_argument("Record already exists");
            }
            else if(res  < 0) {
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
    
    void justDoARewrite() {
        fstream db(dbFileName);
        rewriteFile(db);
        db.close();
    }
    
    void closeDatabase() {
        writeConfig();
        isOpen = false;
    }
    
private:
    string dbName, dbFileName, configFileName;
    int numRecords, recordSize;
    vector<string> fields;
    bool isOpen;
    
    struct DatabaseNotOpenException: public exception {
        const char * what() const throw() {
            return "Database not open";
        }
    };
    
    void validateFileOperations() {
        if(!databaseIsOpen()) {
            throw DatabaseNotOpenException();
        }
    }
    
    void writeConfig() {
        ofstream config(configFileName);
        config << numRecords << endl;
        config << recordSize << endl;
        for(string field: fields) {
            config << field << endl;
        }
        config.close();
    }
    
    void readConfig() {
        ifstream config(configFileName);
        isOpen = false;
        
        if(config.is_open()) {
            int lineNumber = 1;
            string line;
            
            while(getline(config, line)) {
                getConfigParam(lineNumber, line);
                lineNumber++;
            }
            isOpen = true;
        }
        
        config.close();
    }
    
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
    
    bool binarySearch(fstream &db, string pk, Record &rec) {
        int low = 0;
        int high = numRecords-1;
        int mid;
        
        // recursive?
        while(high >= low) {
            mid = (low+high)/2;
            rec = getRecord(db, mid, true);
            
            int res = rec.getInstitutionName().compare(pk);
            if(rec.getInstitutionName().compare(pk) == 0) {
                return true;
            }
            else if(res < 0) {
                low = mid+1;
            }
            else {
                high = mid-1;
            }
        }
        
        return false;
    }
    
    Record getRecord(fstream &db, const int lineNum, bool ignoreBlanks) {
        cout << "Fetching record" << endl;
        if(lineNum < 0 || lineNum > numRecords) {
            throw out_of_range("Requested index out of range");
        }
        
        string univName;
        int satVerb25, satVerb75, satMath25, satMath75, satSub, numEnrl;
        
        db.seekg(lineNum*recordSize, ios::beg);
        
        string line;
        getline(db, line);

        stringstream linestream = stringstream(line);
        linestream >> univName >> satVerb25 >> satVerb75 >> satMath25 >> satMath75 >> satSub >> numEnrl;
        
        // if we hit a blank line on search, check the next record, since we don't know what to do
        // this is a performance hit (on two sides since we can't update lineNum)
        if(ignoreBlanks && univName.length() == 0) {
            return getRecord(db, lineNum+1, ignoreBlanks);
        }
        
        return Record(lineNum, univName, satVerb25, satVerb75, satMath25, satMath75, satSub, numEnrl);
    }
    
    string emptyRecord() {
        string empty = " ";
        empty.insert(empty.end(), (recordSize-1)-empty.size(), ' ');
        empty.push_back('\n');
        
        return empty;
    }
    
    bool lineIsEmpty(string line) {
        string univName;
        int satVerb25, satVerb75, satMath25, satMath75, satSub, numEnrl;
        
        stringstream linestream = stringstream(line);
        linestream >> univName >> satVerb25 >> satVerb75 >> satMath25 >> satMath75 >> satSub >> numEnrl;
        
        return (univName.length() == 0);
    }
    
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
        numRecords = writeLine+1;
    }
    
};
