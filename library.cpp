#include <iostream>
#include <semaphore>
#include <mutex>
#include <vector>
#include <thread>

using namespace std;

counting_semaphore<5> sem(5); 
mutex mtx;

void borrowBook(const string& student_id){
    mtx.lock();
    cout << "Student " << student_id << "is waiting to borrow a book.....\n";
    mtx.unlock();
    
    sem.acquire(); 

    mtx.lock();
    cout << "Student " << student_id << "has borrowed a book.\n";
    mtx.unlock();
}

void returnBook(const string& student_id){
    mtx.lock();
    cout << "Student " << student_id << "is returning a book.....\n";
    mtx.unlock();
    
    sem.release(); 

    mtx.lock();
    cout << "Student " << student_id << "has returned a book.\n";
    mtx.unlock();
}



int main(){
    int choice;
    int student_id;
    
    while(true){
        
        cout << "-------------------\n";
        cout << "Library Book Access";
        cout << "\n-------------------\n";
        cout << "1. Manually Borrow a Book\n";
        cout << "2. Manually Return a Book\n";
        cout << "3. Automated Borrow & Return Book\n";
        cout << "4. Exit\n";
        cout << "Enter your choice(1-4): ";
        cin >> choice;

        if(choice == 1){
            cout << "Enter Student ID to borrow a book: ";
            cin >> student_id;
            thread(borrowBook, student_id).detach();
        }
        else if(choice == 2){
            cout << "Enter Student ID to return a book: ";
            cin >> student_id;
            thread(returnBook, student_id).detach();
        }
    }
    
  return 0;
}
