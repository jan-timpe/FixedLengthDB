//
//  Record.cpp
//  FixedLengthDB
//
//  Created by Jan Timpe on 1/22/17.
//  Copyright © 2017 Jan Timpe. All rights reserved.
//

#include <iostream>
#include <stdio.h>

using namespace std;

class Record {
public:
    Record() {}
    
    Record(string institution) {
        setInstitutionName(institution);
        setPrimaryKey(institution);
    }
    
    Record(int recordNum, string institution, int satVerb25, int satVerb75, int satMath25, int satMath75, int satSub, int numEnrl) {
        recordNumber = recordNum;
        
        setInstitutionName(institution);
        setPrimaryKey(institution);
        
        setSATVerbal25th(satVerb25);
        setSATVerbal75th(satVerb75);
        setSATMath25th(satMath25);
        setSATMath75th(satMath75);
        setNumSATSubmitted(satSub);
        setNumEnrolled(numEnrl);
    }
    
    void display() {
        if(!isBlank()) {
            cout << institutionName << " " << satVerbal25th << " " << satVerbal75th << " " << satMath25th << " " << satMath75th << " " << numSATSubmitted << " " << numEnrolled << endl;
        }
    }
    
    bool isBlank() {
        return (institutionName.length() <= 0);
    }
    
    string getInstitutionName() {
        return institutionName;
    }
    
    int getRecordNumber() {
        return recordNumber;
    }
    
    void setSATVerbal25th(int scr) {
        if(integerArgumentValid(scr)) {
            satVerbal25th = scr;
        }
    }
    
    void setSATVerbal75th(int scr) {
        if(integerArgumentValid(scr)) {
            satVerbal75th = scr;
        }
    }
    
    void setSATMath25th(int scr) {
        if(integerArgumentValid(scr)) {
            satMath25th = scr;
        }
    }
    
    void setSATMath75th(int scr) {
        if(integerArgumentValid(scr)) {
            satMath75th = scr;
        }
    }
    
    void setNumSATSubmitted(int num) {
        if(integerArgumentValid(num)) {
            numSATSubmitted = num;
        }
    }
    
    void setNumEnrolled(int num) {
        if(integerArgumentValid(num)) {
            numEnrolled = num;
        }
    }
    
    string dbRecord() {
        int stringSize = MAX_STR_LEN+1;
        int numSize = MAX_INT_LEN+1;
        
        string pk = primaryKey;
        pk.insert(pk.end(), stringSize-pk.size(), ' ');
        
        string satVerb25 = to_string(satVerbal25th);
        satVerb25.insert(satVerb25.end(), numSize-satVerb25.size(), ' ');
        
        string satVerb75 = to_string(satVerbal75th);
        satVerb75.insert(satVerb75.end(), numSize-satVerb75.size(), ' ');
        
        string satMath25 = to_string(satMath25th);
        satMath25.insert(satMath25.end(), numSize-satMath25.size(), ' ');
        
        string satMath75 = to_string(satMath75th);
        satMath75.insert(satMath75.end(), numSize-satMath75.size(), ' ');
        
        string satSub = to_string(numSATSubmitted);
        satSub.insert(satSub.end(), numSize-satSub.size(), ' ');
        
        string numEnrl = to_string(numEnrolled);
        numEnrl.insert(numEnrl.end(), numSize-numEnrl.size(), ' ');
        
        return pk+satVerb25+satVerb75+satMath25+satMath75+satSub+numEnrl+'\n';
    }
    
private:
    static const int MAX_INT_LEN = 6;
    static const int MAX_INT_VAL = 999999;
    static const int MAX_STR_LEN = 70;
    
    int recordNumber;
    string institutionName, primaryKey;
    int satVerbal25th, satVerbal75th;
    int satMath25th, satMath75th;
    int numSATSubmitted, numEnrolled;
    
    void setInstitutionName(string institution) {
        if(stringArgumentValid(institution)) {
            replace(institution.begin(), institution.end(), '_', ' ');
            institutionName = institution;
        }
    }
    
    void setPrimaryKey(string institution) {
        if(stringArgumentValid(institution)) {
            replace(institution.begin(), institution.end(), ' ', '_');
            primaryKey = institution;
        }
    }
    
    bool integerArgumentValid(int num) {
        return (num <= MAX_INT_VAL && num >= 0);
    }
    
    bool stringArgumentValid(string str) {
        return str.length() <= MAX_STR_LEN;
    }
};
