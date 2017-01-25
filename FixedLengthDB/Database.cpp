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
        if(!isOpen) {
            throw invalid_argument("Database not open");
        }
        
        fstream db(dbFileName.c_str());
        
        Record rec = Record();
        bool found = binarySearch(db, universityName, rec);
        db.close();
        
        if(!found) {
            throw invalid_argument(universityName+" not found");
        }
        
        return rec;
    }
    
    void printReport() {
        if(!isOpen) {
            throw invalid_argument("Database not open");
        }
        
        printFields();
        cout << endl;
        
        int records = 0;
        fstream db(dbFileName.c_str());
        
        for(int line = 0; (line < numRecords && records < 10); line++) {
            Record rec = getRecord(db, line, true);
            
            if(!rec.isBlank()) {
                records++;
                cout << records << ". ";
                rec.display();
            }
        }
        
        cout << endl;
        
        db.close();
    }
    
    void updateRecord(Record rec) {
        if(!isOpen) {
            throw invalid_argument("Database not open");
        }
        
        fstream db(dbFileName);
        
        int lineNum = rec.getRecordNumber();
        db.seekp(lineNum*recordSize, ios::beg);
        
        db << rec.dbRecord();
        
        db.close();
    }
    
    void deleteRecord(Record rec) {
        if(!isOpen) {
            throw invalid_argument("Database not open");
        }
        
        fstream db(dbFileName);
        
        int lineNum = rec.getRecordNumber();
        db.seekp(lineNum*recordSize, ios::beg);
        
        db << emptyRecord();
        
        db.close();
    }
    
    void addRecord(Record rec) {
        cout << "got here" << endl;
        if(!isOpen) {
            throw invalid_argument("Database not open");
        }
        
        fstream db(dbFileName);
        Record insertionPoint = Record();
        bool found = binarySearch(db, rec.getInstitutionName(), insertionPoint);
        
        cout << "got a record" << endl;
        
        if(found) {
            throw invalid_argument("Record already exists");
        }
        
        int lineNum = insertionPoint.getRecordNumber();
        if(!insertionPoint.isBlank()) {
            int res = rec.getInstitutionName().compare(insertionPoint.getInstitutionName());
            
            if(res == 0) {
                // record exists?
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
                // we're fuuuuuucked
                return;
            }
        }
        
        // int lineNum = insertionPoint.getRecordNumber();
        db.seekp(lineNum*recordSize, ios::beg);
        db << rec.dbRecord();
        
        db.close();
    }
    
private:
    string dbName, dbFileName, configFileName;
    int numRecords, recordSize;
    vector<string> fields;
    bool isOpen;
    
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
        if(lineNum < 0 || lineNum > numRecords) {
            throw invalid_argument("Requested index out of range");
        }
        
        string univName;
        int satVerb25, satVerb75, satMath25, satMath75, satSub, numEnrl;
        
        db.seekg(lineNum*recordSize, ios::beg);
        
        string line;
        getline(db, line);

        stringstream linestream = stringstream(line);
        linestream >> univName >> satVerb25 >> satVerb75 >> satMath25 >> satMath75 >> satSub >> numEnrl;
        
        // if we hit a blank line on search, check the next record, since we don't know what to do
        // this is a performance hit.
        // ERROR: this makes it impossible to hit a "not found"
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
    
};
