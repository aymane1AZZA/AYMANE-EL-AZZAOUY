#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char nom[100];  // Nom de l'arr�t
    int horaires[10]; // Tableau d'horaires de passage (par exemple 10 horaires max)
} Arret;

typedef struct {
    char nom[100];   // Nom de la ligne (ex : Ligne 1)
    Arret arrets[10]; // Liste d'arr�ts
    int nbArrets; // Nombre d'arr�ts sur cette ligne
} Ligne;

typedef struct {
    Ligne lignes[10];  // Liste de lignes de transport
    int nbLignes;  // Nombre de lignes dans le r�seau
} ReseauTransport;

void ajouterLigne() {
    ReseauTransport reseau;
    if (reseau.nbLignes >= 10) {
        printf("Le nombre maximum de lignes est atteint.\n");
        return;
    }

    Ligne nouvelleLigne;

    printf("Entrez le nom de la ligne: ");
    scanf(" %[^\n]", nouvelleLigne.nom);  // Utilisation de %[^\n] pour capturer les espaces

    do {
        printf("Entrez le nombre d'arr�ts de la ligne: ");
        scanf("%d", &nouvelleLigne.nbArrets);
        if (nouvelleLigne.nbArrets < 1 || nouvelleLigne.nbArrets > 10) {
            printf("Veuillez entrer un nombre valide d'arr�ts (1 � 10).\n");
        }
    } while (nouvelleLigne.nbArrets < 1 || nouvelleLigne.nbArrets > 10);

    for (int i = 0; i < nouvelleLigne.nbArrets; i++) {
        printf("Entrez le nom de l'arr�t %d: ", i + 1);
        scanf(" %[^\n]", nouvelleLigne.arrets[i].nom);  // Utilisation de %[^\n]

        int nbHoraires;
        do {
            printf("Entrez le nombre d'horaires pour cet arr�t (max 10): ");
            scanf("%d", &nbHoraires);
            if (nbHoraires < 1 || nbHoraires > 10) {
                printf("Veuillez entrer un nombre valide d'horaires (1 � 10).\n");
            }
        } while (nbHoraires < 1 || nbHoraires > 10);

        for (int j = 0; j < nbHoraires; j++) {
            printf("Entrez l'heure de passage %d pour cet arr�t: ", j + 1);
            scanf("%d", &nouvelleLigne.arrets[i].horaires[j]);
        }
    }

    // Ouverture du fichier en mode append pour ajouter la ligne � la fin du fichier
    FILE *f = fopen("lignes.csv", "a+");
    if (f == NULL) {
        printf("Erreur d'ouverture du fichier \n");
        return;
    }

 // Sauvegarde de la ligne dans le fichier
fprintf(f, "%s;%d", nouvelleLigne.nom, nouvelleLigne.nbArrets);
for (int i = 0; i < nouvelleLigne.nbArrets; i++) {
    fprintf(f, ";%s", nouvelleLigne.arrets[i].nom);  // Remplacer la virgule par un ;
    int horairesAffiches = 0;
    for (int j = 0; j < 10; j++) {  // Limite � 10 horaires
        if (nouvelleLigne.arrets[i].horaires[j] != 0) {  // Ignorer les horaires 0
            if (horairesAffiches > 0) {
                fprintf(f, ";");  // Ajouter un point-virgule comme s�parateur
            }
            fprintf(f, "%d", nouvelleLigne.arrets[i].horaires[j]);
            horairesAffiches++;
        }
    }
}
fprintf(f, "\n");

    fclose(f);

    // Ajout de la nouvelle ligne au r�seau
    reseau.lignes[reseau.nbLignes] = nouvelleLigne;
    reseau.nbLignes++;  // Incr�mentation du nombre de lignes
    printf("Ligne ajout�e avec succ�s.\n");
}

// Fonction pour afficher les lignes
void afficherLignes() {
    FILE *f = fopen("lignes.csv", "r");
    if (f == NULL) {
        printf("Erreur d'ouverture du fichier lignes.csv.\n");
        return;
    }

    Ligne ligne;
    printf("\n----- Liste des lignes -----\n");

    int cmp = 0;
    while (fscanf(f, "\"%[^\"]\",%d", ligne.nom, &ligne.nbArrets) == 2) {
        printf("Nom de la ligne: %s\n", ligne.nom);
        printf("Nombre d'arr�ts: %d\n", ligne.nbArrets);

        for (int i = 0; i < ligne.nbArrets; i++) {
            fscanf(f, ",\"%[^\"]\",", ligne.arrets[i].nom); // Lire le nom de l'arr�t
            printf("  Arr�t %d: %s\n", i + 1, ligne.arrets[i].nom);

            // Lire les horaires pour cet arr�t
            for (int j = 0; j < 10; j++) {
                if (fscanf(f, "%d", &ligne.arrets[i].horaires[j]) != 1) {
                    break; // Sortir si on ne peut pas lire un horaire
                }
                if (j < 9) {
                    fscanf(f, ";"); // Lire le s�parateur
                }
            }
        }
        printf("------------------------\n");
        cmp++;
    }

    if (cmp == 0) {
        printf("Aucune ligne n'a �t� trouv�e.\n");
    }

    if (ferror(f)) {
        printf("Erreur lors de la lecture du fichier.\n");
    } else {
        printf("Fin de l'affichage des lignes.\n");
    }

    fclose(f);
}

// Fonction pour supprimer une ligne
void supprimerLigne() {
    FILE *f = fopen("lignes.csv", "r+");
    if (f == NULL) {
        printf("Erreur d'ouverture du fichier lignes.csv.\n");
        return;
    }

    FILE *tempf = fopen("temp.csv", "w");
    if (tempf == NULL) {
        printf("Erreur d'ouverture du fichier temporaire.\n");
        fclose(f);
        return;
    }

    char nomLigneSupprimer[100];
    printf("Entrez le nom de la ligne � supprimer: ");
    scanf(" %[^\n]", nomLigneSupprimer);

    Ligne ligne;
    int trouve = 0;

    rewind(f);
    while (fscanf(f, "\"%[^\"]\",%d", ligne.nom, &ligne.nbArrets) == 2) {
        // Lire les arr�ts
        for (int i = 0; i < ligne.nbArrets; i++) {
            fscanf(f, ",\"%[^\"]\",", ligne.arrets[i].nom); // Lire le nom de l'arr�t
            for (int j = 0; j < 10; j++) {
                fscanf(f, "%d", &ligne.arrets[i].horaires[j]);
                if (j < 9) {
                    fscanf(f, ";"); // Lire le s�parateur
                }
            }
        }

        if (strcmp(ligne.nom, nomLigneSupprimer) != 0) {
            // �crire la ligne dans le fichier temporaire si elle n'est pas � supprimer
            fprintf(tempf, "\"%s\",%d", ligne.nom, ligne.nbArrets);
            for (int i = 0; i < ligne.nbArrets; i++) {
                fprintf(tempf, ",\"%s\",", ligne.arrets[i].nom);
                for (int j = 0; j < 10; j++) {
                    fprintf(tempf, "%d", ligne.arrets[i].horaires[j]);
                    if (j < 9) {
                        fprintf(tempf, ";"); // S�parer les horaires par un point-virgule
                    }
                }
            }
            fprintf(tempf, "\n");
        } else {
            trouve = 1; // Ligne trouv�e pour suppression
        }
    }

    if (trouve) {
        printf("La ligne '%s' a �t� supprim�e.\n", nomLigneSupprimer);
    } else {
        printf("Aucune ligne avec le nom '%s' n'a �t� trouv�e.\n", nomLigneSupprimer);
    }

    fclose(f);
    fclose(tempf);

    remove("lignes.csv");
    rename("temp.csv", "lignes.csv");
}

void modifierLigne() {
    FILE *f = fopen("lignes.csv", "r+");
    if (f == NULL) {
        printf("Erreur d'ouverture du fichier lignes.csv.\n");
        return;
    }

    FILE *tempf = fopen("temp.csv", "w");
    if (tempf == NULL) {
        printf("Erreur d'ouverture du fichier temporaire.\n");
        fclose(f);
        return;
    }

    char nomLigneModifier[100];
    printf("Entrez le nom de la ligne � modifier: ");
    scanf(" %[^\n]", nomLigneModifier);

    Ligne ligne;
    int trouve = 0;

    rewind(f);
    while (fscanf(f, "\"%[^\"]\",%d", ligne.nom, &ligne.nbArrets) == 2) {
        // Lire les arr�ts
        for (int i = 0; i < ligne.nbArrets; i++) {
            fscanf(f, ",\"%[^\"]\",", ligne.arrets[i].nom); // Lire le nom de l'arr�t
            for (int j = 0; j < 10; j++) {
                fscanf(f, "%d", &ligne.arrets[i].horaires[j]);
                if (j < 9) {
                    fscanf(f, ";"); // Lire le s�parateur
                }
            }
        }

        if (strcmp(ligne.nom, nomLigneModifier) == 0) {
            printf("Ligne trouv�e. Entrez les nouvelles informations :\n");
            printf("Nouveau nom de la ligne: ");
            scanf(" %[^\n]", ligne.nom);
            printf("Entrez le nouveau nombre d'arr�ts: ");
            scanf("%d", &ligne.nbArrets);

            for (int i = 0; i < ligne.nbArrets; i++) {
                printf("Entrez le nom de l'arr�t %d: ", i + 1);
                scanf(" %[^\n]", ligne.arrets[i].nom);

                printf("Entrez les horaires pour cet arr�t (max 10):\n");
                for (int j = 0; j < 10; j++) {
                    printf("Horaire %d: ", j + 1);
                    scanf("%d", &ligne.arrets[i].horaires[j]);
                }
            }
            trouve =  1;
        }

        // �criture dans le fichier temporaire
        fprintf(tempf, "\"%s\",%d", ligne.nom, ligne.nbArrets);
        for (int i = 0; i < ligne.nbArrets; i++) {
            fprintf(tempf, ",\"%s\",", ligne.arrets[i].nom);
            for (int j = 0; j < 10; j++) {
                fprintf(tempf, "%d", ligne.arrets[i].horaires[j]);
                if (j < 9) {
                    fprintf(tempf, ";"); // S�parer les horaires par un point-virgule
                }
            }
        }
        fprintf(tempf, "\n");
    }

    if (trouve) {
        printf("La ligne '%s' a �t� modifi�e.\n", nomLigneModifier);
    } else {
        printf("Aucune ligne avec le nom '%s' n'a �t� trouv�e.\n", nomLigneModifier);
    }

    fclose(f);
    fclose(tempf);

    remove("lignes.csv");
    rename("temp.csv", "lignes.csv");
}

// Fonction pour rechercher un itin�raire
void rechercherItineraire() {
    FILE *f = fopen("lignes.csv", "r");
    if (f == NULL) {
        printf("Erreur d'ouverture du fichier lignes.csv.\n");
        return;
    }

    char critere[100];
    printf("Entrez le crit�re de recherche (Nom de la ligne): ");
    getchar();
    scanf("%[^\n]", critere);

    Ligne ligne;
    int trouve = 0;

    printf("\n----- R�sultats de la recherche -----\n");

    while (fscanf(f, "\"%[^\"]\",%d", ligne.nom, &ligne.nbArrets) == 2) {
        // Lire les arr�ts
        for (int i = 0; i < ligne.nbArrets; i++) {
            fscanf(f, ",\"%[^\"]\",", ligne.arrets[i].nom); // Lire le nom de l'arr�t
            for (int j = 0; j < 10; j++) {
                fscanf(f, "%d", &ligne.arrets[i].horaires[j]);
                if (j < 9) {
                    fscanf(f, ";"); // Lire le s�parateur
                }
            }
        }

        if (strstr(ligne.nom, critere) != NULL) {
            printf("Nom de la ligne: %s\n", ligne.nom);
            printf("Nombre d'arr�ts: %d\n", ligne.nbArrets);
            for (int i = 0; i < ligne.nbArrets; i++) {
                printf("  Arr�t %d: %s\n", i + 1, ligne.arrets[i].nom);
            }
            printf("------------------------\n");
            trouve = 1;
        }
    }

    if (!trouve) {
        printf("Aucune ligne trouv�e avec ce crit�re.\n");
    }

    fclose(f);
}

// Fonction pour rechercher un itin�raire avanc�


void rechercherItineraireAvance() {
    FILE *f = fopen("lignes.csv", "r");
    if (f == NULL) {
        printf("Erreur d'ouverture du fichier lignes.csv.\n");
        return;
    }

    char critere[100];
    printf("Entrez le crit�re de recherche (Nom de l'arr�t): ");
    getchar();  // Consommer le newline restant dans le buffer
    scanf("%[^\n]", critere);  // Lire le crit�re (nom de l'arr�t)

    Ligne ligne;
    int trouve = 0;

    printf("\n----- R�sultats de la recherche avanc�e -----\n");

    while (fscanf(f, "%99[^;];%d", ligne.nom, &ligne.nbArrets) == 2) {
        for (int i = 0; i < ligne.nbArrets; i++) {
            fscanf(f, ";%99[^;];", ligne.arrets[i].nom);  // Lire le nom de l'arr�t
            for (int j = 0; j < 10; j++) {
                int horaire;
                if (fscanf(f, "%d", &horaire) == 1) {
                    if (horaire != 0) {
                        ligne.arrets[i].horaires[j] = horaire;
                    }
                }
                if (fgetc(f) == ';') { // Lire le s�parateur ;
                    continue;
                } else {
                    break;
                }
            }

            // Recherche de l'arr�t correspondant au crit�re
            if (strstr(ligne.arrets[i].nom, critere) != NULL) {  // Si l'arr�t correspond au crit�re
                printf("Nom de la ligne: %s\n", ligne.nom);
                printf("Arr�t trouv�: %s\n", ligne.arrets[i].nom);
                printf("Horaires: ");
                int horairesAffiches = 0;
                for (int j = 0; j < 10; j++) {
                    if (ligne.arrets[i].horaires[j] != 0) {
                        if (horairesAffiches > 0) {
                            printf(" ");  // S�parer les horaires par un espace
                        }
                        printf("%d", ligne.arrets[i].horaires[j]);
                        horairesAffiches++;
                    }
                }
                if (horairesAffiches == 0) {
                    printf("Aucun horaire disponible");
                }
                printf("\n------------------------\n");
                trouve = 1;
                break;
            }
        }
    }

    if (!trouve) {
        printf("Aucun itin�raire trouv� avec ce crit�re.\n");
    }

    fclose(f);
}


// Fonction principale
int main() {
    ReseauTransport reseau;
    reseau.nbLignes = 0; // Initialiser le nombre de lignes

    int choix;
    do {
        printf("\n----- Menu -----\n");
        printf("1. Ajouter une ligne\n");
        printf("2. Afficher les lignes\n");
        printf("3. Modifier une ligne\n");
        printf("4. Supprimer une ligne\n");
        printf("5. Rechercher un itin�raire\n");
        printf("6. Rechercher un itin�raire avanc�\n");
        printf("0. Quitter\n");
        printf("Choisissez une option: ");
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                ajouterLigne();
                break;
            case 2:
                afficherLignes();
                break;
            case 3:
                modifierLigne();
                break;
            case 4:
                supprimerLigne();
                break;
            case 5:
                rechercherItineraire();
                break;
            case 6:
                rechercherItineraireAvance();
                break;
            case 0:
                printf("Au revoir!\n");
                break;
            default:
                printf("Choix invalide. Veuillez r�essayer.\n");
        }
    } while (choix != 0);

    return 0;
}
