#include <iostream>
#include <cstring>
#include <vector>
#include<iterator>
#include<iomanip>
#include <stdlib.h>

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

void addStudent(vector<Student*>*, int);
void printStudent(vector<Student*>*, int);
void deleteStudent(vector<Student*>*, int);
int hashKey(int, int);
int getSize(Node*);

int main() {
  //initialize vector pointer, keyword, and student struct
  srand(time(NULL));
	
  vector<char*> fnameList = new vector<char*>();
  vector<char*> lnameList = new vector<char*>();
  
  char* keyword = new char[10];
  Student* student = new Student();
  int size = 100;
  Node** hashTable = new Node*[size];

  for (int i=0; i < size; i++){
    hashTable[i] = NULL;
  }

  char* fname = new char(20);
  char* lname = new char(20);

  char* fileName = new char(10);
  
  cout << "First Name File: " << endl;
  cin.getline(fileName, 10, '\n');
  ifstream fs (fnameFile);
  cout << "Last Name File: " << endl;
  cin.getline(fileName, 10, '\n');
  if (!fs.is_open()){
    cout << "Incorrect File Name." << endl;
    return 0;
  }
  while (!fs.eof()){
    fs >> fname;
    fnameList.push_back(fname);
  }
  ifstream ls (lameFile);
  if (!ls.is_open()){
    cout << "Incorrect File Name." << endl;
    return 0;
  }
  while (!ls.eof()){
    ls >> lname;
    lnameList.push_back(lname);
  }

  bool stillPlaying = true;//will continue prompting for keyword until quit
  while(stillPlaying == true){
    cout << "Type in a keyword (\"ADD\", \"PRINT\",\"DELETE\", or \"QUIT\")"<<endl;
    cin.get(keyword, 10); //put into array of 10. Extra char will be ignored
    cin.clear(); //clear, ignore fixes null bug
    cin.ignore(9999, '\n');
    if (strcmp(keyword, "ADD") == 0){//if keyword char pointer matches with str
      addStudent();
    }
    else if (strcmp(keyword, "PRINT") == 0){
      printStudent(&studentList);
    }
    else if (strcmp(keyword, "DELETE") == 0){
      deleteStudent(&studentList);
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

int hashNumber(int n, int s, int t){
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

void addStudent(int &number, Node**& hashTable, int &size){ //prompts for user to input information about a student, then stores data inside the struct, adds to the vector
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
  int hashKey = getHash(student->id, size);
  bool check = false;
  Node* first = hashTable[hashKey];
  while (first != NULL && first->student != NULL){
    if (first->student->id == student->id){
      cout << "Enter valid ID." << endl;
      return;
    }
    first = first->next;
  }
  if (check == true){
    continue;
  }
  while (getSize(students[hashKey]) == 3){
    int twicesize = size*2;
    Node** newHashTable = new Node*[twicesize];
    for (int i = 0; i < twicesize; i++){
      newHashTable[i] = new Node();
      newHashTable[i]->student = NULL;
      newHashTable[i]->next = NULL;
    }
    for (int i = 0; i < size; i++){
      Node* temp = hashTable[i];
      while (temp != NULL && temp->student != NULL){
	Node* node = new Node();
	node->student = temp->student;
	node->next = NULL;
	int key = getHash(temp->student->id, twicesize);
	if (newHashTable[key]->student == NULL){
	  newHashTable[key] = node;
	}
	else{
	  Node* current = hashTable[key];
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
    size = twicesize;
    Node** toDeleteHash = hashTable;
    hashTable = newHashTable;
    delete toDeleteHash;
  }
  if (hashTable[hashKey]->student == NULL) {
    Node* node = new Node();
    node->next = new Node();
    node->student = student;
    node->next->student = NULL;
    hashTable[hashKey] = node;
  }
  else { // not first in list
    Node* node = hashKey[hashKey];
    while (node->next != NULL && node->next->student != NULL) {
      node = node->next;
    }
    Node* n = new node();
    n->next = new node();
    n->student = student;
    n->next->student = NULL;
    node->next = n;
  }
  cout << "Student added." << endl << endl;
}

void printStudent(Node** hashTable, int size) {//instead of a forloop with variable i, vectors use iterators
  cout << "List of Students:" << endl << endl;
  for (int i = 0; i < size; i++){
    Node* current = hashTable[i];
    while (current != NULL && current->student != NULL){
      Student* element = current->student;
      cout << element->fname << " ";//for each iteration print out elements of struct
      cout << element->lname << ", ";
      cout << element->id << ", ";
      cout << fixed<<setprecision(2);//round to nearest hundreth for the gpa
      cout << element->gpa << endl;
      current = current->next;
   }
}

void deleteStudent(vector<Student*>* studentList){//if name matches user input in iteration, then delete element
  cout << "Enter the Student ID of the student to remove: ";
  int iid;
  cin >> iid;//take user input
  int hashKey = getHash(iid, size);
  if
  if ((*it)->id == iid){//if ints are the same
      delete *it;//delete memory
      studentList->erase(it);//remove from it
      counter++;//add 1
      cout << "Student removed." << endl << endl;
      break;
    }
  }
  if (counter == 0){//if counter did not iterate
    cout << "Not a valid student." << endl << endl;
  }
  cin.clear();//debugging to make header not print twice
  cin.ignore(999,'\n');
}
