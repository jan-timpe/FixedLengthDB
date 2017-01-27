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
    
    // simply cout<<'s a record with spaces between values
    void display() {
        if(!isBlank()) {
            cout << institutionName << " " << satVerbal25th << " " << satVerbal75th << " " << satMath25th << " " << satMath75th << " " << numSATSubmitted << " " << numEnrolled << endl;
        }
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
