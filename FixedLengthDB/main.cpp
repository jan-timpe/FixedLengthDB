//
//  main.cpp
//  FixedLengthDB
//
//  Created by Jan Timpe on 1/22/17.
//  Copyright © 2017 Jan Timpe. All rights reserved.
//

#include <iostream>
#include "Database.cpp"

using namespace std;

//
void displayMenu() {
    cout << "\n\nMenu\n==================" << endl;
    cout << "1. Open a database" << endl;
    cout << "2. Display a record" << endl;
    cout << "3. Update a record" << endl;
    cout << "4. Create report" << endl;
    cout << "5. Add a record" << endl;
    cout << "6. Delete a record" << endl;
    cout << "7. Quit" << endl;
    cout << "==================" << endl;
}

// requests a menu options (1-7) corresponding to values in displayMenu() (above)
int getUserMenuSelection() {
    int option = 0;
    cin.clear();
    cout << "Enter an option number: ";
    cin >> option;
    
    while(option < 1 || option > 7) {
        cout << "Enter a valid option number: ";
        cin >> option;
    }
    
    return option;
}

// OPTION 1
void openDatabase(Database &db) {
    string dbName;
    cin.ignore();
    cout << "Enter the database name (no extension): ";
    getline(cin, dbName);
    
    try {
        db = Database(dbName);
        cout << "Database '" << db.getDatabaseName() << "' successfully opened." << endl;
    }
    catch(exception &e) {
        cout << "ERROR: " << e.what() << endl;
    }
}

// OPTION 2
void displayRecord(Database &db) {
    string universityName;
    cin.ignore();
    cout << "Enter the university name to search for: ";
    getline(cin, universityName);
    
    try {
        Record rec = db.findRecord(universityName);
        cout << "Found: ";
        rec.display();
    }
    catch(exception &e) {
        cout << "ERROR: " << e.what() << endl;
    }
}

// OPTION 3
// todo: allow updating of only a specific field
void updateRecord(Database &db) {
    string universityName;
    cin.ignore();
    cout << "Enter the university name to update: ";
    getline(cin, universityName);
    
    try {
        Record rec = db.findRecord(universityName);
        
        int verb25 = 0;
        cout << "Enter a new SAT Verbal 25th Percentile value: ";
        cin >> verb25;
        rec.setSATVerbal25th(verb25);
        
        int verb75 = 0;
        cout << "Enter a new SAT Verbal 75th Percentile value: ";
        cin >> verb75;
        rec.setSATVerbal75th(verb75);
        
        int math25 = 0;
        cout << "Enter a new SAT Math 25th Percentile value: ";
        cin >> math25;
        rec.setSATMath25th(math25);
        
        int math75 = 0;
        cout << "Enter a new SAT Math 75th Percentile value: ";
        cin >> math75;
        rec.setSATMath75th(math75);
        
        int numSAT = 0;
        cout << "Enter a new Number of SATs Submitted value: ";
        cin >> numSAT;
        rec.setNumSATSubmitted(numSAT);
        
        int numEnrl = 0;
        cout << "Enter a new Number of Students Enrolled value: ";
        cin >> numEnrl;
        rec.setNumEnrolled(numEnrl);
        
        db.updateRecord(rec);
    }
    catch(exception &e) {
        cout << "ERROR: " << e.what() << endl;
    }
}

// OPTION 4
void createReport(Database &db) {
    try {
        db.buildReport();
    }
    catch(exception &e) {
        cout << "ERROR: " << e.what() << endl;
    }
}

// OPTION 5
void addRecord(Database &db) {
    string universityName;
    cin.ignore();
    cout << "Enter the university name to add: ";
    getline(cin, universityName);
    
    Record rec = Record(universityName);
    
    int verb25 = 0;
    cout << "Enter an SAT Verbal 25th Percentile value: ";
    cin >> verb25;
    rec.setSATVerbal25th(verb25);
    
    int verb75 = 0;
    cout << "Enter an SAT Verbal 75th Percentile value: ";
    cin >> verb75;
    rec.setSATVerbal75th(verb75);
    
    int math25 = 0;
    cout << "Enter an SAT Math 25th Percentile value: ";
    cin >> math25;
    rec.setSATMath25th(math25);
    
    int math75 = 0;
    cout << "Enter an SAT Math 75th Percentile value: ";
    cin >> math75;
    rec.setSATMath75th(math75);
    
    int numSAT = 0;
    cout << "Enter a Number of SATs Submitted value: ";
    cin >> numSAT;
    rec.setNumSATSubmitted(numSAT);
    
    int numEnrl = 0;
    cout << "Enter a Number of Students Enrolled value: ";
    cin >> numEnrl;
    rec.setNumEnrolled(numEnrl);
    
    try {
        db.addRecord(rec);
    }
    catch(exception &e) {
        cout << "ERROR: " << e.what() << endl;
    }
}

// OPTION 6
void deleteRecord(Database &db) {
    string universityName;
    cin.ignore();
    cout << "Enter the name of the university you'd like to delete: ";
    getline(cin, universityName);
    
    try {
        Record rec = db.findRecord(universityName);
        db.deleteRecord(rec);
    }
    catch(exception &e) {
        cout << "ERROR: " << e.what() << endl;
    }
}

// OPTION 7
void closeAndQuit(Database &db) {
    char q;
    cin.ignore();
    cout << "Are you sure you want to quit [y/n]? ";
    cin >> q;
    
    if(q == 'y' || q == 'Y') {
        db.closeDatabase();
        cout << "Bye." << endl;
        exit(9);
    }
}

// given a menu option, performs the appropriate function
void performMenuAction(int menuOption, Database &db) {
    switch(menuOption) {
        case 1:
            openDatabase(db);
            break;
            
        case 2:
            displayRecord(db);
            break;
            
        case 3:
            updateRecord(db);
            break;
            
        case 4:
            createReport(db);
            break;
            
        case 5:
            addRecord(db);
            break;
            
        case 6:
            deleteRecord(db);
            break;
            
        case 7:
            closeAndQuit(db);
            
        default:
            break;
    }
}

// displays menu and performs action based on selected option
void nextAction(Database &db) {
    displayMenu();
    int menuOption = getUserMenuSelection();
    performMenuAction(menuOption, db);
}

//
int main(int argc, const char * argv[]) {
    cout << "Welcome to the world's worst database." << endl; // name is a work in progress
    
    // instantiate an empty database
    // (effectively a singleton for this application, although more instances can be created)
    Database db = Database();
    
    // go on and on and on (until i say stop)
    char terminate = 'n';
    while(terminate != 'y' && terminate != 'Y') {
        nextAction(db);
    }
    
    return 0;
}
