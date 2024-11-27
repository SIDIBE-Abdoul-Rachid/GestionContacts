#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <limits>
#include <regex>
#include <cctype>

// Structure pour representer un contact
struct Contact {
    std::string nom, prenom, numero, email, adresse;
};

// Liste des contacts
std::vector<Contact> contacts;

// Fonction pour lire une ligne de texte et éviter les erreurs de saisie
std::string lireString() {
    std::string input;
    std::getline(std::cin, input);
    return input;
}

// Fonction pour vérifier si une chaîne est un nombre
bool estNumerique(const std::string& str) {
    for (char c : str) {
        if (!isdigit(c)) return false;
    }
    return true;
}

// Fonction pour vérifier si une adresse email est valide
bool estEmailValide(const std::string& email) {
    std::regex pattern("^[\\w-]+(\\.[\\w-]+)*@[\\w-]+(\\.[a-zA-Z]{2,})+$");
    return std::regex_match(email, pattern);
}

// Fonction pour convertir une chaîne de caractères en minuscules
std::string convertirEnMinuscules(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

// Vérifier que le nom et prénom ne sont pas vides
bool estNonVide(const std::string& str) {
    return !str.empty();
}

// Sauvegarder les contacts dans un fichier
void sauvegarderContacts() {
    std::ofstream fichier("contacts.txt");
    if (!fichier) {
        std::cout << "Erreur d'ouverture du fichier pour sauvegarde.\n";
        return;
    }

    std::sort(contacts.begin(), contacts.end(), [](const Contact& a, const Contact& b) {
        return a.nom < b.nom;
    });

    for (const auto& c : contacts) {
        fichier << c.nom << " " << c.prenom << " " << c.numero << " " << c.email << " " << c.adresse << '\n';
    }
    fichier.close();
    std::cout << "Contacts sauvegardes avec succes.\n";
}

// Ajouter un contact
void ajouterContact() {
    Contact c;

    // Vérifier que le nom n'est pas vide
    do {
        std::cout << "Nom : ";
        c.nom = lireString();
        if (!estNonVide(c.nom)) {
            std::cout << "Le nom ne peut pas etre vide. Veuillez entrer un nom valide.\n";
        }
    } while (!estNonVide(c.nom));

    // Vérifier que le prénom n'est pas vide
    do {
        std::cout << "Prenom : ";
        c.prenom = lireString();
        if (!estNonVide(c.prenom)) {
            std::cout << "Le prenom ne peut pas etre vide. Veuillez entrer un prenom valide.\n";
        }
    } while (!estNonVide(c.prenom));

    std::cout << "Numero : ";
    std::string numero;
    while (true) {
        numero = lireString();
        if (estNumerique(numero)) {
            c.numero = numero;
            break;
        } else {
            std::cout << "Veuillez entrer un numero valide (composé uniquement de chiffres) : ";
        }
    }

    std::cout << "Email : ";
    while (true) {
        c.email = lireString();
        if (estEmailValide(c.email)) {
            break;
        } else {
            std::cout << "Veuillez entrer un email valide : ";
        }
    }

    std::cout << "Adresse : "; c.adresse = lireString();
    contacts.push_back(c);
    std::cout << "Contact ajoute avec succes !\n";

    // Sauvegarde automatique après ajout
    sauvegarderContacts();
}

// Afficher tous les contacts
void afficherContacts() {
    if (contacts.empty()) {
        std::cout << "Aucun contact a afficher.\n";
        return;
    }
    for (const auto& c : contacts) {
        std::cout << "Nom: " << c.nom << ", Prenom: " << c.prenom << ", Numero: " << c.numero
                  << ", Email: " << c.email << ", Adresse: " << c.adresse << '\n';
    }
}

// Rechercher un contact par nom (ignorant la casse)
void rechercherContact() {
    std::string nomRecherche;
    std::cout << "Entrez le nom a rechercher : ";
    nomRecherche = lireString();

    std::vector<int> indices;
    // Convertir la recherche en minuscules
    std::string nomRechercheMinuscule = convertirEnMinuscules(nomRecherche);

    for (int i = 0; i < contacts.size(); ++i) {
        // Comparer le nom de chaque contact en minuscules
        if (convertirEnMinuscules(contacts[i].nom) == nomRechercheMinuscule) {
            indices.push_back(i);
            std::cout << "Contact " << indices.size() << " : \n"
                      << "Nom: " << contacts[i].nom << ", Prenom: " << contacts[i].prenom
                      << ", Numero: " << contacts[i].numero << ", Email: " << contacts[i].email
                      << ", Adresse: " << contacts[i].adresse << '\n';
        }
    }

    if (indices.empty()) {
        std::cout << "Aucun contact trouve avec ce nom.\n";
    } else {
        int choix;
        do {
            std::cout << "Selectionnez le numero du contact a afficher : ";
            std::cin >> choix;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Ignore les caractères supplémentaires
        } while (choix < 1 || choix > indices.size());

        int index = indices[choix - 1];
        std::cout << "Contact selectionne : \n"
                  << "Nom: " << contacts[index].nom << ", Prenom: " << contacts[index].prenom
                  << ", Numero: " << contacts[index].numero << ", Email: " << contacts[index].email
                  << ", Adresse: " << contacts[index].adresse << '\n';
    }
}

// Supprimer un contact
void supprimerContact() {
    std::string nomASupprimer;
    std::cout << "Entrez le nom du contact a supprimer : ";
    nomASupprimer = lireString(); // Lire la saisie de l'utilisateur pour le nom

    std::vector<int> indices;
    // Trouver tous les contacts avec le même nom
    for (int i = 0; i < contacts.size(); ++i) {
        // Comparer le nom de chaque contact en minuscules
        if (convertirEnMinuscules(contacts[i].nom) == convertirEnMinuscules(nomASupprimer)) {
            indices.push_back(i);
            std::cout << "Contact " << indices.size() << " : \n"
                      << "Nom: " << contacts[i].nom << ", Prenom: " << contacts[i].prenom
                      << ", Numero: " << contacts[i].numero << ", Email: " << contacts[i].email
                      << ", Adresse: " << contacts[i].adresse << '\n';
        }
    }

    if (indices.empty()) {
        std::cout << "Aucun contact trouve avec ce nom.\n";
    } else {
        int choix;
        // Demander à l'utilisateur de sélectionner un contact à supprimer
        do {
            std::cout << "Selectionnez le numero du contact a supprimer : ";
            std::cin >> choix;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Ignore les caractères supplémentaires
        } while (choix < 1 || choix > indices.size());  // Assurer que l'utilisateur entre un choix valide

        int index = indices[choix - 1];
        // Demander confirmation avant de supprimer
        std::cout << "Vous etes sur de vouloir supprimer ce contact ? (o/n) : ";
        char confirmation;
        std::cin >> confirmation;

        if (confirmation == 'o' || confirmation == 'O') {
            contacts.erase(contacts.begin() + index);  // Supprimer le contact de la liste
            std::cout << "Contact supprime avec succes.\n";
        } else {
            std::cout << "Suppression annulee.\n";
        }
    }

    // Sauvegarde automatique après suppression
    sauvegarderContacts();
}

// Trier les contacts par nom
void trierContacts() {
    std::cout << "\nAvant le tri:\n";
    afficherContacts(); // Affiche les contacts avant le tri

    std::sort(contacts.begin(), contacts.end(), [](const Contact& a, const Contact& b) {
        return a.nom < b.nom; // Trier par nom dans l'ordre croissant
    });

    std::cout << "\nApres le tri:\n";
    afficherContacts(); // Affiche les contacts apres le tri
}

// Modifier un contact avec gestion des erreurs
void modifierContact() {
    std::string nomRecherche;
    std::cout << "Entrez le nom du contact a modifier : ";
    nomRecherche = lireString();

    std::vector<int> indices;
    // Convertir la recherche en minuscules
    std::string nomRechercheMinuscule = convertirEnMinuscules(nomRecherche);

    for (int i = 0; i < contacts.size(); ++i) {
        // Comparer le nom de chaque contact en minuscules
        if (convertirEnMinuscules(contacts[i].nom) == nomRechercheMinuscule) {
            indices.push_back(i);
            std::cout << "Contact " << indices.size() << " : \n"
                      << "Nom: " << contacts[i].nom << ", Prenom: " << contacts[i].prenom
                      << ", Numero: " << contacts[i].numero << ", Email: " << contacts[i].email
                      << ", Adresse: " << contacts[i].adresse << '\n';
        }
    }

    if (indices.empty()) {
        std::cout << "Aucun contact trouve avec ce nom.\n";
    } else {
        int choix;
        do {
            std::cout << "Selectionnez le numero du contact a modifier : ";
            std::cin >> choix;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Ignore les caractères supplémentaires
        } while (choix < 1 || choix > indices.size());

        int index = indices[choix - 1];
        std::cout << "Contact selectionne : \n"
                  << "Nom: " << contacts[index].nom << ", Prenom: " << contacts[index].prenom
                  << ", Numero: " << contacts[index].numero << ", Email: " << contacts[index].email
                  << ", Adresse: " << contacts[index].adresse << '\n';

        std::cout << "Souhaitez-vous modifier ce contact ? (o/n) : ";
        char confirmation;
        std::cin >> confirmation;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore les caractères supplémentaires

        if (confirmation == 'o' || confirmation == 'O') {
            std::string nouveauNom, nouveauPrenom, nouveauNumero, nouveauEmail, nouvelleAdresse;

            // Vérification du nom
            do {
                std::cout << "Nom (" << contacts[index].nom << ") : "; std::getline(std::cin, nouveauNom);
            } while (!estNonVide(nouveauNom));

            // Vérification du prénom
            do {
                std::cout << "Prenom (" << contacts[index].prenom << ") : "; std::getline(std::cin, nouveauPrenom);
            } while (!estNonVide(nouveauPrenom));

            // Vérification du numéro
            do {
                std::cout << "Numero (" << contacts[index].numero << ") : "; std::getline(std::cin, nouveauNumero);
            } while (!estNumerique(nouveauNumero));

            // Vérification de l'email
            do {
                std::cout << "Email (" << contacts[index].email << ") : "; std::getline(std::cin, nouveauEmail);
            } while (!estEmailValide(nouveauEmail));

            std::cout << "Adresse (" << contacts[index].adresse << ") : "; std::getline(std::cin, nouvelleAdresse);

            std::cout << "Voici les nouvelles informations :\n"
                      << "Nom: " << nouveauNom << "\n"
                      << "Prenom: " << nouveauPrenom << "\n"
                      << "Numero: " << nouveauNumero << "\n"
                      << "Email: " << nouveauEmail << "\n"
                      << "Adresse: " << nouvelleAdresse << "\n";
            std::cout << "Confirmez-vous ces modifications ? (o/n) : ";
            char confirmerModif;
            std::cin >> confirmerModif;

            if (confirmerModif == 'o' || confirmerModif == 'O') {
                contacts[index].nom = nouveauNom;
                contacts[index].prenom = nouveauPrenom;
                contacts[index].numero = nouveauNumero;
                contacts[index].email = nouveauEmail;
                contacts[index].adresse = nouvelleAdresse;
                std::cout << "Les modifications ont ete enregistrees.\n";
            } else {
                std::cout << "Les modifications ont ete annulees.\n";
            }
        } else {
            std::cout << "Modification annulee.\n";
        }
    }

    // Sauvegarde automatique après modification
    sauvegarderContacts();
}

// Charger les contacts depuis un fichier
void chargerContactsDepuisFichier() {
    std::ifstream fichier("contacts.txt");
    if (!fichier) {
        std::cout << "Fichier de contacts introuvable. Creation d'un nouveau fichier.\n";
        return;
    }

    Contact c;
    while (fichier >> c.nom >> c.prenom >> c.numero >> c.email) {
        fichier.ignore();
        std::getline(fichier, c.adresse);
        contacts.push_back(c);
    }
    fichier.close();
    std::cout << "Contacts charges avec succes.\n";
}

int main() {
    chargerContactsDepuisFichier();
    int choix;
    do {
        std::cout << "\n1. Ajouter un contact\n2. Afficher les contacts\n3. Rechercher un contact\n";
        std::cout << "4. Supprimer un contact\n5. Trier les contacts par nom\n6. Modifier un contact\n";
        std::cout << "0. Quitter\n";
        std::cout << "Choix : ";
        std::cin >> choix;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore les caractères supplémentaires

        switch (choix) {
            case 1: ajouterContact(); break;
            case 2: afficherContacts(); break;
            case 3: rechercherContact(); break;
            case 4: supprimerContact(); break;
            case 5: trierContacts(); break;
            case 6: modifierContact(); break;
            case 0:
                sauvegarderContacts();
                std::cout << "Au revoir !\n";
                break;
            default:
                std::cout << "Choix invalide. Veuillez entrer un nombre entre 0 et 6.\n";
        }
    } while (choix != 0);

    return 0;
}
