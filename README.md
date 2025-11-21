<div align="center">
  <a href="https://iutparis-seine.u-paris.fr">
    <img src="https://medias.podcastics.com/podcastics/podcasts/artwork/universite-paris-cite.png.23d93fd89c820e5e702963c782b2214f.png" alt="Logo" width="80" height="80">
  </a>

  <h3 align="center">Programme de gestion des étudiants - BUT Informatique</h3>

  <p align="center">
    Application en ligne de commande permettant de gérer la scolarité complète des étudiants du BUT Informatique.
    <br>
    <i>⚠️ | Projet universitaire ! Ne reflète pas totalement le système réel de notation d’un BUT.</i>
    <br />
    <br />
    <a href="https://wamor.free.nf/?i=1#contact">Signaler un bug</a>
  </p>
</div>

## À propose du projet

Cette application console permet de gérer la scolarité d’étudiants d’un Bachelor Universitaire de Technologie (BUT).  
Elle a été développée dans le cadre d’un projet universitaire en première année (SAE 1.01) - Novembre 2025.

⛔️ | **Limitations du projet :**
- Le calcul automatique des UE n’est pas intégré (elles doivent être ajoutées manuellement).  
- La logique de 3ᵉ année n’est pas totalement conforme (ex : seulement 3 UE en réalité, contre 6 dans les deux premières années).
- Le code est contenu dans un seul et même fichier, comme cela avait été demandé.

### Fonctionnalités principales
- Inscription des étudiants  
- Gestion des notes  
- Suivi du cursus (moyennes, validation de semestres, ajournement, diplome)  
- Gestion des décisions de jury (Calcul des compensations & passage admis ou refusé)
  
[Voir un exemple en image](https://media.discordapp.net/attachments/1441496793635360808/1441496872748322846/Capture_decran_2025-11-21_a_18.37.51.png?ex=69220214&is=6920b094&hm=c32aa72b92a3c78e60e6ce082b1593f858f39c102010a64e6ecfcdb1cc825b08&=&format=webp&quality=lossless&width=1542&height=1544)

---

### Commandes & Utilisations

- `INSCRIRE <prenom> <nom>` — Ajoute un étudiant (avec vérification de doublon) et génère un ID unique.
- `NOTE <id> <ue> <note>` — Ajouter/modifier une note dans une UE  
- `CURSUS <id>` — Afficher le dossier complet d’un étudiant (Note, Moyenne, Compensations, Status Demission/Defaillance, Diplome, Ajournée)
- `DEMISSION <id>` — Verrouiller un dossier pour cause de démission  
- `DEFAILLANCE <id>` — Verrouiller un dossier pour cause de défaillance  
- `JURY <Semestre>` — Effectuer les décisions de jury pour les étudiants inscrit au semestre donner
- `ETUDIANTS` — Voir tous les étudiants + statut + semestre  
- `EXIT` — Quitter le programme

💡 | *Le programme peut être amélioré avec un système de saisie guidée (phrases, menus, etc.).  
Cependant, ce projet devait respecter des entrées/sorties strictes pour la correction automatique.*

---

### Language Utilisé & IDE

<img src="https://upload.wikimedia.org/wikipedia/commons/1/19/C_Logo.png" alt="Logo C" width="40" height="40">
<img src="https://upload.wikimedia.org/wikipedia/commons/thumb/9/9a/Visual_Studio_Code_1.35_icon.svg/2048px-Visual_Studio_Code_1.35_icon.svg.png" alt="Logo VScode" width="40" height="40">
