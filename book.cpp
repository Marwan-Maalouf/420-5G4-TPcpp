#include "book.h"
#include <iostream>
#include <sstream>
#include <algorithm>


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

//methode checkout: emprunter un livre

void Book::checkOut(const std::string& borrowerId) {
    if (isAvailable)
    {
        this->isAvailable = false;
        this->borrowerId = borrowerId;
    }
    else {
        std::cout << "Book is not available: already taken." << std::endl;
    }
}


//methode returnBook: retourne le livre
void Book::returnBook() {
    //regarder si le livre est emprunté
        if (!isAvailable) {
            this->isAvailable = true;
            this->borrowerId = "";
        }
}


//methode toString: retourne une chaine de caracteres representant le livre
std::string Book::toString() const {
    std::string stringPattern = "\nTitre: " + title + "\nAuteur(e): " + author + "\nISBN: " + isbn + "\nDisponible: " + (isAvailable ? "Oui " : "Non ") + (isAvailable ? "": ("Code de l'emprunteur: " + borrowerId));
    return stringPattern;
}


//methode toFileFormat
std::string Book::toFileFormat() const {
    std::string fileFormat = title + "|" + author + "|" + isbn + "|" + (isAvailable ? "Oui" : "Non") + "|" + borrowerId;
    return fileFormat;
}

//methode fromFileFormat
void Book::fromFileFormat(const std::string& line) {
    std::stringstream ss(line);
    std::string isAvailableStr;

    std::getline(ss, title, '|');
    std::getline(ss, author, '|');
    std::getline(ss, isbn, '|');
    std::getline(ss, isAvailableStr, '|');
    std::getline(ss, borrowerId);

    isAvailable = (isAvailableStr == "1");
}
//methode compareTitle: comparer les titres
bool Book::compareTitle(const Book& other) const {
    return this->title < other.title;
}

//methode compareAuthor: comparer les auteurs
bool Book::compareAuthor(const Book& other) const {
    return this->author < other.author;
}