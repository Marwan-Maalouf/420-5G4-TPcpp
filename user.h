#ifndef USER_H
#define USER_H

#include <string>
#include <vector>

using namespace std;

class User {
private:
    string name;
    string userId;
    vector<string> borrowedBooks; // Store ISBNs of borrowed books
    static int nextUserId; // Static member to generate unique user IDs

public:
    // Constructors
    User();
    User(const string& name, const string& userId);
    User(const string& name);
    
    // Getters
    string getName() const;
    string getUserId() const;
    vector<string> getBorrowedBooks() const;
    
    // Setters
    void setName(const string& name);
    void setUserId(const string& userId);
    
    // Methods
    void borrowBook(const string& isbn);
    void returnBook(const string& isbn);
    bool hasBorrowedBook(const string& isbn) const;
    int getNumberOfBorrowedBooks() const;
    string toString() const;
    string toFileFormat() const;
    void fromFileFormat(const string& line);
};

#endif
