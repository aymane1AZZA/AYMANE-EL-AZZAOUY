#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char nom[100];  // Nom de l'arrêt
    int horaires[10]; // Tableau d'horaires de passage (par exemple 10 horaires max)
} Arret;

typedef struct {
    char nom[100];   // Nom de la ligne (ex : Ligne 1)
    Arret arrets[10]; // Liste d'arrêts
    int nbArrets; // Nombre d'arrêts sur cette ligne
} Ligne;

typedef struct {
    Ligne lignes[10];  // Liste de lignes de transport
    int nbLignes;  // Nombre de lignes dans le réseau
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
        printf("Entrez le nombre d'arrêts de la ligne: ");
        scanf("%d", &nouvelleLigne.nbArrets);
        if (nouvelleLigne.nbArrets < 1 || nouvelleLigne.nbArrets > 10) {
            printf("Veuillez entrer un nombre valide d'arrêts (1 à 10).\n");
        }
    } while (nouvelleLigne.nbArrets < 1 || nouvelleLigne.nbArrets > 10);

    for (int i = 0; i < nouvelleLigne.nbArrets; i++) {
        printf("Entrez le nom de l'arrêt %d: ", i + 1);
        scanf(" %[^\n]", nouvelleLigne.arrets[i].nom);  // Utilisation de %[^\n]

        int nbHoraires;
        do {
            printf("Entrez le nombre d'horaires pour cet arrêt (max 10): ");
            scanf("%d", &nbHoraires);
            if (nbHoraires < 1 || nbHoraires > 10) {
                printf("Veuillez entrer un nombre valide d'horaires (1 à 10).\n");
            }
        } while (nbHoraires < 1 || nbHoraires > 10);

        for (int j = 0; j < nbHoraires; j++) {
            printf("Entrez l'heure de passage %d pour cet arrêt: ", j + 1);
            scanf("%d", &nouvelleLigne.arrets[i].horaires[j]);
        }
    }

    // Ouverture du fichier en mode append pour ajouter la ligne à la fin du fichier
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
    for (int j = 0; j < 10; j++) {  // Limite à 10 horaires
        if (nouvelleLigne.arrets[i].horaires[j] != 0) {  // Ignorer les horaires 0
            if (horairesAffiches > 0) {
                fprintf(f, ";");  // Ajouter un point-virgule comme séparateur
            }
            fprintf(f, "%d", nouvelleLigne.arrets[i].horaires[j]);
            horairesAffiches++;
        }
    }
}
fprintf(f, "\n");

    fclose(f);

    // Ajout de la nouvelle ligne au réseau
    reseau.lignes[reseau.nbLignes] = nouvelleLigne;
    reseau.nbLignes++;  // Incrémentation du nombre de lignes
    printf("Ligne ajoutée avec succès.\n");
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
        printf("Nombre d'arrêts: %d\n", ligne.nbArrets);

        for (int i = 0; i < ligne.nbArrets; i++) {
            fscanf(f, ",\"%[^\"]\",", ligne.arrets[i].nom); // Lire le nom de l'arrêt
            printf("  Arrêt %d: %s\n", i + 1, ligne.arrets[i].nom);

            // Lire les horaires pour cet arrêt
            for (int j = 0; j < 10; j++) {
                if (fscanf(f, "%d", &ligne.arrets[i].horaires[j]) != 1) {
                    break; // Sortir si on ne peut pas lire un horaire
                }
                if (j < 9) {
                    fscanf(f, ";"); // Lire le séparateur
                }
            }
        }
        printf("------------------------\n");
        cmp++;
    }

    if (cmp == 0) {
        printf("Aucune ligne n'a été trouvée.\n");
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
    printf("Entrez le nom de la ligne à supprimer: ");
    scanf(" %[^\n]", nomLigneSupprimer);

    Ligne ligne;
    int trouve = 0;

    rewind(f);
    while (fscanf(f, "\"%[^\"]\",%d", ligne.nom, &ligne.nbArrets) == 2) {
        // Lire les arrêts
        for (int i = 0; i < ligne.nbArrets; i++) {
            fscanf(f, ",\"%[^\"]\",", ligne.arrets[i].nom); // Lire le nom de l'arrêt
            for (int j = 0; j < 10; j++) {
                fscanf(f, "%d", &ligne.arrets[i].horaires[j]);
                if (j < 9) {
                    fscanf(f, ";"); // Lire le séparateur
                }
            }
        }

        if (strcmp(ligne.nom, nomLigneSupprimer) != 0) {
            // Écrire la ligne dans le fichier temporaire si elle n'est pas à supprimer
            fprintf(tempf, "\"%s\",%d", ligne.nom, ligne.nbArrets);
            for (int i = 0; i < ligne.nbArrets; i++) {
                fprintf(tempf, ",\"%s\",", ligne.arrets[i].nom);
                for (int j = 0; j < 10; j++) {
                    fprintf(tempf, "%d", ligne.arrets[i].horaires[j]);
                    if (j < 9) {
                        fprintf(tempf, ";"); // Séparer les horaires par un point-virgule
                    }
                }
            }
            fprintf(tempf, "\n");
        } else {
            trouve = 1; // Ligne trouvée pour suppression
        }
    }

    if (trouve) {
        printf("La ligne '%s' a été supprimée.\n", nomLigneSupprimer);
    } else {
        printf("Aucune ligne avec le nom '%s' n'a été trouvée.\n", nomLigneSupprimer);
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
    printf("Entrez le nom de la ligne à modifier: ");
    scanf(" %[^\n]", nomLigneModifier);

    Ligne ligne;
    int trouve = 0;

    rewind(f);
    while (fscanf(f, "\"%[^\"]\",%d", ligne.nom, &ligne.nbArrets) == 2) {
        // Lire les arrêts
        for (int i = 0; i < ligne.nbArrets; i++) {
            fscanf(f, ",\"%[^\"]\",", ligne.arrets[i].nom); // Lire le nom de l'arrêt
            for (int j = 0; j < 10; j++) {
                fscanf(f, "%d", &ligne.arrets[i].horaires[j]);
                if (j < 9) {
                    fscanf(f, ";"); // Lire le séparateur
                }
            }
        }

        if (strcmp(ligne.nom, nomLigneModifier) == 0) {
            printf("Ligne trouvée. Entrez les nouvelles informations :\n");
            printf("Nouveau nom de la ligne: ");
            scanf(" %[^\n]", ligne.nom);
            printf("Entrez le nouveau nombre d'arrêts: ");
            scanf("%d", &ligne.nbArrets);

            for (int i = 0; i < ligne.nbArrets; i++) {
                printf("Entrez le nom de l'arrêt %d: ", i + 1);
                scanf(" %[^\n]", ligne.arrets[i].nom);

                printf("Entrez les horaires pour cet arrêt (max 10):\n");
                for (int j = 0; j < 10; j++) {
                    printf("Horaire %d: ", j + 1);
                    scanf("%d", &ligne.arrets[i].horaires[j]);
                }
            }
            trouve =  1;
        }

        // Écriture dans le fichier temporaire
        fprintf(tempf, "\"%s\",%d", ligne.nom, ligne.nbArrets);
        for (int i = 0; i < ligne.nbArrets; i++) {
            fprintf(tempf, ",\"%s\",", ligne.arrets[i].nom);
            for (int j = 0; j < 10; j++) {
                fprintf(tempf, "%d", ligne.arrets[i].horaires[j]);
                if (j < 9) {
                    fprintf(tempf, ";"); // Séparer les horaires par un point-virgule
                }
            }
        }
        fprintf(tempf, "\n");
    }

    if (trouve) {
        printf("La ligne '%s' a été modifiée.\n", nomLigneModifier);
    } else {
        printf("Aucune ligne avec le nom '%s' n'a été trouvée.\n", nomLigneModifier);
    }

    fclose(f);
    fclose(tempf);

    remove("lignes.csv");
    rename("temp.csv", "lignes.csv");
}

// Fonction pour rechercher un itinéraire
void rechercherItineraire() {
    FILE *f = fopen("lignes.csv", "r");
    if (f == NULL) {
        printf("Erreur d'ouverture du fichier lignes.csv.\n");
        return;
    }

    char critere[100];
    printf("Entrez le critère de recherche (Nom de la ligne): ");
    getchar();
    scanf("%[^\n]", critere);

    Ligne ligne;
    int trouve = 0;

    printf("\n----- Résultats de la recherche -----\n");

    while (fscanf(f, "\"%[^\"]\",%d", ligne.nom, &ligne.nbArrets) == 2) {
        // Lire les arrêts
        for (int i = 0; i < ligne.nbArrets; i++) {
            fscanf(f, ",\"%[^\"]\",", ligne.arrets[i].nom); // Lire le nom de l'arrêt
            for (int j = 0; j < 10; j++) {
                fscanf(f, "%d", &ligne.arrets[i].horaires[j]);
                if (j < 9) {
                    fscanf(f, ";"); // Lire le séparateur
                }
            }
        }

        if (strstr(ligne.nom, critere) != NULL) {
            printf("Nom de la ligne: %s\n", ligne.nom);
            printf("Nombre d'arrêts: %d\n", ligne.nbArrets);
            for (int i = 0; i < ligne.nbArrets; i++) {
                printf("  Arrêt %d: %s\n", i + 1, ligne.arrets[i].nom);
            }
            printf("------------------------\n");
            trouve = 1;
        }
    }

    if (!trouve) {
        printf("Aucune ligne trouvée avec ce critère.\n");
    }

    fclose(f);
}

// Fonction pour rechercher un itinéraire avancé


void rechercherItineraireAvance() {
    FILE *f = fopen("lignes.csv", "r");
    if (f == NULL) {
        printf("Erreur d'ouverture du fichier lignes.csv.\n");
        return;
    }

    char critere[100];
    printf("Entrez le critère de recherche (Nom de l'arrêt): ");
    getchar();  // Consommer le newline restant dans le buffer
    scanf("%[^\n]", critere);  // Lire le critère (nom de l'arrêt)

    Ligne ligne;
    int trouve = 0;

    printf("\n----- Résultats de la recherche avancée -----\n");

    while (fscanf(f, "%99[^;];%d", ligne.nom, &ligne.nbArrets) == 2) {
        for (int i = 0; i < ligne.nbArrets; i++) {
            fscanf(f, ";%99[^;];", ligne.arrets[i].nom);  // Lire le nom de l'arrêt
            for (int j = 0; j < 10; j++) {
                int horaire;
                if (fscanf(f, "%d", &horaire) == 1) {
                    if (horaire != 0) {
                        ligne.arrets[i].horaires[j] = horaire;
                    }
                }
                if (fgetc(f) == ';') { // Lire le séparateur ;
                    continue;
                } else {
                    break;
                }
            }

            // Recherche de l'arrêt correspondant au critère
            if (strstr(ligne.arrets[i].nom, critere) != NULL) {  // Si l'arrêt correspond au critère
                printf("Nom de la ligne: %s\n", ligne.nom);
                printf("Arrêt trouvé: %s\n", ligne.arrets[i].nom);
                printf("Horaires: ");
                int horairesAffiches = 0;
                for (int j = 0; j < 10; j++) {
                    if (ligne.arrets[i].horaires[j] != 0) {
                        if (horairesAffiches > 0) {
                            printf(" ");  // Séparer les horaires par un espace
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
        printf("Aucun itinéraire trouvé avec ce critère.\n");
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
        printf("5. Rechercher un itinéraire\n");
        printf("6. Rechercher un itinéraire avancé\n");
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
                printf("Choix invalide. Veuillez réessayer.\n");
        }
    } while (choix != 0);

    return 0;
}
