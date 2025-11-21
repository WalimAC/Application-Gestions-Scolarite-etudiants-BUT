#include <math.h>
#include <stdio.h>
#include <string.h>

#pragma warning(disable:4996)

// Enumération pour les semestres (S1 à S6)
enum { S1 = 0, S2, S3, S4, S5, S6 };

// Enumération pour les statuts des étudiants
enum {
  VALIDE = 0,      // Semestre valide
  ENCOURS = 1,     // Formation en cours
  DEMISSION = 2,   // Étudiant a démissionne
  DEFAILLANCE = 3, // Étudiant en défaut
  DIPLOME = 4,     // Étudiant diplômé
  AJOURNE = 5      // Étudiant ajourné
};

// Constantes globales
enum {
  MAX_ETUDIANTS = 100, // Nombre maximum d'étudiants
  MAX_TEXT = 31,       // Taille max des chaînes de caracteres
  NB_UE = 6,           // Nombre d'UE par semestre
  NB_SEMESTRES = 6     // Nombre de semestres
};

// Structure pour un semestre
typedef struct {
  float ue[NB_UE];     // Notes des 6 UE
  int status_semestre; // Statut du semestre
} Semestre;

// Structure pour un étudiant
typedef struct {
  char prenom[MAX_TEXT];            // Prenom de l'étudiant
  char nom[MAX_TEXT];               // Nom de l'étudiant
  Semestre semestres[NB_SEMESTRES]; // Tableau des 6 semestres
} Etudiant;

// Structure pour la promotion
typedef struct {
  int nbEtudiants;                   // Nombre d'étudiants actuels
  Etudiant etudiants[MAX_ETUDIANTS]; // Tableau des étudiants
} Promotion;

// FONCTIONS INTERMEDIAIRES

/**
 * Trouve le semestre en cours d'un étudiant
 * @param[in] etu Tableau des étudiants
 * @param[in] i Numero tableau de l'étudiant
 * @return Index du semestre en cours ou 6 si aucun
 */
int find_semestre_encours(const Etudiant etu[], int i) {
  for (int s = 0; s < NB_SEMESTRES; s++) {
    int etu_status = etu[i].semestres[s].status_semestre;
    if (etu_status == ENCOURS || etu_status == DEMISSION || etu_status == DEFAILLANCE || etu_status == AJOURNE || etu_status == DIPLOME) {
      return s;
    }
  }
  return 6; // Aucun semestre en cours
}

/**
 * Vérifie si un étudiant existe déjà (doublon)
 * @param[in] etu Tableau des étudiants
 * @param[in] nbEtudiants Nombre d'étudiants
 * @param[in] prenom Prénom à vérifier
 * @param[in] nom Nom à vérifier
 * @return 1 si doublon, 0 sinon
 */

int verif_doublons(const Etudiant etu[], int *nbEtudiants, char prenom[], char nom[]) {
  for (int i = 0; i < *nbEtudiants; i++) {
    if ((strcmp(etu[i].prenom, prenom) == 0) && (strcmp(etu[i].nom, nom) == 0)) {
      printf("Nom incorrect\n");
      return 1;
    }
  }
  return 0;
}

/**
 * Vérifie la validité d'un ID étudiant
 * @param[in] id ID à vérifier
 * @param[in] nbEtudiants Nombre d'étudiants
 * @return 1 si ID invalide, 0 sinon
 */
int verif_etu_id(int id, int *nbEtudiants) {
  if ((*nbEtudiants < id) || (id > 100 || id < 0)) {
    printf("Identifiant incorrect\n");
    return 1;
  }
  return 0;
}

/**
 * Vérifie si un étudiant est encore en formation
 * @param[in] etu Tableau des étudiants
 * @param[in] id ID de l'étudiant
 * @param[in] sem Semestre à vérifier
 * @return 1 si hors formation, 0 sinon
 */
int verif_etu_encours(const Etudiant etu[], int id, int sem) {
  int etu_status = etu[id - 1].semestres[sem].status_semestre;
  if (etu_status == DEMISSION || etu_status == DEFAILLANCE || etu_status == AJOURNE || etu_status == DIPLOME) {
    printf("Etudiant hors formation\n");
    return 1;
  }
  return 0;
}

/**
 * Vérifie s'il manque des notes pour un semestre donné
 * @param[in] etu Tableau des étudiants
 * @param[in] nbEtudiants Nombre d'étudiants
 * @param[in] semestre Semestre à vérifier
 * @return 1 si notes manquantes, 0 sinon
 */
int etu_note_manquante(const Etudiant etu[], int *nbEtudiants, int semestre) {
  for (int i = 0; i < *nbEtudiants; i++) {
    int sem = find_semestre_encours(etu, i);
    if (sem == semestre - 1) {     // Vérifie si l'étudiant est dans le semestre concerné
      if (etu[i].semestres[sem].status_semestre == ENCOURS) {
        for (int ue = 0; ue < NB_UE; ue++) {    // Vérifie chaque UE du semestre
          if (etu[i].semestres[semestre - 1].ue[ue] == -1) {
            printf("Des notes sont manquantes\n");
            return 1;
          }
        }
      }
    }
  }
  return 0;
}

/**
 * Calcule la moyenne annuelle d'une UE
 * @param[in] etu Tableau des étudiants
 * @param[in] id ID de l'étudiant
 * @param[in] semestre1 Premier semestre
 * @param[in] semestre2 Deuxième semestre
 * @param[in] ue UE concernée
 * @return Moyenne de l'UE
 */

float MoyenneUeAnne(const Etudiant etu[], int id, int semestre1, int semestre2, int ue) {
  float note1 = etu[id - 1].semestres[semestre1].ue[ue];
  float note2 = etu[id - 1].semestres[semestre2].ue[ue];
  return (note1 + note2) / 2;
}

/**
 * Calcule la moyenne avec compensation dans l'année (ADC)
 * @param[in] etu Tableau des étudiants
 * @param[in] sem Semestre actuel
 * @param[in] id ID étudiant
 * @param[in] ue UE concernée
 * @param[in] status_etu Statut de l'étudiant
 * @param[in] s Semestre à calculer
 * @return Moyenne compensée ou 0
 */

float calculMoyenneUeADC(const Etudiant etu[], int sem, int id, int ue, int status_etu, int s) {
  if (((sem > S2) || status_etu == AJOURNE) && (s == S1 || s == S2))   // Compensation pour S1 et S2
    return MoyenneUeAnne(etu, id, S1, S2, ue);
  if (((sem > S4) || status_etu == AJOURNE) && (s == S3 || s == S4))   // Compensation pour S3 et S4
    return MoyenneUeAnne(etu, id, S3, S4, ue);
  if (((sem == S6 && (status_etu == DIPLOME)) || status_etu == AJOURNE) &&   // Compensation pour S5 et S6
      (s == S5 || s == S6))
    return MoyenneUeAnne(etu, id, S5, S6, ue);
  return 0;
}

/**
 * Calcule la moyenne avec compensation sur l'année suivante (ADS)
 * @param[in] etu Tableau des étudiants
 * @param[in] sem Semestre actuel
 * @param[in] id ID étudiant
 * @param[in] ue UE concernée
 * @param[in] status_etu Statut de l'étudiant
 * @param[in] s Semestre à calculer
 * @return Moyenne compensée ou 0
 */
float calculMoyenneUeADS(const Etudiant etu[], int sem, int id, int ue, int status_etu, int s) {
  if (((sem > S4) || status_etu == AJOURNE) && (s == S1 || s == S2))   // Compensation de S1/S2 avec S3/S4
    return MoyenneUeAnne(etu, id, S3, S4, ue);
  if (((sem == S6 && (status_etu == DIPLOME)) || status_etu == AJOURNE) && (s == S3 || s == S4))   // Compensation de S3/S4 avec S5/S6
    return MoyenneUeAnne(etu, id, S5, S6, ue);
  return 0;
}

/**
 * Affiche le statut d'un semestre
 * @param[in] sem Semestre actuel
 * @param[in] status_etu Statut de l'étudiant
 * @param[in] s Semestre à afficher
 */
void affichageStatusSemestre(int sem, int status_etu, int s) {
  if ((status_etu == ENCOURS) && (sem == s)) {
    printf(" en cours");
  } else if ((status_etu == DEMISSION) && (sem == s)) {
    printf(" demission");
  } else if ((status_etu == DEFAILLANCE) && (sem == s)) {
    printf(" defaillance");
  }
  printf("\n");
}

/**
 * Affiche les notes d'un semestre avec les statuts
 * @param[in] etu Tableau des étudiants
 * @param[in] id ID étudiant
 * @param[in] s Semestre à afficher
 * @param[in] sem Semestre actuel
 * @param[in] status_etu Statut de l'étudiant
 */
void affichageNoteSemestre(const Etudiant etu[], int id, int s, int sem, int status_etu) {
  printf("S%d -", s + 1);
  for (int ue = 0; ue < NB_UE; ue++) {
    float MoyenneUE = calculMoyenneUeADC(etu, sem, id, ue, status_etu, s);
    float MoyenneUeAnnee_suivante = calculMoyenneUeADS(etu, sem, id, ue, status_etu, s);
    float note = etu[id - 1].semestres[s].ue[ue];

    if (note == -1) {
      printf(" * (*) -"); // Note manquante
    } else if (note >= 10) {
      printf(" %.1f (ADM) -", floorf(note * 10.f) / 10.f); // Admis
    } else if (note < 10 && MoyenneUE >= 10) {
      printf(" %.1f (ADC) -", floorf(note * 10.f) / 10.f); // Admis avec compensation
    } else if (note < 10 && MoyenneUE < 10 && MoyenneUeAnnee_suivante >= 10) {
      printf(" %.1f (ADS) -", floorf(note * 10.f) / 10.f); // Admis sur année suivante
    } else if (note < 10 && MoyenneUE < 10) {
      printf(" %.1f (AJ) -", floorf(note * 10.f) / 10.f); // Ajourné
    }
  }
  affichageStatusSemestre(sem, status_etu, s);
}

/**
 * Affiche la moyenne annuelle avec décision
 * @param[in] etu Tableau des étudiants
 * @param[in] id ID étudiant
 * @param[in] s Semestre
 * @param[in] sem Semestre actuel
 */
void affichageMoyenneAnnee(const Etudiant etu[], int id, int s, int sem) {
  int status_etu = etu[id - 1].semestres[sem].status_semestre;
  for (int ue = 0; ue < NB_UE; ue++) {
    float moyenne = MoyenneUeAnne(etu, id, s - 1, s, ue);
    float MoyenneUeAnnee_suivante = calculMoyenneUeADS(etu, sem, id, ue, status_etu, s);

    if (moyenne >= 10) {
      printf(" %.1f (ADM) -", floorf(moyenne * 10.f) / 10.f); // Admis
    } else if (moyenne < 10 && moyenne >= 8 && MoyenneUeAnnee_suivante >= 10) {
      printf(" %.1f (ADS) -", floorf(moyenne * 10.f) / 10.f); // Admis sur année suivante
    } else if (moyenne < 10 && moyenne >= 8) {
      printf(" %.1f (AJ) -", floorf(moyenne * 10.f) / 10.f); // Ajourné
    } else if (moyenne < 8) {
      printf(" %.1f (AJB) -", floorf(moyenne * 10.f) / 10.f); // Ajourné bloquant
    }
  }

  if (s == S6 && sem == S6 && status_etu == DIPLOME) {   // Affichage de la décision finale si finis semestre 6
    printf(" diplome");
  } else if (sem == s) {
    printf(" ajourne");
  }
  printf("\n");
}

// FONCTIONS DE COMMANDE

/**
 * INSCRIRE - Inscrit un nouvel étudiant
 * @param[in] etu Tableau des étudiants
 * @param[in] nbEtudiants Pointeur vers le nombre d'étudiants
 * @param[in] prenom Prénom de l'étudiant
 * @param[in] nom Nom de l'étudiant
 */
void inscriptions(Etudiant etu[], int *nbEtudiants, char prenom[], char nom[]) {
  if (verif_doublons(etu, nbEtudiants, prenom, nom) != 0) return;

  strcpy(etu[*nbEtudiants].prenom, prenom);
  strcpy(etu[*nbEtudiants].nom, nom);

  etu[*nbEtudiants].semestres[S1].status_semestre = ENCOURS;

  for (int s = 0; s < NB_SEMESTRES; s++) {
    for (int ue = 0; ue < NB_UE; ue++) {
      etu[*nbEtudiants].semestres[s].ue[ue] = -1;
    }
  }

  *nbEtudiants = *nbEtudiants + 1;
  printf("Inscription enregistree (%d)\n", *nbEtudiants);
}

/**
 * NOTE - Ajoute une note à un étudiant
 * @param[in] etu Tableau des étudiants
 * @param[in] id ID de l'étudiant
 * @param[in] ue UE concernée
 * @param[in] note Note à ajouter
 * @param[in] nbEtudiants Nombre d'étudiants
 */
void note(Etudiant etu[], int id, int ue, float note, int *nbEtudiants) {
  int sem = find_semestre_encours(etu, id - 1);

  if (verif_etu_id(id, nbEtudiants) != 0) return;
  if (verif_etu_encours(etu, id, sem) != 0) return;

  if (ue < 1 || ue > 6) {
    printf("UE incorrecte\n");
    return;
  } else if (note < 0 || note > 20) {
    printf("Note incorrecte\n");
    return;
  }

  etu[id - 1].semestres[sem].ue[ue - 1] = note;
  printf("Note enregistree\n");
}

/**
 * DEMISSION - Enregistre la démission d'un étudiant
 * @param[in] etu Tableau des étudiants
 * @param[in] id ID de l'étudiant
 * @param[in] nbEtudiants Nombre d'étudiants
 */
void demission(Etudiant etu[], int id, int *nbEtudiants) {
  int sem = find_semestre_encours(etu, id - 1);

  if (verif_etu_id(id, nbEtudiants) != 0) return;
  if (verif_etu_encours(etu, id, sem) != 0) return;

  etu[id - 1].semestres[sem].status_semestre = DEMISSION;
  printf("Demission enregistree\n");
}

/**
 * DEFAILLANCE - Enregistre la défailance d'un étudiant
 * @param[in] etu Tableau des étudiants
 * @param[in] id ID de l'étudiant
 * @param[in] nbEtudiants Nombre d'étudiants
 */
void defaillance(Etudiant etu[], int id, int *nbEtudiants) {
  int sem = find_semestre_encours(etu, id - 1);

  if (verif_etu_id(id, nbEtudiants) != 0) return;
  if (verif_etu_encours(etu, id, sem) != 0) return;

  etu[id - 1].semestres[sem].status_semestre = DEFAILLANCE;
  printf("Defaillance enregistree\n");
}

/**
 * ETUDIANTS - Affiche la liste de tous les étudiants
 * @param[in] etu Tableau des étudiants
 * @param[in] nbEtudiants Nombre d'étudiants
 */
void etudiants_list(Etudiant etu[], int *nbEtudiants) {
  for (int i = 0; i < *nbEtudiants; i++) {
    int sem = find_semestre_encours(etu, i);
    int status_etu = etu[i].semestres[sem].status_semestre;

    if (status_etu == ENCOURS) {
      printf("%d - %s %s - S%d - en cours\n", i + 1, etu[i].nom, etu[i].prenom, sem + 1);
    } else if (status_etu == DEMISSION) {
      printf("%d - %s %s - S%d - demission\n", i + 1, etu[i].nom, etu[i].prenom, sem + 1);
    } else if (status_etu == DEFAILLANCE) {
      printf("%d - %s %s - S%d - defaillance\n", i + 1, etu[i].nom, etu[i].prenom, sem + 1);
    } else if (status_etu == AJOURNE) {
      printf("%d - %s %s - S%d - ajourne\n", i + 1, etu[i].nom, etu[i].prenom, sem + 1);
    } else if (status_etu == DIPLOME) {
      printf("%d - %s %s - S%d - diplome\n", i + 1, etu[i].nom, etu[i].prenom, sem + 1);
    }
  }
}

/**
 * CURSUS - Affiche le parcours complet d'un étudiant
 * @param[in] etu Tableau des étudiants
 * @param[in] id ID de l'étudiant
 * @param[in] nbEtudiants Nombre d'étudiants
 */

void cursus(Etudiant etu[], int id, int *nbEtudiants) {
  if (verif_etu_id(id, nbEtudiants) != 0) return;

  printf("%d %s %s\n", id, etu[id - 1].nom, etu[id - 1].prenom);

  int sem = find_semestre_encours(etu, id - 1);
  int status_etu = etu[id - 1].semestres[sem].status_semestre;

  for (int s = 0; s <= sem; s++) {   // Affichage semestre par semestre
    affichageNoteSemestre(etu, id, s, sem, status_etu);

    // Affichage des bilans annee quand disponible
    if (((sem > S2) || status_etu == AJOURNE) && (s == S2)) { 
      printf("B1 -");
      affichageMoyenneAnnee(etu, id, s, sem);
    } else if (((sem > S4) || status_etu == AJOURNE) && (s == S4)) {
      printf("B2 -");
      affichageMoyenneAnnee(etu, id, s, sem);
    } else if (((sem == S6 && (status_etu == DIPLOME)) || status_etu == AJOURNE) && (s == S6)) {
      printf("B3 -");
      affichageMoyenneAnnee(etu, id, s, sem);
    }
  }
}

/**
 * JURY - Traite la fin d'un semestre pour tous les étudiants du semestre si tout le monde a toute les notes
 * @param[in] etu Tableau des étudiants
 * @param[in] semestre Semestre à traiter
 * @param[in] nbEtudiants Nombre d'étudiants
 */
void jury(Etudiant etu[], int semestre, int *nbEtudiants) {
  int nbEtu_End_Sem = 0;

  if (semestre > NB_SEMESTRES || semestre < 1) {
    printf("Semestre incorrect\n");
    return;
  }

  if (etu_note_manquante(etu, nbEtudiants, semestre) != 0) return;

  for (int i = 0; i < *nbEtudiants; i++) {
    int sem = find_semestre_encours(etu, i);

    if (sem == semestre - 1) {     // Si l'étudiant est dans le semestre concerne et en cours
      if (etu[i].semestres[sem].status_semestre == ENCOURS) {
        nbEtu_End_Sem++;

        if (sem == S1 || sem == S3 || sem == S5) {         // Gestion des semestre impairs (S1, S3, S5)
          etu[i].semestres[sem].status_semestre = VALIDE;
          etu[i].semestres[sem + 1].status_semestre = ENCOURS;
        }
        else if (sem == S2 || sem == S4 || sem == S6) {         // Gestion de semestres pairs (S2, S4, S6) et décisions de jury
          int count_moyenne = 0;
          int etu_ajourne = 0;
          int PasPassage3A = 0;
          int PasDiplome = 0;

          // Règles spécifiques pour le passage en 3ème année (S4)
          if (sem == S4) {
            for (int s = 0; s < 2; s++) {
              for (int ue = 0; ue < NB_UE; ue++) {
                float MoyenneB1 = MoyenneUeAnne(etu, i + 1, S1, S2, ue);
                float MoyenneB2 = MoyenneUeAnne(etu, i + 1, S3, S4, ue);
                float note = etu[i].semestres[s].ue[ue];

                // Application des regle de compensation
                if (note >= 10) {
                  if (PasPassage3A != 1)
                    PasPassage3A = 0;
                } else if (note < 10 && MoyenneB1 >= 10) {
                  if (PasPassage3A != 1)
                    PasPassage3A = 0;
                } else if (note < 10 && MoyenneB1 < 10 && MoyenneB2 >= 10) {
                  if (PasPassage3A != 1)
                    PasPassage3A = 0;
                } else if (note < 10 && MoyenneB1 < 10) {
                  PasPassage3A = 1;
                }
              }
            }

            if (PasPassage3A == 1) {
              etu[i].semestres[sem].status_semestre = AJOURNE;
            }
          }

          // Si l'étudiant peut passer en 3A verification des compensations
          if (PasPassage3A != 1) {
            for (int ue = 0; ue < NB_UE; ue++) {
              float note_sem_precedent = etu[i].semestres[sem - 1].ue[ue];
              float note_sem = etu[i].semestres[sem].ue[ue];
              float moyenne = (note_sem_precedent + note_sem) / 2;

              // Comptage des UE entre 8 et 10
              if (moyenne < 10 && moyenne >= 8) {
                count_moyenne++;
              } else if (moyenne < 8) {
                // Échec AJB
                etu[i].semestres[sem].status_semestre = AJOURNE;
                etu_ajourne = 1;
              }
            }

            // Application des regles de compensation
            if (etu_ajourne != 1) {
              if (count_moyenne > 2) {
                // Trop d'UE en AJ
                etu[i].semestres[sem].status_semestre = AJOURNE;
              } else {
                if (sem == S6) {
                  // Vérification de l'obtention du diplôme
                  for (int s = 0; s <= sem; s++) {
                    for (int ue = 0; ue < NB_UE; ue++) {
                      float MoyenneUE;
                      if (s == S1 || s == S2)
                        MoyenneUE = MoyenneUeAnne(etu, i + 1, S1, S2, ue);
                      if (s == S3 || s == S4)
                        MoyenneUE = MoyenneUeAnne(etu, i + 1, S3, S4, ue);
                      if (s == S5 || s == S6)
                        MoyenneUE = MoyenneUeAnne(etu, i + 1, S5, S6, ue);
                      float MoyenneUeAnnee_suivante = calculMoyenneUeADS(
                          etu, sem, i + 1, ue,
                          etu[i].semestres[sem].status_semestre, s);
                      if (s == S3 || s == S4)
                        MoyenneUeAnnee_suivante =
                            MoyenneUeAnne(etu, i + 1, S5, S6, ue);

                      float note = etu[i].semestres[s].ue[ue];

                      // Application des regles de compensation pour le diplome
                      if (note >= 10) {
                        if (PasDiplome != 1)
                          PasDiplome = 0;
                      } else if (note < 10 && MoyenneUE >= 10) {
                        if (PasDiplome != 1)
                          PasDiplome = 0;
                      } else if (note < 10 && MoyenneUE < 10 &&
                                 MoyenneUeAnnee_suivante >= 10) {
                        if (PasDiplome != 1)
                          PasDiplome = 0;
                      } else if (note < 10 && MoyenneUE < 10) {
                        PasDiplome = 1;
                      }
                    }
                  }

                  // Décision finale pour le diplôme
                  if (PasDiplome == 1) {
                    etu[i].semestres[sem].status_semestre = AJOURNE;
                  } else {
                    etu[i].semestres[sem].status_semestre = DIPLOME;
                  }

                } else {
                  // Passage au semestre suivant
                  etu[i].semestres[sem].status_semestre = VALIDE;
                  etu[i].semestres[sem + 1].status_semestre = ENCOURS;
                }
              }
            }
          }
        }
      }
    }
  }
  printf("Semestre termine pour %d etudiant(s)\n", nbEtu_End_Sem);
}

// FONCTION MAIN

/*
  Fonction principale - Gestionnaire de commandes
 */

int main() {
  Promotion p;
  p.nbEtudiants = 0;

  char cmd[MAX_TEXT] = "";
  do {
    scanf("%s", cmd);

    // INSCRIRE - Inscription d'un nouvel étudiant
    if (strcmp(cmd, "INSCRIRE") == 0) {
      char prenom[MAX_TEXT];
      char nom[MAX_TEXT];
      scanf("%s", prenom);
      scanf("%s", nom);
      inscriptions(p.etudiants, &p.nbEtudiants, nom, prenom);
    }
    // NOTE - Ajout d'une note
    else if (strcmp(cmd, "NOTE") == 0) {
      int id;
      int ue;
      float note_entree;
      scanf("%d", &id);
      scanf("%d", &ue);
      scanf("%f", &note_entree);
      note(p.etudiants, id, ue, note_entree, &p.nbEtudiants);
    }
    // CURSUS - Affichage du parcours d'un étudiant
    else if (strcmp(cmd, "CURSUS") == 0) {
      int id;
      scanf("%d", &id);
      cursus(p.etudiants, id, &p.nbEtudiants);
    }
    // ETUDIANTS - Liste de tous les étudiants
    else if (strcmp(cmd, "ETUDIANTS") == 0) {
      etudiants_list(p.etudiants, &p.nbEtudiants);
    }
    // DEMISSION - Enregistrement d'une démission
    else if (strcmp(cmd, "DEMISSION") == 0) {
      int id;
      scanf("%d", &id);
      demission(p.etudiants, id, &p.nbEtudiants);
    }
    // DEFAILLANCE - Enregistrement d'une défaillance
    else if (strcmp(cmd, "DEFAILLANCE") == 0) {
      int id;
      scanf("%d", &id);
      defaillance(p.etudiants, id, &p.nbEtudiants);
    }
    // JURY - Traitement de fin de semestre
    else if (strcmp(cmd, "JURY") == 0) {
      int semestre;
      scanf("%d", &semestre);
      jury(p.etudiants, semestre, &p.nbEtudiants);
    }
    // BILAN - Non implémenté
    else if (strcmp(cmd, "BILAN") == 0) {
      printf("Commande non implémenté");
    }

  } while (strcmp(cmd, "EXIT") != 0); // Sortir du programme

  return 0;
}
