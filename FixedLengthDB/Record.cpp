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

#pragma mark - Private Properties
private:
    static const int MAX_INT_LEN = 6;
    static const int MAX_INT_VAL = 999999;
    static const int MAX_STR_LEN = 70;
    
    int recordNumber;
    string institutionName, primaryKey;
    int satVerbal25th, satVerbal75th;
    int satMath25th, satMath75th;
    int numSATSubmitted, numEnrolled;
    
    //
    bool integerArgumentValid(int num) {
        return (num <= MAX_INT_VAL && num >= 0);
    }
    
    /* Private setters - useful for abstracting some of the error handling in the constructors */
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
    /* */
    
    /* Private getters - return formatted field data, ready for output */
    // fixme: make these work for arbitrary record lengths
    string getPrimaryKeyFormatted(int colSize) {
        string pk = primaryKey;
        pk.insert(pk.end(), colSize-pk.size(), ' ');
        return pk;
    }
    
    string getInstitutionNameFormatted(int colSize) {
        string inst = institutionName;
        inst.insert(inst.end(), colSize-inst.size(), ' ');
        return inst;
    }
    
    string getSATVerbal25thFormatted(int colSize) {
        string satVerb25 = to_string(satVerbal25th);
        satVerb25.insert(satVerb25.end(), colSize-satVerb25.size(), ' ');
        return satVerb25;
    }
    
    string getSATVerbal75thFormatted(int colSize) {
        string satVerb75 = to_string(satVerbal75th);
        satVerb75.insert(satVerb75.end(), colSize-satVerb75.size(), ' ');
        return satVerb75;
    }
    
    string getSATMath25thFormatted(int colSize) {
        string satMath25 = to_string(satMath25th);
        satMath25.insert(satMath25.end(), colSize-satMath25.size(), ' ');
        return satMath25;
    }
    
    string getSATMath75thFormatted(int colSize) {
        string satMath75 = to_string(satMath75th);
        satMath75.insert(satMath75.end(), colSize-satMath75.size(), ' ');
        return satMath75;
    }
    
    string getNumSATSubmittedFormatted(int colSize) {
        string satSub = to_string(numSATSubmitted);
        satSub.insert(satSub.end(), colSize-satSub.size(), ' ');
        return satSub;
    }
    
    string getNumStudentsEnrolledFormatted(int colSize) {
        string numEnrl = to_string(numEnrolled);
        numEnrl.insert(numEnrl.end(), colSize-numEnrl.size(), ' ');
        return numEnrl;
    }
    
    /* */
    
    //
    bool stringArgumentValid(string str) {
        return str.length() <= MAX_STR_LEN;
    }
  
    
#pragma mark - Public Properties
public:
    /* Constructors - many. */
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
    /* */
    
    // prints out a formatted record
    // ready for insertion into the database
    // fixme: make this work for arbitrary record lengths
    string dbRecord() {
        const int strSize = MAX_STR_LEN+1;
        const int numSize = MAX_INT_LEN+1;
        
        string pk = getPrimaryKeyFormatted(strSize);
        string satVerb25 = getSATVerbal25thFormatted(numSize);
        string satVerb75 = getSATVerbal75thFormatted(numSize);
        string satMath25 = getSATMath25thFormatted(numSize);
        string satMath75 = getSATMath75thFormatted(numSize);
        string satSub = getNumSATSubmittedFormatted(numSize);
        string numEnrl = getNumStudentsEnrolledFormatted(numSize);
        
        return pk+satVerb25+satVerb75+satMath25+satMath75+satSub+numEnrl+'\n';
    }
    
    // simply cout<<'s a record with spaces between values
    void display() {
        if(!isBlank()) {
            cout << institutionName << " " << satVerbal25th << " " << satVerbal75th << " " << satMath25th << " " << satMath75th << " " << numSATSubmitted << " " << numEnrolled << endl;
        }
    }
    
    // returns a formatted record
    // ready for display or entry into a report file
    // fixme: make this work for arbitrary record lengths
    // fixme: trim excess space in the institution name
    // fixme: make this match the size of the field titles in the report file
    string fileDisplay() {
        string inst = "a. " + getInstitutionNameFormatted(50);
        string satVerb25 = "b. " + getSATVerbal25thFormatted(MAX_INT_LEN+1);
        string satVerb75 = "c. " + getSATVerbal75thFormatted(MAX_INT_LEN+1);
        string satMath25 = "d. " + getSATMath25thFormatted(MAX_INT_LEN+1);
        string satMath75 = "e. " + getSATMath75thFormatted(MAX_INT_LEN+1);
        string satSub = "f. " + getNumSATSubmittedFormatted(MAX_INT_LEN+1);
        string numEnrl = "g. " + getNumStudentsEnrolledFormatted(MAX_INT_LEN+1);
        
        return inst+satVerb25+satVerb75+satMath25+satMath75+satSub+numEnrl+'\n';
    }
    
    /* Getters - pretty self-explanatory */
    string getInstitutionName() {
        return institutionName;
    }
    
    // returns the line number of the record in the database
    // records are searched before modification so this shouldn't be wrong.... shouldn't..
    int getRecordNumber() {
        return recordNumber;
    }
    /* */
    
    // returns true if a record's primary key (institution name) is blank
    bool isBlank() {
        return (institutionName.length() <= 0);
    }
    
    /* Setters - even more self-explanatory */
    // todo: leverage exceptions to notify the user if their value wasn't accepted
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
    /* */
};

// this was pretty much a complete waste of time
// i think i used one of these once
inline bool operator==(Record &left, Record &right) {
    return (left.getInstitutionName().compare(right.getInstitutionName()) == 0);
}
inline bool operator<(Record &left, Record &right) {
    return (left.getInstitutionName().compare(right.getInstitutionName()) < 0);
}
inline bool operator>(Record &left, Record &right) {
    return !(left < right);
}

//
inline bool operator==(Record &left, string &right) {
    return (left.getInstitutionName().compare(right) == 0);
}
inline bool operator<(Record &left, string &right) {
    return (left.getInstitutionName().compare(right) < 0);
}
inline bool operator>(Record &left, string &right) {
    return !(left < right);
}

//
inline bool operator==(string &left, Record &right) {
    return (right == left);
}
inline bool operator<(string &left, Record &right) {
    return (right < left);
}
inline bool operator>(string &left, Record &right) {
    return !(left < right);
}
