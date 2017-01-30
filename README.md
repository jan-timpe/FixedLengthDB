# FixedLengthDB
An Database Management Systems exercise involving a file-based database of fixed-length records.

## Running

Download/clone the repository, navigate to the root directory.

If you're not using Xcode to run the code, navigate to the source directory
```
$ cd FixedLengthDB
```

Run the code with the following command

```
$ g++ -std=c++0x -o fixedlength.exe main.cpp -w -Wfatal-errors
```

The command suppresses warnings and other non-fatal messages, and compiles with C++11 support.

## Generate a clean Database file
Navigate to the source directory
```
$ cd FixedLengthDB
```

Run SetupDB.py
```
python SetupDB.py
```

The cleaned file is generated as ResetDatabase.db
