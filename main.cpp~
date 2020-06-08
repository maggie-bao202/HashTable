#include <iostream>
#include <cstring>
#include <vector>
#include<iterator>
#include<iomanip>
using namespace std;

/*Date: 10/30/19 Author: Maggie Bao. Description: Student List allows the user to input information about a student from inputting "ADD". The student is added to a list, which can be viewed by the "PRINT" command. The user can delete students by the "DELETE" command, typing in the Student ID of the student. Finally, the user could exit out of the program with the "QUIT" command.*/

struct Student{//student structure
  char fname[40];
  char lname[40];
  int id;
  float gpa;
};

void addStudent(vector<Student*>*);
void printStudent(vector<Student*>*);
void deleteStudent(vector<Student*>*);
  
int main() {
  //initialize vector pointer, keyword, and student struct
  vector<Student*> studentList;
  char* keyword = new char[10];
  Student* student = new Student();
  
  bool stillPlaying = true;//will continue prompting for keyword until quit
  while(stillPlaying == true){
    cout << "Type in a keyword (\"ADD\", \"PRINT\",\"DELETE\", or \"QUIT\")"<<endl;
    cin.get(keyword, 10); //put into array of 10. Extra char will be ignored
    cin.clear(); //clear, ignore fixes null bug
    cin.ignore(9999, '\n');
    if (strcmp(keyword, "ADD") == 0){//if keyword char pointer matches with str
      addStudent(&studentList);
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

void addStudent(vector<Student*>* studentList){ //prompts for user to input information about a student, then stores data inside the struct, adds to the vector
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
  studentList->push_back(student);//add to vector
  cout << "Student added." << endl << endl;
}
 
void printStudent(vector<Student*>* studentList) {//instead of a forloop with variable i, vectors use iterators
  cout << "List of Students:" << endl << endl;
  vector<Student*>::iterator it; //create iterator
  for(it = studentList->begin(); it != studentList->end(); it++){// goes through each element of the vector
    cout << (*it)->fname << " ";//for each iteration print out elements of struct
     cout << (*it)->lname << ", ";
     cout << (*it) -> id << ", ";
     cout << fixed<<setprecision(2);//round to nearest hundreth for the gpa
     cout << (*it)->gpa << endl;

   }
  cout << endl;
}

void deleteStudent(vector<Student*>* studentList){//if name matches user input in iteration, then delete element
  vector<Student*>::iterator it;
  cout << "Enter the Student ID of the student to remove: ";
  int iid;
  cin >> iid;//take user input
  int counter = 0;
  for(it = studentList->begin(); it != studentList->end(); it++){//same as print
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
