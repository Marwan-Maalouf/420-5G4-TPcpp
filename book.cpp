#include "book.h"
#include <iostream>


//Constructeurs
Book::Book(): title(""), author(""), isbn(""), isAvailable(false), borrowerId("") {}
Book::Book(const std::string& title, const std::string& author, const std::string& isbn): title(title), author(author), isbn(isbn), isAvailable(true), borrowerId("") {}

//getters
 std::string Book::getTitle() const {
    return title;
}

 std::string Book::getAuthor() const {
    return author;
}

 std::string Book::getISBN() const {
    return isbn;
}

bool Book::getAvailability() const {
    return isAvailable;
}

 std::string Book::getBorrowerId() const {
    return borrowerId;
}

//setters;

void Book::setTitle(const std::string& title) {
    this->title = title;
}

void Book::setAuthor(const std::string& author) {
    this->author = author;
}

void Book::setISBN(const std::string& isbn) {
    this->isbn = isbn;
}

void Book::setAvailability(bool isAvailable) {
    this->isAvailable = isAvailable;
}

void Book::setBorrowerId(const std::string& borrowerId) {
    this->borrowerId = borrowerId;
}