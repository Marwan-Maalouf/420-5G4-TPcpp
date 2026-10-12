#include <iostream>
#include <limits>
#include <string>
#include <filesystem>
#include <algorithm>

#include "library.h"
#include "filemanager.h"
#include "logger.h"

using namespace std;




void clearScreen() {
    system("cls || clear");
}

void pauseForInput() {
    cout << "\nAppuyez sur Entrée pour continuer...";
    cin.ignore();
    cin.get();
}

void displayMenu() {
    cout << "\n=== SYSTÈME DE GESTION DE BIBLIOTHÈQUE PERSONNELLE ===\n";
    cout << "1.  Ajouter un Livre\n";
    cout << "2.  Supprimer un Livre\n";
    cout << "3.  Rechercher des Livres par Titre\n";
    cout << "4.  Rechercher des Livres par Auteur\n";
    cout << "5.  Afficher Tous les Livres\n";
    cout << "6.  Afficher les Livres Disponibles\n";
    cout << "7.  Ajouter un Utilisateur\n";
    cout << "8.  Afficher Tous les Utilisateurs\n";
    cout << "9.  Emprunter un Livre\n";
    cout << "10. Retourner un Livre\n";
    cout << "11. Statistiques de la Bibliothèque\n";
    cout << "12. Sauvegarder les Données\n";
    cout << "13. Créer une Sauvegarde\n";
    cout << "0.  Quitter\n";
    cout << "======================================================\n";
    cout << "Entrez votre choix : ";
}

string getInput(const string& prompt) {
    string input;
    cout << prompt;
    getline(cin, input);
    return input;
}

void printUsage(const char* programName) {
    cerr << "Usage: " << programName << " [--data-dir|-d <répertoire>]\n";
}

int main(int argc, char* argv[]) {
    string dataDir;
    string logFile = dataDir.empty() ? "journal.log"
                                     : (filesystem::path(dataDir) / "journal.log").string();
    Logger logger(logFile);
    logger.log("DÉMARRAGE", "Application démarrée.");
    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "--data-dir" || arg == "-d") {
            if (i + 1 >= argc) {
                cerr << "Erreur : " << arg << " nécessite un répertoire.\n";
                printUsage(argv[0]);
                return 1;
            }
            dataDir = argv[++i];
        } else {
            cerr << "Erreur : argument inconnu '" << arg << "'.\n";
            printUsage(argv[0]);
            return 1;
        }
    }

    string booksFile;
    string usersFile;
    if (!dataDir.empty()) {
        if (!filesystem::is_directory(dataDir)) {
            cerr << "Erreur : le répertoire " << dataDir << " n'existe pas.\n";
            printUsage(argv[0]);
            return 1;
        }
        booksFile = (filesystem::path(dataDir) / "books.txt").string();
        usersFile = (filesystem::path(dataDir) / "users.txt").string();
    }

    Library library;
    FileManager fileManager(booksFile, usersFile);
    bool canSaveData = !booksFile.empty() && !usersFile.empty();
    
    // Load existing data
    cout << "Chargement des données de la bibliothèque...\n";
    fileManager.loadLibraryData(library);
    
    int choice;
    bool running = true;
    
    while (running) {
        displayMenu();
        
        if (!(cin >> choice)) {
            cout << "Saisie invalide. Veuillez entrer un nombre.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            pauseForInput();
            continue;
        }
        cin.ignore(); // Clear newline from buffer
        
        switch (choice) {
            case 1: { // Add Book
                string title = getInput("Entrez le titre du livre : ");
                string author = getInput("Entrez l'auteur du livre : ");
                string isbn = getInput("Entrez l'ISBN du livre : ");
                
                if (library.findBookByISBN(isbn)) {
                    cout << "Erreur : Un livre avec l'ISBN " << isbn << " existe déjà.\n";
                    logger.log("ERREUR", "Un livre avec l'ISBN " + isbn + " existe déjà.");
                } else {
                    Book newBook(title, author, isbn);
                    library.addBook(newBook);
                    cout << "Livre ajouté avec succès !\n";
                    logger.log("AJOUT LIVRE", "Livre ajouté : " + title + " (ISBN " + isbn + ")");
                }
                pauseForInput();
                
                break;
            }
            
            case 2: { // Remove Book
                string isbn = getInput("Entrez l'ISBN du livre à supprimer : ");
                
                if (library.removeBook(isbn)) {
                    cout << "Livre supprimé avec succès !\n";
                    logger.log("SUPPRESSION LIVRE", "Livre supprimé : " + isbn);
                } else {
                    cout << "Livre non trouvé.\n";
                    logger.log("ERREUR SUPPRESSION", "Tentative de suppression d'un livre non trouvé : " + isbn + " introuvable");
                }
                pauseForInput();
                break;
            }
            
            case 3: { // Search by Title
                string title = getInput("Entrez le titre à rechercher : ");
                auto results = library.searchBooksByTitle(title);
                
                if (results.empty()) {
                    cout << "Aucun livre trouvé avec ce titre.\n";
                    logger.log("ERREUR RECHERCHE", "Aucun livre trouvé avec le titre : " + title);
                } else {
                    cout << "\n=== RÉSULTATS DE RECHERCHE ===\n";
                    for (size_t i = 0; i < results.size(); ++i) {
                        cout << "\nRésultat " << (i + 1) << " :\n";
                        cout << results[i]->toString() << "\n";
                        cout << "-----------------------------\n";
                    }
                    logger.log("RECHERCHE", "Résultats de la recherche par titre : " + title);
                }
                pauseForInput();
                break;
            }
            
            case 4: { // Search by Author
                string author = getInput("Entrez l'auteur à rechercher : ");
                auto results = library.searchBooksByAuthor(author);
                
                if (results.empty()) {
                    cout << "Aucun livre trouvé de cet auteur.\n";
                    logger.log("ERREUR RECHERCHE", "Aucun livre trouvé de l'auteur : " + author);
                } else {
                    cout << "\n=== RÉSULTATS DE RECHERCHE ===\n";
                    for (size_t i = 0; i < results.size(); ++i) {
                        cout << "\nRésultat " << (i + 1) << " :\n";
                        cout << results[i]->toString() << "\n";
                        cout << "-----------------------------\n";
                    }
                    logger.log("RECHERCHE", "Résultats de la recherche par auteur : " + author);
                }
                pauseForInput();
                break;
            }
            
            case 5: // Display All Books
                library.displayAllBooks();
                logger.log("AFFICHAGE", "Affichage de tous les livres.");
                pauseForInput();
                break;
            
            case 6: // Display Available Books
                library.displayAvailableBooks();
                logger.log("AFFICHAGE", "Affichage des livres disponibles.");
                pauseForInput();
                break;
            
            case 7: { // Add User
                string name = getInput("Entrez le nom de l'utilisateur : ");
                string userId = getInput("Entrez l'ID de l'utilisateur : ");
                
                if (library.findUserById(userId)) {
                    cout << "Erreur : Un utilisateur avec l'ID " << userId << " existe déjà.\n";
                    logger.log("ERREUR AJOUT UTILISATEUR", "Tentative d'ajout d'un utilisateur existant : " + userId + " existe deja");
                } else {
                    User newUser(name, userId);
                    library.addUser(newUser);
                    cout << "Utilisateur ajouté avec succès !\n";
                    logger.log("AJOUT UTILISATEUR", "Utilisateur ajouté : " + name + " (ID " + userId + ")");
                }
                pauseForInput();
                break;
            }
            
            case 8: // Display All Users
                library.displayAllUsers();
                logger.log("AFFICHAGE", "Affichage de tous les utilisateurs.");
                pauseForInput();
                break;
            
            case 9: { // Check Out Book
                string isbn = getInput("Entrez l'ISBN du livre à emprunter : ");
                string userId = getInput("Entrez l'ID de l'utilisateur : ");
                
                if (library.checkOutBook(isbn, userId)) {
                    cout << "Livre emprunté avec succès !\n";
                    logger.log("EMPRUNT", "Livre emprunté : " + isbn + " par l'utilisateur : " + userId);
                } else {
                    cout << "Erreur : Impossible d'emprunter le livre. Vérifiez l'ISBN, l'ID utilisateur et la disponibilité du livre.\n";
                    logger.log("ERREUR EMPRUNT", "Échec de l'emprunt du livre : " + isbn + " par l'utilisateur : " + userId + ": non disponible");
                }
                pauseForInput();
                break;
            }
            
            case 10: { // Return Book
                string isbn = getInput("Entrez l'ISBN du livre à retourner : ");
                
                if (library.returnBook(isbn)) {
                    cout << "Livre retourné avec succès !\n";
                    logger.log("RETOUR", "Livre retourné : " + isbn);
                } else {
                    cout << "Erreur : Impossible de retourner le livre. Vérifiez l'ISBN et que le livre est bien emprunté.\n";
                    logger.log("ERREUR RETOUR", "Échec du retour du livre : " + isbn + ": non emprunté ou introuvable");
                }
                pauseForInput();
                break;
            }
            
            case 11: { // Library Statistics
                cout << "\n=== STATISTIQUES DE LA BIBLIOTHÈQUE ===\n";
                cout << "Total des Livres : " << library.getTotalBooks() << "\n";
                cout << "Livres Disponibles : " << library.getAvailableBookCount() << "\n";
                cout << "Livres Empruntés : " << library.getCheckedOutBookCount() << "\n";
                cout << "Total des Utilisateurs : " << library.getAllUsers().size() << "\n";
                logger.log("STATISTIQUES", "Affichage des statistiques de la bibliothèque.");
                pauseForInput();
                break;
            }
            case 12: {
                if (!canSaveData) {
                    cout << "Impossible de sauvegarder : aucun répertoire de données (--data-dir).\n";
                    logger.log("ERREUR SAUVEGARDE", "Répertoire de données non spécifié.");
                } else if (fileManager.saveLibraryData(library)) {
                    cout << "Données de la bibliothèque sauvegardées avec succès !\n";
                    logger.log("SAUVEGARDE", "Données de la bibliothèque sauvegardées.");
                } else {
                    cout << "Erreur lors de la sauvegarde des données de la bibliothèque.\n";
                    logger.log("ERREUR SAUVEGARDE", "Échec de la sauvegarde des données de la bibliothèque.");
                }
                pauseForInput();
                break;
            }

            case 13: {
                if (!canSaveData) {
                    cout << "Impossible de créer une sauvegarde : aucun répertoire de données (--data-dir).\n";
                    logger.log("ERREUR SAUVEGARDE", "Répertoire de données non spécifié.");
                } else {
                    fileManager.createBackup();
                    logger.log("SAUVEGARDE", "Sauvegarde de la bibliothèque effectuée.");
                }
                pauseForInput();
                break;
            }
            
            case 0: // Exit
                 if (canSaveData) {
                    cout << "Sauvegarde des données avant la fermeture...\n";
                    if (fileManager.saveLibraryData(library)) {
                        logger.log("FERMETURE", "Sauvegarde des données effectuée.");
                    } else {
                        cout << "Erreur lors de la sauvegarde des données.\n";
                        logger.log("ERREUR SAUVEGARDE", "Échec de la sauvegarde à la fermeture.");
                    }
                } else {
                    cout << "Aucun répertoire de données (--data-dir) : les données ne sont pas sauvegardées.\n";
                    logger.log("FERMETURE", "Aucune sauvegarde : répertoire de données non spécifié.");
                }
                cout << "Merci d'avoir utilisé le Système de Gestion de Bibliothèque Personnelle !\n";
                running = false;
                break;
            
            default:
                cout << "Choix invalide. Veuillez réessayer.\n";
                pauseForInput();
                break;
        }
    }
    
    return 0;
}