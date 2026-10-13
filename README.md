# Construire le projet
Vous pouvez utiliser un dev container de base C++ de VScode.
Le projet utilise cmake, pensez à l'inclure dans votre dev container.

Voici les lignes de commandes pour compiler le projet:
```
$ mkdir build
$ cd build
$ cmake ..
$ make
```

# Répertoire data

Il contient 2 fichiers `books.txt`et `users.txt` que vous pouvez utilisez pour tester votre code.
Pour ca il suffit de donner chemin vers le repertoire data avec l'application `bibliotheque -d <chemin verrs data>`


# Veille technologique: Systeme de gestion de bibliotheque personnelle

**Auteur:** Marwan Maalouf
**Cours:** 420-5G4

## Fonctionnalites implementees:

- numérotation automatique des ID utilisateurs.
- Tri des résultats par titre, auteur pour l’affichage (utilisation de la fonction de tri de la STL).

## Questions

### Question 1 : C++

j'ai utilise le pointeur intelligent (`std::unique_ptr`) qui vient de l'en-tete *memory* pour guarantir une propriete exclusive a un objet alloue dynamiquement. Un `unique_ptr` ne peut posseder qu'un seul objet a la fois, c'est-a-dire qu'on ne peut pas le copier. Quand on supprime le pointeur, c'est l'equivalent a faire `delete`. Je l'ai utilise dans `library.cpp` pour gerer les fonctions d'ajout et de suppression de livre, car les vecteurs de `Book` et `User` sont des pointeurs intelligents.

Pour l'ajout d'un livre, je fais `make_unique` pour creer un objet de type `Book` en copiant celui recu en parametre. Ensuite, Le pointeur se range dans le vecteur:

```cpp
void Library::addBook(const Book& book) {
    books.push_back(make_unique<Book>(book));
}
```

Pour la suppression d'un livre, `books.erase(it)` retire le `unique_ptr` du vecteur, ce qui libere le `Book` correspondant (et cela sans utiliser `delete`).

```cpp
bool Library::removeBook(const string& isbn) {
    auto it = find_if(books.begin(), books.end(),
        [&isbn](const unique_ptr<Book>& book) {
            return book->getISBN() == isbn;
        });

    if (it != books.end()) {
        books.erase(it);
        return true;
    }
    return false;
}
```

### Question 2 : Options de developpement possible

Ce que je suggererais qui sera mieux est d'avoir un meilleur systeme de stockage que celui actuel, car cette-derniere ne pourra tenir quelques milliers et dizaines de milliers de livres comme une vrai bibliotheque comme celle d'Alexandrie. Le systeme de recherche et de chargement prendront trop de temps et cela peut etre plus efficace. Il sera mieux de remplacer le fichier txt par une base de donnees relationnelles, PostgreSQL, avec des tables correspondantes comme livres, users et emprunts avec des cles primaires comme ISBN ou ID. Pour le tri, un simple `ORDER BY titre, auteur` suffit.

On peut aussi ameliorer le cote C++ en ajoutant une interface avec libpqxx par exemple qui est recommande pour PostgreSQL. La classe `Database` pourrait remplacer `FileManager`, comme ca les operations du programme (ajout, recherche, suppression, etc.) enverrait des requetes SQL. On peut aussi essayer de separer l'application eu 2: un cote API rest qui est expose par un cote serveur avec Python par exemple, et le programme lui-meme en C++.