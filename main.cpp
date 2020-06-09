#include <iostream>
#include <cstring>
#include <vector>
#include<iomanip>
#include <stdlib.h>
#include <fstream>
#include <math.h>
#include <iterator>
using namespace std;

/*Date: 6/7/2020 Author: Maggie Bao. Description: In this program, students were stored using a data structure called a hash table. The program prompts the user to add, delete, and print out students. Help from Joyce Chen.*/


struct Student{//student structure
  char fname[40];
  char lname[40];
  int id;
  float gpa;
};

struct Node{//node structure for collision
  Student* student;
  Node* next;
};

void addStudent(Node** &, int&);
void printStudent(Node**, int);
void deleteStudent(Node** &, int&);
int getHashKey(int, int, int);
int getSize(Node*);
void randomAdd(Node** &, int &, int &, vector<char*>*, vector<char*>*);

int main() {
  //initialize vector pointer, keyword, and student struct
  srand(time(NULL));
  vector<char*>* fnameList = new vector<char*>();//allocate memory for random names
  vector<char*>* lnameList = new vector<char*>();
  
  char* keyword = new char[10];
  Student* student = new Student();
  int size = 100;
  int n = 1;
  Node** hashTable = new Node*[size];
  for (int i=0; i < size; i++){//set elements of hashtable to null
    hashTable[i] = new Node();
    hashTable[i]->student = NULL;
    hashTable[i]->next = NULL;
  }
  char* fileName = new char(10);
  cout << "First Name File: " << endl;
  cin.getline(fileName, 10, '\n');//read filename from user input
  ifstream fs(fileName);
  if (!fs.is_open()){//if file is not found
    cout << "Incorrect File Name." << endl;
    return 0;
  }
  while (!fs.eof()){//if file found, push each "name" into vector
    char* fname = new char(20);
    fs >> fname;
    fnameList->push_back(fname);
  }
  cout << "Last Name File: " << endl;//do same for last names file
  cin.getline(fileName, 10, '\n');
  ifstream ls(fileName);
  if (!ls.is_open()){
    cout << "Incorrect File Name." << endl;
    return 0;
  }
  while (!ls.eof()){
    char* lname = new char(20);
    ls >> lname;
    lnameList->push_back(lname);
  }

  bool stillPlaying = true;//will continue prompting for keyword until quit
  while(stillPlaying == true){
    cout << "Type in a keyword (\"MANUAL\" ADD, \"RANDOM\" ADD, \"PRINT\",\"DELETE\", or \"QUIT\")"<<endl;
    cin.get(keyword, 10); //put into array of 10. Extra char will be ignored
    cin.clear(); //clear, ignore fixes null bug
    cin.ignore(9999, '\n');
    if (strcmp(keyword, "MANUAL") == 0){//if keyword char pointer matches with str
      addStudent(hashTable, size);
    }
    else if (strcmp(keyword, "RANDOM") == 0){
      randomAdd(hashTable, n, size, fnameList, lnameList);
    }
    else if (strcmp(keyword, "PRINT") == 0){
      printStudent(hashTable, size);
    }
    else if (strcmp(keyword, "DELETE") == 0){
      deleteStudent(hashTable, size);
    }
    else if(strcmp(keyword, "QUIT") == 0){
      cout << "Have a nice day!" << endl;
      stillPlaying = false; //close program

    }
    else{
      cout << "Make sure the keyword is capitalized." << endl;
    }
  }
  return 0;
}

int getHashKey(int n, int s, int t){//algorithm to calculate index of hash table from id
  while (n != 0){
    t = ((t*17)+(n%10))%s;
    n = n/10;
  }
  t = (t*17)%s;
  return t;
}

int getSize(Node* current){//returns how many students are in the linked list. Used when storing collisions
  int counter = 0;
  while (current != NULL && current->student != NULL){
    counter++;
    current = current->next;
  }
  return counter;
}

void addStudent(Node**& hashTable, int &size){ //prompts for user to input information about a student, then stores data inside the struct, adds to hash table
  Student* student = new Student();
  int iid = 0;
  float igpa = 0.0;
  cout << "Enter the first name: ";
  cin >> student->fname;//stores input inside fname in struct pointer
  cout << "Enter the last name: ";
  cin >> student->lname;
  cout << "Enter the Student ID: ";
  cin >> iid; //reads in int inputid
  while (iid > 999999 || iid < 100000){//if out of id range
    cout << "Enter a 6 digit number." << endl;
    cout << "Enter the Student ID: ";
    cin >> iid;//take input again
  }
  student->id = iid;//read into struct student
  cout << "Enter the GPA: ";
  cin >> igpa; 
  while (igpa > 5.0 || igpa <= 0.0){//if gpa is greater than 5 and not 0
    cout << "GPA cannot exceed a 5.00, and cannot be 0.00." << endl;
    cout << "Enter the GPA: ";
    cin >> igpa;
  }
  student->gpa = igpa;
  cin.clear();//debugging, so it doesn't print header twice
  cin.ignore(999, '\n');
  while (getSize(hashTable[getHashKey(student->id, size, 0)]) == 3){//if the size of the linked list at that particular index of the hash table is 3, rehash
    int twicesize = size*2;
    Node** newHashTable = new Node*[twicesize];//create new hash table double the size
    for (int i = 0; i < twicesize; i++){
      newHashTable[i] = new Node();
      newHashTable[i]->student = NULL;
      newHashTable[i]->next = NULL;
    }
    for (int i = 0; i < size; i++){//goes through original and rehashes new hashtable
      Node* temp = hashTable[i];
      while (temp->student != NULL){
	Node* node = new Node();
	node->student = temp->student;
	node->next = NULL;
	//int key = getHashKey(temp->student->id, twicesize, 0);
	//cout << "KEY:" << key << endl;
	if (newHashTable[getHashKey(temp->student->id, twicesize, 0)]->student == NULL){//if the student at that element is NULL
	  newHashTable[getHashKey(temp->student->id, twicesize, 0)] = node;//make element the new node
	}
	else{
	  Node* current = hashTable[getHashKey(temp->student->id, twicesize, 0)];
	  while (current->next != NULL){
	    current = current->next;
	  }
	  current->next = node;
	}
	Node* toDelete = temp;//remove from linked list
	temp = temp->next;
	delete toDelete;
      }
    }
    delete[] hashTable;//remove original hash table
    hashTable = newHashTable;
    size = twicesize;//update to new size
  }
  if (hashTable[getHashKey(student->id, size, 0)]->student == NULL) {//student is first in list
    Node* node = new Node();
    node->student = student;
    node->next = new Node();
    node->next->student = NULL;
    hashTable[getHashKey(student->id, size, 0)] = node;//add student to hash table
  }
  else {
    Node* node = hashTable[getHashKey(student->id, size, 0)];
    while (node->next != NULL && node->next->student != NULL) {//traverse to last element
      node = node->next;
    }
    Node* n = new Node();
    n->student = student;
    n->next = new Node();
    n->next->student = NULL;
    node->next = n;//add to end of linked list
  }
  //cout << "KEY:" << getHashKey(student->id, size, 0) << endl;
  cout << "Student added." << endl << endl;
}

void randomAdd(Node** &hashTable, int &n, int &size, vector<char*>* fnameList, vector<char*>* lnameList){
  int num = 0;
  int newid = 100000;
  cout << "Number of Students: " << endl;
  cin >> num;//how many students?
  while (num != 0) {
    Student* student = new Student();//new student
    int fsize = fnameList->size();
    int lsize = lnameList->size();
    int fi = rand() % fnameList->size();//random first name
    int li = rand() % lnameList->size();//random last name
    int ind = 0;
    vector<char*> :: iterator f;
    for ( f = fnameList->begin(); f != fnameList->end(); f++) {//find first name at random index
      if (ind == fi) {
	strcpy(student->fname, (*f));//copy into the student
      }
      ind++;
    }
    ind = 0;
    vector<char*> :: iterator l;
    for ( l = lnameList->begin(); l != lnameList->end(); l++) {//find last name at random index
      if (ind == li) {
        strcpy(student->lname, (*l));//copy into student
      }
      ind++;
    }
    student->gpa = float(rand()%350+100)/100;//random gpa
    student->id = n + newid;//student id, incrementing by 1 each time
    n++;
    int key = getHashKey(student->id, size, 0);
    Node* node = hashTable[key];
    int linknum = getSize(hashTable[getHashKey(student->id, size, 0)]);
    int twice = size*2;
    while (linknum == 3){//rehashing if 3 or more students in linked list (see above)
    int twicesize = size*2;
    Node** newHashTable = new Node*[twicesize];
    for (int i = 0; i < twicesize; i++){
      newHashTable[i] = new Node();
      newHashTable[i]->student = NULL;
      newHashTable[i]->next = NULL;
    }
    for (int i = 0; i < size; i++){//rehash
      Node* temp = hashTable[i];
      while (temp->student != NULL){
        Node* node = new Node();
        node->student = temp->student;
        node->next = NULL;
        if (newHashTable[getHashKey(temp->student->id, twicesize, 0)]->student == NULL){
          newHashTable[getHashKey(temp->student->id, twicesize, 0)] = node;
        }
        else{
          Node* current = hashTable[getHashKey(temp->student->id, twicesize, 0)];
          while (current->next != NULL){
            current = current->next;
          }
          current->next = node;
        }
        Node* toDelete = temp;
        temp = temp->next;
        delete toDelete;
      }
    }
    delete[] hashTable;
    hashTable = newHashTable;
    size = twicesize;
    }
    if (hashTable[getHashKey(student->id, size, 0)]->student == NULL) {
      Node* node = new Node();
      node->student = student;
      node->next = new Node();
      node->next->student = NULL;
      hashTable[getHashKey(student->id, size, 0)] = node;
    }
    else {
      Node* node = hashTable[getHashKey(student->id, size, 0)];
      while (node->next != NULL && node->next->student != NULL) {
	node = node->next;
      }
      Node* n = new Node();
      n->student = student;
      n->next = new Node();
      n->next->student = NULL;
      node->next = n;
    }
    num--;
  }
  cout << "Students Added." << endl;
}

void printStudent(Node** hashTable, int size) {//print students
  cout << "List of Students:" << endl;
  for (int i = 0; i < size; i++){//
    Node* element = hashTable[i];
    while (element != NULL && element->student != NULL){
      cout << element->student->fname << " ";//traverse linked lsit
      cout << element->student->lname << ", ";
      cout << element->student->id << ", ";
      cout << fixed<<setprecision(2);//round to nearest hundreth for the gpa
      cout << element->student->gpa << endl;
      element = element->next;
    }
  }
  cout << endl;
}

void deleteStudent(Node** &hashTable, int &size){
  int iid = 0;
  cout << "Enter ID: " << endl;
  cin >> iid;
  int hashKey = getHashKey(iid, size, 0);
  if (iid != hashTable[hashKey]->student->id){//if id is not in hash table
    Node* current = hashTable[hashKey];
    while (iid != current->next->student->id){//traverse linked list
      current = current->next;
    }
    Node* toDelete = current->next;//delete
    current->next = toDelete->next;
    delete toDelete;
  }
  else{//if in hash table
    Node* toDelete = hashTable[hashKey];
    hashTable[hashKey] = hashTable[hashKey]->next;
    delete toDelete;
  }
  cout << "Student Deleted." << endl;
  cin.clear();//debugging to make header not print twice
  cin.ignore(999,'\n');
}
