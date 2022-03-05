#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
  char table[3][3];
  int win;
  int countChildren;
  char logic;
  struct Node *next, *child;
} Node;

Node *getNode(char table[][3]);
Node *addNextSibling(Node *here, char table[][3]);
Node *addChild(Node *here, char table[][3]);
void printNodeTable(Node *here, int depth, FILE *f);
void printNodeWin(Node *here, int depth, FILE *f);
Node *readFromFile(char *startChar, char *path);
char isWin(char table[][3]);
void buildPossibleMoves(Node *root, char player, int depth, FILE *f);
void buildTree(Node *root, char initPlayer, char player, int depth);
void copyGameTable(char table[][3], char copyTable[][3]);
void freeNode(Node *here);
int buildTFTree(Node *root, char player, int depth, int nodeType, FILE *f);
void printNodeLogic(Node *root, int depth, FILE *f);
void printLogicTree(Node *root, int depth, FILE *f);
void freeBuildTree(Node *root);

int main(int argc, char *argv[]) {
  if (strcmp(argv[1], "-c1") == 0) {
    FILE *g;
    char startChar;
    g = fopen(argv[3], "w+");
    Node *root = readFromFile(&startChar, argv[2]);
    buildPossibleMoves(root, startChar, 0, g);
    fclose(g);
    freeNode(root);
  }
  if (strcmp(argv[1], "-c2") == 0) {
    FILE *g;
    char startChar;
    g = fopen(argv[3], "w+");
    Node *root = readFromFile(&startChar, argv[2]);
    buildTree(root, startChar, startChar, 0);
    if (buildTFTree(root, startChar, 0, 1, g) == 1)
      root->logic = 'T';
    else
      root->logic = 'F';
    printLogicTree(root, 0, g);
    freeBuildTree(root);
    fclose(g);
  }
  return 0;
}

// functie pentru crearea si initializarea unui nod de arbore
Node *getNode(char table[][3]) {
  Node *newNode = (Node *)malloc(sizeof(Node));
  newNode->next = NULL;
  newNode->child = NULL;
  newNode->countChildren = 0;
  newNode->win = 0;
  int i, j;
  for (i = 0; i < 3; i++)
    for (j = 0; j < 3; j++)
      newNode->table[i][j] = table[i][j];
  return newNode;
}

// functie pentru adaugarea unui vecin al unui nod
Node *addNextSibling(Node *here, char table[][3]) {
  if (here == NULL)
    return NULL;
  while (here->next)
    here = here->next;
  here->next = getNode(table);
  return here->next;
}

// functie pentru adaugarea unui copil
Node *addChild(Node *here, char table[][3]) {
  if (here == NULL)
    return NULL;
  if (here->child)
    return addNextSibling(here, table);
  else {
    here->child = getNode(table);
    here->countChildren += 1;
    return here->child;
  }
}

// functie pentru printarea unei table de joc dintr-un nod
void printNodeTable(Node *here, int depth, FILE *f) {
  int i, j, k;
  for (i = 0; i < 3; i++) {
    for (k = 0; k < depth; k++)
      fprintf(f, "\t");
    for (j = 0; j < 3; j++) {
      if (j != 2)
        fprintf(f, "%c ", here->table[i][j]);
      else
        fprintf(f, "%c", here->table[i][j]);
    }
    fprintf(f, "\n");
  }
}

// functie pentru citirea din fisier a jucatorului si jocului
Node *readFromFile(char *startChar, char *path) {
  FILE *f;
  f = fopen(path, "r");
  char table[3][3];
  int i = 0, j = 0;
  char character;
  *startChar = fgetc(f);
  fgetc(f);
  while ((character = fgetc(f)) != EOF) {
    fgetc(f);
    table[i][j++] = character;
    if (j == 3) {
      j = 0;
      i++;
    }
  }
  Node *root = getNode(table);
  fclose(f);
  return root;
}

// functie pentru verificarea starii de castig a unei table de joc
char isWin(char table[][3]) {
  int i;
  // verificare pe linii si coloane
  for (i = 0; i < 3; i++) {
    if (((table[i][0] != '-') && (table[i][0] == table[i][1]) &&
         (table[i][0] == table[i][2])))
      return table[i][0];
    if (((table[0][i] != '-') && (table[0][i] == table[1][i]) &&
         (table[0][i] == table[2][i])))
      return table[0][i];
  }
  // verificare pe diagonala secundara
  if (((table[0][0] != '-') && (table[0][0] == table[1][1]) &&
       (table[0][0] == table[2][2])))
    return table[0][0];
  // verificare pe diagonala principala
  if (((table[0][2] != '-') && (table[0][2] == table[1][1]) &&
       (table[0][2] == table[2][0])))
    return table[0][2];
  return '-';
}

// functie pentru copierea unei table de joc in alta tabla
void copyGameTable(char table[][3], char copyTable[][3]) {
  int i, j;
  for (i = 0; i < 3; i++)
    for (j = 0; j < 3; j++) {
      copyTable[i][j] = table[i][j];
    }
}

// functie pentru construirea arborelui de joc
void buildPossibleMoves(Node *root, char player, int depth, FILE *f) {
  int i, j;
  // printarea matricei
  printNodeTable(root, depth, f);
  fprintf(f, "\n");
  if (isWin(root->table) != '-')
    return;
  for (i = 0; i < 3; i++)
    for (j = 0; j < 3; j++) {
      if (root->table[i][j] == '-') {
        char copyTable[3][3];
        copyGameTable(root->table, copyTable);
        copyTable[i][j] = player;
        Node *newTable = getNode(copyTable);
        if (player == 'X') {
          buildPossibleMoves(newTable, 'O', depth + 1, f);
          freeNode(newTable);
        } else {
          buildPossibleMoves(newTable, 'X', depth + 1, f);
          freeNode(newTable);
        }
      }
    }
}

// functie pentru eliberarea memoriei unui nod
void freeNode(Node *here) { free(here); }

// functie pentru construirea unui arbore bazat pe starea de castig a nodurilor
void buildTree(Node *root, char initPlayer, char player, int depth) {
  int i, j;
  // verific daca se castiga jocul pe tabla curenta
  if (isWin(root->table) != '-') {
    if (isWin(root->table) == initPlayer)
      root->win = 1;
    else
      root->win = 0;
    return;
  } else {
    root->win = 0;
  }
  for (i = 0; i < 3; i++)
    for (j = 0; j < 3; j++) {
      if (root->table[i][j] == '-') {
        char copyTable[3][3];
        copyGameTable(root->table, copyTable);
        copyTable[i][j] = player;
        Node *newTable;
        // daca exista copil ii adaug vecin
        if (root->child)
          newTable = addNextSibling(root->child, copyTable);
        // adaug copil
        else
          newTable = addChild(root, copyTable);
        if (player == 'X') {
          buildTree(newTable, initPlayer, 'O', depth + 1);
        } else {
          buildTree(newTable, initPlayer, 'X', depth + 1);
        }
      }
    }
}

// functie pentru afisarea unui arbore, bazata pe starea de castig a nodurilor
int buildTFTree(Node *root, char player, int depth, int nodeType, FILE *f) {
  // schimb semnul
  player = (player == 'X') ? 'O' : 'X';
  int i;
  // prin verificarea castigului asociez tip de logica
  if (root->win == 1) {
    root->logic = 'T';
    return 1;
  } else if (!root->child) {
    root->logic = 'F';
    return 0;
  }
  // daca nodul este SAU
  if (nodeType == 1) {
    int ok = 0;
    Node *aux = root->child;
    int i;
    while (aux) {
      if (buildTFTree(aux, player, depth + 1, (-1) * nodeType, f) == 1) {
        ok = 1;
        aux->logic = 'T';
      } else {
        aux->logic = 'F';
      }
      aux = aux->next;
    }
    return ok;
    // daca nodul este SI
  } else {
    Node *aux = root->child;
    int ok = 1;
    int i;
    while (aux) {
      if (buildTFTree(aux, player, depth + 1, (-1) * nodeType, f) == 1) {
        aux->logic = 'T';
      } else {
        ok = 0;
        aux->logic = 'F';
      }
      aux = aux->next;
    }
    return ok;
  }
  return 0;
}

// functie pentru printarea logicii unui nod in functie de adancime
void printNodeWin(Node *here, int depth, FILE *f) {
  int i;
  if (here->win) {
    for (i = 0; i < depth; i++)
      printf("\t");
    printf("T");
  }

  else {
    for (i = 0; i < depth; i++)
      printf("\t");
    printf("F");
  }
}

// functie pentru printarea unui nod in functie de adancime
void printNodeLogic(Node *root, int depth, FILE *f) {
  int i, j, k;
  for (i = 0; i < depth; i++)
    fprintf(f, "\t");
  fprintf(f, "%c", root->logic);
}

// functie pentru printarea logicii nodurilor din arbore
void printLogicTree(Node *root, int depth, FILE *f) {
  printNodeLogic(root, depth, f);
  fprintf(f, "\n");
  Node *aux = root->child;
  while (aux) {
    printLogicTree(aux, depth + 1, f);
    aux = aux->next;
  }
}

// functie pentru eliberarea memoriei arborelui
void freeBuildTree(Node *root) {
  Node *aux = root->child;
  Node *ptr;
  freeNode(root);
  while (aux) {
    ptr = aux->next;
    freeBuildTree(aux);
    aux = ptr;
  }
}
