#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const int datalength = 7;
int c = 0;
int ssize = 1;
int st = 0;
double hyptime = 0.0;
double btime = 0.0;

//structure

//a node of the tree

struct node {
  int Known[7];
  int nbrK;
  float b1;
  float b2;
};

//the tree, as a stack

struct todo {
  struct node act;
  struct todo *next;
};

//a multi-obj solution (2 obj)

struct solution {
  float s1;
  float s2;
  struct node n;
};

//a group of solution

struct solutions {
  struct solution s;
  struct solutions *next;
};

//create a random matrice of float between 0 and 1

void datacreator(float m[datalength][datalength]){
  int i,j;
  for (i = 0; i < datalength; i++){
    for (j = 0; j < datalength; j++){
      m[i][j] = ((float)  ((rand() % 100)))/100;
    }
  }       
}

//printers

void printnode(struct node n){
  int i;
  printf("ubf: %f %f ,,, they are %d variable fixed on this node\nthese variables are:",n.b1,n.b2,n.nbrK);
  for (i = 0; i < datalength;i++){
  printf("%d ",n.Known[i]);
  }
  printf("\n");
}

void printnodes(struct todo *t){
  struct todo *tn = t;
  while (tn != NULL){
    printnode(tn -> act);
    tn = tn->next;
  }
}


void printsolution(struct solution sol){
  printf("solution: f1: %f f2 %f\n",sol.s1,sol.s2);
  int i;
  for ( i = 0; i < datalength; i++){
    //printf("%d ",(sol.n).Known[i]);
  }
  //printf("\n");
}

void printsolutions(struct solutions *sol){
  struct solutions *sl = sol;
  while (sl != NULL){
    printsolution(sl->s);
    sl = sl->next;
  }
}

void printdata(float m[datalength][datalength]){
  int i,j;
  for (i = 0; i < datalength ; i++) {
    for (j = 0; j< datalength; j++) {
      printf("tab[%d][%i] = %f ",i,j,m[i][j]);
    }
    printf("\n");
  }
}




//true if the variable c isnt fixed yet, false otherwise

int notK (int tab[], int c){
  int i;
  for (i = 0; i < datalength; i++) {
    if (tab[i] == (int) NULL) {}
    else if (tab[i] == c) {
      return 0;
    }
  }
  return 1;
}

struct solutions *copy(struct solutions *start1)
{
    if(start1==NULL) return NULL;
    struct solutions *temp=(struct solutions *) malloc(sizeof(struct solutions));
    temp->s=start1->s;
    temp->next=copy(start1->next);
    return temp;
}

void freecp(struct solutions *cp){
  if (cp == NULL)
    return;
  else {
    freecp(cp->next);
    free(cp);
  }
}


  
testcopy(){
  struct solution sol4b;
  struct solutions sols4b;
  sol4b.s1 = 0.3;
  sol4b.s2 = 0.7;
  sols4b.s = sol4b;
  sols4b.next = NULL;
  struct solution sol3b;
  struct solutions sols3b;
  sol3b.s1 = 1.;
  sol3b.s2 = 4.;
  sols3b.s = sol3b;
  sols3b.next = &sols4b;
  struct solution sol2b;
  struct solutions sols2b;
  sol2b.s1 = 3.;
  sol2b.s2 = 3.;
  sols2b.s = sol2b;
  sols2b.next = &sols3b;
  struct solution sol1b;
  struct solutions sols1b;
  sol1b.s1 = 4.;
  sol1b.s2 = 1.;
  sols1b.next = &sols2b;
  sols1b.s = sol1b;
  struct solutions *new = copy(&sols1b);
  (new->next)->next = NULL;
  printf("--new--\n");
  printsolutions(new);
  printf("--old--\n");
  printsolutions(&sols1b);
}

int nodeEquals(struct node n1, struct node n2){
  int i;
  if (n1.nbrK != n2.nbrK)
    return 0;
  else {
    for (i = 0; i < n1.nbrK;i++) {
      if (n1.Known[i] != n2.Known[i])
	return 0;
    }
    return 1;
  }
}

int sizesol(struct solutions *s){
  if (s == NULL)
    return 0;
  else
    return 1+ sizesol(s->next);
}

struct todo *extract(struct node n,struct todo *ns){
  struct todo *fr;
  if (ns->next ==  NULL){
    return NULL;
    }
  if (nodeEquals(n, ns->act)){
    ns->act = (ns->next)->act;
    ns->next = (ns->next)->next;
     return ns;
  }
  struct todo *temp = ns;
  while(temp->next != NULL && !(nodeEquals(n,(temp->next)->act))){
    temp = temp->next;
  }
  if (temp->next == NULL)
    return ns;
  fr = temp->next;
  temp->next = (temp->next)->next;
  free(fr);
  return ns;
}

void testextract(){
  struct node n4 = {{2,0,0,0},3};
  struct todo s4;
  s4.act = n4;
  s4.next = NULL;
  struct node n3 = {{3,0,0,0},1};
  struct todo s3;
  s3.act = n3;
  s3.next = &s4;
  struct node n2 = {{1,0,0,0},1};
  struct todo s2;
  s2.act = n2;
  s2.next = &s3;
  struct node n1 = {{4,2,0,0},2};
  struct todo s1;
  s1.act = n1;
  s1.next = &s2;
  struct todo *t = &s1;
  t = extract(n3,t);
  t = extract(n2,t);
  t = extract(n1,t);
  t = extract(n4,t);
  printnodes(t);
}

int cmpfunc (const void * a, const void *b)
{
  float res = 0;
  res= (*(struct solution*)b).s1 - (*(struct solution*)a).s1;
  if (res == 0.)
    return 0;
  if (res > 0.)
    return 1;
  return -1;
}

void testcmpfunc(){
  int res = 0;
  struct solution sa;
  struct solution sb;
  sa.s1 = 1.;
  sa.s2 = 10.;
  sb.s1 = 0.5;
  sb.s2 = 20.;
  res = cmpfunc((void*) &sa, (void *) &sb);
  printf("res: %d \n",res);
}

float hypervolume(struct solutions *sls){
  int sizestruct = 0;
  struct solutions *tete = sls;
  while (sls != NULL){
    sizestruct++;
    sls = sls->next;
  }
  struct solution tab[sizestruct];
  int i;
  for (i = 0; i < sizestruct;i++){
    //printsolution(tete->s);
    tab[i] = tete->s;
    tete = tete->next;
  }
  qsort(tab,sizestruct, sizeof(struct solution),cmpfunc);
  float hypervol = 0.;
  float minus = 0.;
  for (i = 0; i < sizestruct; i++) {
    hypervol = hypervol + ((tab[i]).s1 * ((tab[i]).s2 - minus));
    minus = (tab[i]).s2;
    //printf("hyp: %f minus: %f\n",hypervol,minus);
  }
  //printf("hyp: %f minus: %f\n",hypervol,minus);
  free(tete);
  return hypervol;
}

void testhypervolume(){
  struct solution sol4b;
  struct solutions sols4b;
  sol4b.s1 = 0.3;
  sol4b.s2 = 0.7;
  sols4b.s = sol4b;
  sols4b.next = NULL;
  struct solution sol3b;
  struct solutions sols3b;
  sol3b.s1 = 1.;
  sol3b.s2 = 4.;
  sols3b.s = sol3b;
  sols3b.next = NULL;
  struct solution sol2b;
  struct solutions sols2b;
  sol2b.s1 = 3.;
  sol2b.s2 = 3.;
  sols2b.s = sol2b;
  sols2b.next = &sols3b;
  struct solution sol1b;
  struct solutions sols1b;
  sol1b.s1 = 4.;
  sol1b.s2 = 1.;
  sols1b.next = &sols2b;
  sols1b.s = sol1b;
  printf("%f\n",hypervolume(&sols1b));
}
  


//return the best potential solution from a node and a matrice of data

float UBF (struct node *n, float m[datalength][datalength]) {
  int i,ib,j;
  float inter;
  float res = 1.0;
  int kn = (*n).nbrK;
  int *known = (*n).Known;
  for (i = 0; i < kn; i++){
    res = res * (m[known[i]-1][i]);
  }
  //printf("après known i: %d res : %f\n",i,res);
  while (i < datalength) {
    //printf("debut i: %d\n",i);
    inter = 0.;
    for (j = 1; j <(datalength+1); j++) {
      if ((inter <= m[j-1][i]) && (notK(known,j))) {
	inter = m[j-1][i];
      }
    }
    // printf("ajout val : %f\n",inter);
    res = res * inter;
    i++;
  }
  return res;
}

void testUBF(){
    static float members4[4][4];
  members4[0][0] = 0.9;
  members4[0][1] = 0.8;
  members4[0][2] = 0.9;
  members4[0][3] = 0.85;
  members4[1][0] = 0.7;
  members4[1][1] = 1.;
  members4[1][2] = 0.8;
  members4[1][3] = 0.7;
  members4[2][0] = 0.85;
  members4[2][1] = 0.7;
  members4[2][2] = 0.85;
  members4[2][3] = 0.8;
  members4[3][0] = 0.75;
  members4[3][1] = 0.7;
  members4[3][2] = 0.75;
  members4[3][3] = 0.7;
  static float members3[4][4];
  members3[0][0] = 1.;
  members3[0][1] = 1.;
  members3[0][2] = 1.;
  members3[0][3] = 1.;
  members3[1][0] = 1.;
  members3[1][1] = 1.;
  members3[1][2] = 1.;
  members3[1][3] = 1.;
  members3[2][0] = 1.;
  members3[2][1] = 1.;
  members3[2][2] = 1.;
  members3[2][3] = 1.;
  members3[3][0] = 1.;
  members3[3][1] = 1.;
  members3[3][2] = 1.;
  members3[3][3] = 1.;
  static float members2[4][4];
  members2[0][0] = 0.9;
  members2[0][1] = 0.8;
  members2[0][2] = 0.9;
  members2[0][3] = 0.85;
  members2[1][0] = 0.7;
  members2[1][1] = 0.6;
  members2[1][2] = 0.8;
  members2[1][3] = 0.7;
  members2[2][0] = 0.85;
  members2[2][1] = 0.7;
  members2[2][2] = 0.85;
  members2[2][3] = 0.8;
  members2[3][0] = 0.75;
  members2[3][1] = 0.7;
  members2[3][2] = 0.75;
  members2[3][3] = 0.7;
  static float members[4][4];
  members[0][0] = 0.9;
  members[0][1] = 0.8;
  members[0][2] = 0.9;
  members[0][3] = 0.85;
  members[1][0] = 0.7;
  members[1][1] = 0.6;
  members[1][2] = 0.8;
  members[1][3] = 0.7;
  members[2][0] = 0.85;
  members[2][1] = 0.7;
  members[2][2] = 0.85;
  members[2][3] = 0.8;
  members[3][0] = 0.75;
  members[3][1] = 0.7;
  members[3][2] = 0.75;
  members[3][3] = 0.7;
  struct node n = {{3,0,0,0},1};
  printf("ubf: %f\n",UBF(&n,members));
}

//1 if the newsol dominate the old one, -1

int dominate(struct solution newsol,struct solution oldsol){
  if (((newsol.s1 > oldsol.s1) && (newsol.s2 >= oldsol.s2)) || ((newsol.s1 >= oldsol.s1) && (newsol.s2 > oldsol.s2)))
    return 1;
  else if (((newsol.s1 <= oldsol.s1) && (newsol.s2 < oldsol.s2)) ||((newsol.s1 < oldsol.s1) && (newsol.s2 <= oldsol.s2)))
    return -1;
  else
    return 0;
}

void testdominate(){
  struct solution sol1;
  struct solution sol2;
  sol1.s1 = 0.4;
  sol1.s2 = 0.4;
  sol2.s1 = 0.4;
  sol2.s2 = 0.4;
  printf("dominate: %d\n",dominate(sol1,sol2));
}

//check if a node has a potential

int potential(struct solution sol, struct solutions *data){
  struct solutions *sls = malloc (sizeof (struct solutions));
  *sls = *data;
  while (sls !=NULL){
    if (dominate(sol,sls->s) == -1){
      return 0;
      //free(sls);
      }
    sls = sls->next;
  }
  //free(sls);
  return 1;
}

void testPotential(){
struct solution sol4b;
  struct solutions sols4b;
  sol4b.s1 = 0.8;
  sol4b.s2 = 0.3;
  sols4b.s = sol4b;
  sols4b.next = NULL;
  struct solution sol3b;
  struct solutions sols3b;
  sol3b.s1 = 0.1;
  sol3b.s2 = 0.1;
  sols3b.s = sol3b;
  sols3b.next = &sols4b;
  struct solution sol2b;
  struct solutions sols2b;
  sol2b.s1 = 0.1;
  sol2b.s2 = 0.1;
  sols2b.s = sol2b;
  sols2b.next = &sols3b;
  struct solution sol1b;
  struct solutions sols1b;
  sol1b.s1 = 0.2;
  sol1b.s2 = 0.8;
  sols1b.next = &sols2b;
  sols1b.s = sol1b;
  struct solution toadd;
  toadd.s1 = 0.5;
  toadd.s2 = 0.5;
}

void hasPotential(struct solution toadd, struct solutions *data){
  int *changed = malloc(sizeof(int));
  struct solutions *sls = malloc(sizeof (struct solutions));
  struct solutions *res1 = malloc(sizeof (struct solutions));
  struct solutions *fr;
  res1->next = NULL;
  res1->s = toadd;
  *sls = *data;
  // on clean jusqu'à avoir retiré tout les dominés en tête
  while ((sls != NULL) && (dominate(toadd,sls->s)==1)){
    if (sls->next != NULL){
      fr = sls;
      *sls = *(sls->next);
      free(fr);
      fr = data;
      *data = *(data->next);
      free(fr);
    }
    else {
      sls = NULL;
      //data =NULL;
    }
    *changed = 1;
  }
  //si tout est dominé, on renvoie uniquement la nouvelle solution
  if (sls == NULL){
    *changed =1;
    *data = *res1;
    return;
  }
  if (dominate(toadd,sls->s) == -1){
    return;
  }

  ///////////////////////////////////////////////////////////////
  while ((sls->next != NULL) && (dominate(toadd,(sls->next)->s) == 1)){
    if ((sls->next)->next == NULL) {
      data->next = res1;
      return;
    }
    data->next =(data->next)->next;
    sls->next = (sls->next)->next;
  }
  //////////////////////////////////////////////////////////////
  if (sls->next != NULL){
    if(dominate(toadd,(sls->next)->s) == 1){
      if ((sls->next)->next == NULL) {
	data->next = res1;
	return;
      }
      data->next = (data->next)->next;
      sls->next = (sls->next)->next;
    }
    else if (dominate(toadd,(sls->next)->s) == -1){
      return;
    }
    else
      sls = sls->next;
    ///////////////////////////////////////////////////////////////
  }
  while (sls->next != NULL) {
     
    //si domine
    if (dominate(toadd,(sls->next)->s) == 1){
      *changed = 1;
      if ((sls->next)->next == NULL) {
	sls->next = res1;
	return;
      }
      sls->next = (sls->next)->next;
    }

     
    //si dominé
    else if (dominate(toadd,(sls->next)->s) == -1){
      return;
    }

     
    //si neutre
    else
      sls = sls->next;
  }
  res1->next = data->next;
  data->next = res1;
  return;
}

//test

void testhasPotential(){
   struct solution sol4b;
  struct solutions sols4b;
  sol4b.s1 = 0.8;
  sol4b.s2 = 0.3;
  sols4b.s = sol4b;
  sols4b.next = NULL;
  struct solution sol3b;
  struct solutions sols3b;
  sol3b.s1 = 0.2;
  sol3b.s2 = 0.25;
  sols3b.s = sol3b;
  sols3b.next = &sols4b;
  struct solution sol2b;
  struct solutions sols2b;
  sol2b.s1 = 0.2;
  sol2b.s2 = 0.2;
  sols2b.s = sol2b;
  sols2b.next = &sols3b;
  struct solution sol1b;
  struct solutions sols1b;
  sol1b.s1 = 0.3;
  sol1b.s2 = 0.6;
  sols1b.next = &sols2b;
  sols1b.s = sol1b;
  struct solution add;
  add.s1 = 0.45;
  add.s2 = 0.45;
  int changed = 0;
  printsolutions(&sols1b);
  printf("\naprès\n");
  hasPotential(add,&sols1b);
  printsolutions(&sols1b);
}

//fullfile kn depending on weither something is available or not

void dispo (int disp[datalength],int kn[datalength]){
  int i,b;
  for (i = 0; i<datalength; i++) {
    if (!(kn[i] == 0)){
     disp[kn[i]-1] =0;
    }
  }
}


void CHILDREN(struct node *n,struct node *children,float m[datalength][datalength],float m2[datalength][datalength]) {
  //printf("children\n");
  int i,j,z;
  int kn = (*n).nbrK;
  int disp[datalength];
  //mesure du temps de calcul des bornes
  double pretime = 0.0;
  double toadd = 0.0;
  for (z = 0; z < datalength; z++){
    disp[z] = 1;
  }
  dispo(disp,(*n).Known);
  for (i = 0; i <datalength -(kn);i++) {
    for (j = 0; j < kn ; j++){
      (children[i]).Known[j] = (*n).Known[j];
    }
    for (j = kn; j < datalength ; j++){
      (children[i]).Known[j] = (int) NULL;
      (children[i]).nbrK = kn+1;
    }
  }
  for (j = 0; j <(datalength - kn); j++){
    for (i = 0; i < datalength; i++) {
      if (disp[i]){
	(children[j]).Known[kn] = i+1;
	disp[i] = 0;
	break;
      }
    }
  }
  for (i = 0; i <datalength -kn;i++){
    c++;
    pretime = (double) clock();
    children[i].b1 = UBF(&children[i],m);
    children[i].b2 = UBF(&children[i],m2);
    toadd = ((double) clock()) - pretime;
    btime = btime +toadd;
    
}
}

//test
 
/*
void testchildren(){
  struct node n ={{1,3,0,0},2};
  struct node c[3];
  CHILDREN(&n,c);
  printnode(c[0]);
  printnode(c[1]);
  //printnode(c[2]);
  
  }*/

//unstack the tree until we get a potential better solution/node, then add the solution or the new node to the tree


struct todo *newheadb(struct todo *old,struct solutions *ActBest,float m[datalength][datalength],float m2[datalength][datalength]){
  struct todo *head = old;
  struct todo *temp;
  struct solutions *cp,*cp2;
  struct solution headsol,bestsol;
  float besthyper = 0;
  float temphyper = 0;
  double prehyp,hypt;
  int potent,i,size;
  int found = 0;
  cp2 = copy(ActBest);
  if (sizesol(cp2) != ssize){
    ssize = sizesol(cp2);
    st++;
    //printf("%d %d\n",st,c);
  }
  free(cp2);
  //totab(ActBest);
  //printf("\n");
  while (head != NULL){
    //printf("head:\n");
    //printnodes(head);
    //printf("\nold:\n");
    //printnodes(old);
    //printf("\n\n");
    headsol.s1 = (head->act).b1;
    headsol.s2 = (head->act).b2;
    headsol.n = (head->act);
    potent = potential(headsol,ActBest);
    //debut dernier cas
    if (head->next == NULL){
      if (!potent){
	old = extract(head->act,old);
	head = NULL;
	continue;
      }
      else {
	found =1;
        prehyp = (double) clock();
	cp = copy(ActBest);
	hasPotential(headsol,cp);
	temphyper = hypervolume(cp);
	hypt = ((double) clock()) - prehyp;
	freecp(cp);
	hyptime = hyptime+hypt;
	if (temphyper > besthyper){
	  besthyper = temphyper;
	  bestsol = headsol;
	  size = (head->act).nbrK;
	}
	head = NULL;
	continue;
      }
    }
    //fin dernier cas, cas general
    if (!potent){
      temp = head;
      head = head->next;
      old = extract(temp->act,old);
      
      continue;
    }
    else{
      //printf("test\n");
      found = 1;
      prehyp = (double) clock();
      cp = copy(ActBest);
      hasPotential(headsol,cp);
      temphyper = hypervolume(cp);
      freecp(cp);
      hypt = ((double) clock()) - prehyp;
      hyptime = hyptime+hypt;
      if (temphyper > besthyper){
	besthyper = temphyper;
	bestsol = headsol;
	size = (head->act).nbrK;
      }
      head = head->next;
    }
  }
  //printf("fin première boucle\n");
  //printnodes(head);
  //printnode(bestsol.n);
  if (found == 0)
    return NULL;
  if(size == datalength){
    hasPotential(bestsol,ActBest);
    old = extract(bestsol.n,old);
    return old;
  }
  else  {
    old = extract(bestsol.n,old);
    //printf("newhead:entering childs size: %d\n",size);
    struct node ch[datalength-size];
    CHILDREN(&(bestsol.n),ch,m,m2);
    //printf("done1\n");
    struct todo *todochilds[datalength-size];
    for (i = 0; i < (datalength-size);i++){
      todochilds[i] = malloc (sizeof (struct todo));
      todochilds[i]->act = ch[i];
    }
    for (i = 0; i< ((datalength-1)-(size));i++){
      todochilds[i]->next = todochilds[i+1];
    }
    todochilds[i]->next = old;
    return todochilds[0];
  }
}

void testnewheadb(){
  float m[datalength][datalength];
  float m2[datalength][datalength];
  datacreator(m);
  datacreator(m2);
  //printdata(m);
  //printdata(m2);
static float members4[4][4];
  members4[0][0] = 0.9;
  members4[0][1] = 0.8;
  members4[0][2] = 0.9;
  members4[0][3] = 0.85;
  members4[1][0] = 0.7;
  members4[1][1] = 1.;
  members4[1][2] = 0.8;
  members4[1][3] = 0.7;
  members4[2][0] = 0.85;
  members4[2][1] = 0.7;
  members4[2][2] = 0.85;
  members4[2][3] = 0.8;
  members4[3][0] = 0.75;
  members4[3][1] = 0.7;
  members4[3][2] = 0.75;
  members4[3][3] = 0.7;
  static float members[4][4];
  members[0][0] = 0.9;
  members[0][1] = 0.8;
  members[0][2] = 0.9;
  members[0][3] = 0.85;
  members[1][0] = 0.7;
  members[1][1] = 0.6;
  members[1][2] = 0.8;
  members[1][3] = 0.7;
  members[2][0] = 0.85;
  members[2][1] = 0.7;
  members[2][2] = 0.85;
  members[2][3] = 0.8;
  members[3][0] = 0.75;
  members[3][1] = 0.7;
  members[3][2] = 0.75;
  members[3][3] = 0.7;
  struct solution sol4b;
  struct solutions sols4b;
  sol4b.s1 = 0.1;
  sol4b.s2 = 0.9;
  sols4b.s = sol4b;
  sols4b.next = NULL;
  struct solution sol3b;
  struct solutions sols3b;
  sol3b.s1 = 0.9;
  sol3b.s2 = 0.1;
  sols3b.s = sol3b;
  sols3b.next = &sols4b;
  struct solution sol2b;
  struct solutions sols2b;
  sol2b.s1 = 0.3251;
  sol2b.s2 = 0.541875;
  sols2b.s = sol2b;
  sols2b.next = &sols3b;
  struct solution sol1b;
  struct solutions sols1b;
  sol1b.s1 = 0.0;
  sol1b.s2 = 0.0;
  sols1b.next = NULL;
  sols1b.s = sol1b;
  struct node startingnode = {{2,1,0,0},2,UBF(&startingnode,members),UBF(&startingnode,members4)};
  struct node startingnode2 = {{4,2,0,0},2,UBF(&startingnode2,members),UBF(&startingnode2,members4)};
  struct node startingnode3 = {{3,1,0,0},2,UBF(&startingnode3,members),UBF(&startingnode3,members4)};
  struct node startingnode4 = {{1,2,0,0},2,UBF(&startingnode4,members),UBF(&startingnode3,members4)};
  struct todo stack;
  struct todo stack2;
  struct todo stack3;
  struct todo stack4;
  stack4.next = NULL;
  stack4.act = startingnode4;
  stack3.next = &stack4;
  stack3.act = startingnode3;
  stack2.next = &stack3;
  stack2.act = startingnode2;
  stack.next = &stack2;;
  stack.act = startingnode;
  struct todo *newh= newheadb(&stack,&sols1b,members,members4);
  printf("retour:\n");
  printnodes(newh);
  printsolutions(&sols1b);
}


void final(struct todo *start,float m[datalength][datalength],float m2[datalength][datalength],struct solutions *actbest){
  //printf("\n\n--------FINAL-------\n\n");
  struct todo *head = start;
  int i = 0;
  double totaltime =0.0;
  double d = 0.0;
  double iterationtime = 0.0;
  while (head != NULL){
    //printf("\n\n");
    //printnodes(head);
    //printsolutions(actbest);
    head = newheadb(head,actbest,m,m2);
    i++;
    d = totaltime;
    totaltime = (double) clock();
    iterationtime = totaltime - d;
  }
  //printf("nombre d'iterations: %d\n",i);
  //printf("%d %d \n",ssize,c);
  printf("%d %f\n",datalength,(btime/totaltime));
  //printf("clock per s: %d\n",CLOCKS_PER_SEC);
  //printf("total time (en s): %f\n",(totaltime / (float) CLOCKS_PER_SEC));
}

void totab(struct solutions *sol){
   int sizestruct = 0;
  struct solutions *tete = sol;
  while (sol != NULL){
    sizestruct++;
    sol = sol->next;
  }
  struct solution tab[sizestruct];
  int i;
  for (i = 0; i < sizestruct;i++){
    tab[i] = tete->s;
    tete = tete->next;
  }
  qsort(tab,sizestruct, sizeof(struct solution),cmpfunc);
  for (i = 0; i<sizestruct;i++){
    printsolution(tab[i]);
  }
}


int main(int argc, char* argv[]){
  //testextract();
  //testnewheadb();
  //printf("c: %d\n",c);
  //srand ((unsigned int)atoi(argv[1]));
  float m[datalength][datalength];
  float m2[datalength][datalength];
  datacreator(m);
  datacreator(m2);
  //printdata(m);
  //printdata(m2);
  static float members4[4][4];
  members4[0][0] = 0.9;
  members4[0][1] = 0.8;
  members4[0][2] = 0.9;
  members4[0][3] = 0.85;
  members4[1][0] = 0.7;
  members4[1][1] = 1.;
  members4[1][2] = 0.8;
  members4[1][3] = 0.7;
  members4[2][0] = 0.85;
  members4[2][1] = 0.7;
  members4[2][2] = 0.85;
  members4[2][3] = 0.8;
  members4[3][0] = 0.75;
  members4[3][1] = 0.7;
  members4[3][2] = 0.75;
  members4[3][3] = 0.7;
  static float members3[4][4];
  members3[0][0] = 1.;
  members3[0][1] = 1.;
  members3[0][2] = 1.;
  members3[0][3] = 1.;
  members3[1][0] = 1.;
  members3[1][1] = 1.;
  members3[1][2] = 1.;
  members3[1][3] = 1.;
  members3[2][0] = 1.;
  members3[2][1] = 1.;
  members3[2][2] = 1.;
  members3[2][3] = 1.;
  members3[3][0] = 1.;
  members3[3][1] = 1.;
  members3[3][2] = 1.;
  members3[3][3] = 1.;
  static float members2[4][4];
  members2[0][0] = 0.9;
  members2[0][1] = 0.8;
  members2[0][2] = 0.9;
  members2[0][3] = 0.85;
  members2[1][0] = 0.7;
  members2[1][1] = 0.6;
  members2[1][2] = 0.8;
  members2[1][3] = 0.7;
  members2[2][0] = 0.85;
  members2[2][1] = 0.7;
  members2[2][2] = 0.85;
  members2[2][3] = 0.8;
  members2[3][0] = 0.75;
  members2[3][1] = 0.7;
  members2[3][2] = 0.75;
  members2[3][3] = 0.7;
  static float members[4][4];
  members[0][0] = 0.9;
  members[0][1] = 0.8;
  members[0][2] = 0.9;
  members[0][3] = 0.85;
  members[1][0] = 0.7;
  members[1][1] = 0.6;
  members[1][2] = 0.8;
  members[1][3] = 0.7;
  members[2][0] = 0.85;
  members[2][1] = 0.7;
  members[2][2] = 0.85;
  members[2][3] = 0.8;
  members[3][0] = 0.75;
  members[3][1] = 0.7;
  members[3][2] = 0.75;
  members[3][3] = 0.7;
  struct node testnode = {{1,2,0,0,0,0},2,UBF(&testnode,m),1};
  //printf("%f %f %f %f %f %f\n",m[0][0],m[1][1],m[2][2],m[3][3],m[4][4],m[5][5]);
  // struct node startingnode = {{0,0,0,0},0,UBF(&startingnode,members),UBF(&startingnode,members4)};
  struct node startingnode = {{0,0},0,UBF(&startingnode,m),UBF(&startingnode,m2)};
   struct todo stack;
  stack.act = startingnode;
  stack.next = NULL;
  struct solutions sols;
  sols.next = NULL;
  (sols.s).s1 = 0.0;
  (sols.s).s2 = 0.0;
  //final(&stack,members,members4,&sols);
  final(&stack,m,m2,&sols);
  //printsolutions(&sols);
  //totab(&sols);
}








