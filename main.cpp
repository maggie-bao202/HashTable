#include <iostream>
#include <cstring>
#include <vector>
#include<iomanip>
#include <stdlib.h>
#include <fstream>
#include <math.h>
using namespace std;

/*Date: 6/7/2020 Author: Maggie Bao.*/


struct Student{//student structure
  char fname[40];
  char lname[40];
  int id;
  float gpa;
};

struct Node{
  Student* student;
  Node* next;
};

void addStudent(Node** &, int&);
void printStudent(Node**, int);
void deleteStudent(Node** &, int&);
int getHashKey(int, int, int);
int getSize(Node*);

int main() {
  //initialize vector pointer, keyword, and student struct
  srand(time(NULL));
	
  vector<char*>* fnameList = new vector<char*>();
  vector<char*>* lnameList = new vector<char*>();
  
  char* keyword = new char[10];
  Student* student = new Student();
  int size = 100;
  Node** hashTable = new Node*[size];

  for (int i=0; i < size; i++){
    hashTable[i] = new Node();
    hashTable[i]->student = NULL;
    hashTable[i]->next = NULL;
  }
  
  char* fileName = new char(10);
  cout << "First Name File: " << endl;
  cin.getline(fileName, 10, '\n');
  ifstream fs(fileName);
  if (!fs.is_open()){
    cout << "Incorrect File Name." << endl;
    return 0;
  }
  char* fname = new char(20);
  while (!fs.eof()){
    fs >> fname;
    fnameList->push_back(fname);
  }
  cout << "Last Name File: " << endl;
  cin.getline(fileName, 10, '\n');
  ifstream ls(fileName);
  if (!ls.is_open()){
    cout << "Incorrect File Name." << endl;
    return 0;
  }
  char* lname = new char(20);
  while (!ls.eof()){
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

int getHashKey(int n, int s, int t){
  while (n != 0){
    t = ((t*17)+(n%10))%s;
    n = n/10;
  }
  t = (t*17)%s;
  return t;
}

int getSize(Node* current){
  int counter = 0;
  while (current != NULL && current->student != NULL){
    counter++;
    current = current->next;
  }
  return counter;
}

void addStudent(Node**& hashTable, int &size){ //prompts for user to input information about a student, then stores data inside the struct, adds to the vector
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
  //int hashKey = getHashKey(student->id, size, 0);
  while (getSize(hashTable[getHashKey(student->id, size, 0)]) == 3){
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
	//int key = getHashKey(temp->student->id, twicesize, 0);
	//cout << "KEY:" << key << endl;
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
  if (hashTable[getHashKey(student->id, size, 0)]->student == NULL) {//first in list
    Node* node = new Node();
    node->student = student;
    node->next = new Node();
    node->next->student = NULL;
    hashTable[getHashKey(student->id, size, 0)] = node;
  }
  else { // not first in list
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
  cout << "KEY:" << getHashKey(student->id, size, 0) << endl;
  cout << "Student added." << endl << endl;
}


 
void printStudent(Node** hashTable, int size) {//instead of a forloop with variable i, vectors use iterators
  cout << "List of Students:" << endl;
  for (int i = 0; i < size; i++){
    Node* element = hashTable[i];
    while (element != NULL && element->student != NULL){
      cout << element->student->fname << " ";//for each iteration print out elements of struct
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
  if (iid != hashTable[hashKey]->student->id){
    Node* current = hashTable[hashKey];
    while (iid != current->next->student->id){
      current = current->next;
    }
    Node* toDelete = current->next;
    current->next = toDelete->next;
    delete toDelete;
  }
  else{
    Node* toDelete = hashTable[hashKey];
    hashTable[hashKey] = hashTable[hashKey]->next;
    delete toDelete;
  }
  cout << "Student Deleted." << endl;
  cin.clear();//debugging to make header not print twice
  cin.ignore(999,'\n');
}
