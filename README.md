====== Game Tree - Minimax Algorithm ======

Structuri folosite:

    Node - structura folosita pentru a memora nodul arborelui
         - se retin in structura: 
             - tabla de joc (matrice de char-uri)
             - starea jocului (if win)
             - numarul de copii al nodului
             - tipul nodului (True/False)
             - pointer la lista de copii si vecinul nodului

TASK 1:

    Functii folosite:
        - readFromFile
        - buildPossibleMoves
        - printNodeTable
        - copyGameTable
    
    Functia readFromFile:
        - functia primeste ca argument adresa caracterului unde se retine jucatorul care incepe si path-ul catre fisier
        - functia citeste dintr-un fisier jucatorul care incepe jocul si matricea care reprezinta tabla de joc
        
    Functia buildPossibleMoves:
        - functia primeste ca argument root-ul arborelui, jucatorul curent, nivelul curent si pointerul catre fisierul in care se scrie
        - se printeaza starea curenta a jocului
        - se shifteaza prin caracterele tablei de joc si se verifica daca este loc liber pentru inca o mutare
        - se face o copie a tablei de joc, se creeaza un nod de arbore asociat tablei curente de joc, apoi se reia recursiv functia, depinzand de jucatorul curent care trebuie sa faca insertia in tabel
        - dupa fiecare insertie se elibereaza memoria asociata nodurilor
    
    Functia printNodeTable:
        - functia primeste ca argument un nod din arbore, adancimea la care se afla si un pointer catre fisier
        - in functie de nivelul la care se afla nodul se printeaza \t 
        - se printeaza matricea si \n dupa fiecare linie a acesteia
    
    Functia copyGameTable:
        - functia primeste ca argument doua matrice (doua table de joc)
        - aceasta copiaza in prima matrice pe cea de a doua
    
    mainTask1:
        - se deschide fisierul in care se scrie
        - se citeste din fisier si se printeaza toate variantele posibile ale jocului folosind buildPossileMoves
        - se inchide fisierul si se elibereaza memoria radacinii

TASK 2:
    
    Functii folosite:
        - readFromFile (analog Task 1)
        - buildTree
        - buildTFTree
        - addNextSibling
        - addChild
        - copyGameTable
        - isWin
        - printLogicTree
        - printNodeWin
        - freeBuildTree
    
    Functia buildTree:
        - functia primeste ca parametru radacina arborelui, jucatorul initial, jucatorul curent si adancimea curenta
        - se verifica daca jocul este castigat de jucatorul initial (jucator ale carui noduri sunt cele SAU) si se aplica eticheta win (1 - castigat , 0 - necastigat)
        - se shifteaza prin caractere tablei curente de joc si se verifica daca nu este gol
        - daca este gol (jucatorul poate insera) se creeaza un copil pentru parintele root (addChild), altfel, daca exista un copil al nodului root se va insera un vecin pentru copil (addNextSibling)
        - se apeleaza recursiv functia, depinzand de jucatorul curent
        
    Functia buildTFTree:
        - functia primeste ca parametru nodul root al arborelui, jucatorul curent, adancimea curenta, tipul nodului (SI/SAU), fisierul in care se scrie
        - se verifica daca nodul curent este o frunza si daca acesta este castigat, se pune eticheta nodului
        - se parcurge recursiv arborele punand etichete pentru fiecare nod, depinzant de tipul nodului curent(SI/SAU) si un flag(ok) care este 1 daca nodul curent are minim un copil
        
    Functia isWin:
        - functia verifica daca tabla de joc este castigata
    
    Functia printLogicTree: 
        - functia primeste ca argument radacina arborelui, adancimea nodului si fisierul in care se scrie
        - se scrie in fisier logica nodului, depinzand de adancimea nodului curent (se printeaza \t)
    
    Functia freeBuildTree:
        - functia primeste ca argument radacina arborelui
        - se parcurge recursiv arborele si se elibereaza memoria alocata nodurilor

    mainTask2:
        - se deschide fisierul de scriere
        - se citeste din fisierul de intrare
        - se construieste arborele recursiv folosind logica(T/F)
        - se atribuie si radacinii logica
        - se printeaza logica arborelui
