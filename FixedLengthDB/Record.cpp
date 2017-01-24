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
    
    Record(string univName) {
        institutionName = univName;
        replace(univName.begin(), univName.end(), '_', ' ');
        primaryKey = univName;
    }
    
    Record(int recordNum, string institution, int satVerb25, int satVerb75, int satMath25, int satMath75, int satSub, int numEnrl) {
        recordNumber = recordNum;
        primaryKey = institution;
        replace(institution.begin(), institution.end(), '_', ' ');
        institutionName = institution;
        satVerbal25th = satVerb25;
        satVerbal75th = satVerb75;
        satMath25th = satMath25;
        satMath75th = satMath75;
        numSATSubmitted = satSub;
        numEnrolled = numEnrl;
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
        satVerbal25th = scr;
    }
    
    void setSATVerbal75th(int scr) {
        satVerbal75th = scr;
    }
    
    void setSATMath25th(int scr) {
        satMath25th = scr;
    }
    
    void setSATMath75th(int scr) {
        satMath75th = scr;
    }
    
    void setNumSATSubmitted(int num) {
        numSATSubmitted = num;
    }
    
    void setNumEnrolled(int num) {
        numEnrolled = num;
    }
    
    string dbRecord() {
        string pk = primaryKey;
        pk.insert(pk.end(), 70-pk.size(), ' ');
        
        string satVerb25 = to_string(satVerbal25th);
        satVerb25.insert(satVerb25.end(), 6-satVerb25.size(), ' ');
        
        string satVerb75 = to_string(satVerbal75th);
        satVerb75.insert(satVerb75.end(), 6-satVerb75.size(), ' ');
        
        string satMath25 = to_string(satMath25th);
        satMath25.insert(satMath25.end(), 6-satMath25.size(), ' ');
        
        string satMath75 = to_string(satMath75th);
        satMath75.insert(satMath75.end(), 6-satMath75.size(), ' ');
        
        string satSub = to_string(numSATSubmitted);
        satSub.insert(satSub.end(), 6-satSub.size(), ' ');
        
        string numEnrl = to_string(numEnrolled);
        numEnrl.insert(numEnrl.end(), 6-numEnrl.size(), ' ');
        
        return pk+satVerb25+satVerb75+satMath25+satMath75+satSub+numEnrl+'\n';
    }
    
private:
    int recordNumber;
    string institutionName, primaryKey;
    int satVerbal25th, satVerbal75th;
    int satMath25th, satMath75th;
    int numSATSubmitted, numEnrolled;
};
