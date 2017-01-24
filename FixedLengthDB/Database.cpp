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
        
        ifstream db(dbFileName.c_str());
        
        Record rec = Record(0, "", 0, 0, 0, 0, 0, 0);
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
        ifstream db(dbFileName.c_str());
        
        for(int line = 0; (line < numRecords && records < 10); line++) {
            Record rec = getRecord(db, line);
            
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
        
        cout << "Editing line " << rec.getRecordNumber() << endl;
        
        fstream db(dbFileName);
        
        int lineNum = rec.getRecordNumber();
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
    
    bool binarySearch(ifstream &db, string pk, Record &rec) {
        int low = 0;
        int high = numRecords-1;
        int mid;
        
        while(high >= low) {
            mid = (low+high)/2;
            rec = getRecord(db, mid);
            
            // debug
//            cout << rec.getRecordNumber() << " * ";
//            rec.display();
            
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
    
    Record getRecord(ifstream &db, const int lineNum) const {
        if(lineNum < 0 || lineNum > numRecords) {
            throw invalid_argument("Requested index out of range");
        }
        
        string univName;
        int satVerb25, satVerb75, satMath25, satMath75, satSub, numEnrl;
        
        db.seekg(lineNum*recordSize, ios::beg);
        db >> univName >> satVerb25 >> satVerb75 >> satMath25 >> satMath75 >> satSub >> numEnrl;
        
        return Record(lineNum, univName, satVerb25, satVerb75, satMath25, satMath75, satSub, numEnrl);
    }
    
};
