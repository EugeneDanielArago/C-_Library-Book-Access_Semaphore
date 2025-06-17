#include <iostream>
#include <thread>
#include <semaphore>
#include <vector>
#include <mutex>
#include <chrono>
#include <string>

using namespace std;

counting_semaphore<5> sem(5);
mutex mtx;

void borrow_book(const string& student_id);
void return_book(const string& student_id);
void automatedSimulation();
void simulateStudent(int studentId);

// Borrow a book - Manually
void borrow_book(const string& student_id) {
    lock_guard<mutex> lock(mtx);
    cout << "Student " << student_id << " is waiting to borrow a book...\n";

    sem.acquire();

    cout << "Student " << student_id << " has borrowed a book.\n";
}

// Return a book - Manually
void return_book(const string& student_id) {
    lock_guard<mutex> lock(mtx);
    cout << "Student " << student_id << " is returning a book.\n";

    sem.release();

    cout << "Student " << student_id << " has returned the book.\n";
}

// Automated simulation function
void automatedSimulation() {
    const int totalStudents = 8;
    thread students[totalStudents];

    mtx.lock();
    cout << "\n===== Automated Library Book Access Simulation =====\n";
    cout << "There are 5 copies available.\n";
    mtx.unlock();

    for (int i = 0; i < totalStudents; ++i) {
        students[i] = thread(simulateStudent, i + 1);
        this_thread::sleep_for(chrono::milliseconds(300));
    }

    for (int i = 0; i < totalStudents; ++i) {
        students[i].join();
    }

    mtx.lock();
    cout << "Automated simulation completed.\n";
    mtx.unlock();
}

// Simulates a student borrowing and returning a book
void simulateStudent(int studentId) {
    string id = to_string(studentId);

    {
        lock_guard<mutex> lock(mtx);
        cout << "Student " << id << " is waiting to borrow a book...\n";
    }

    sem.acquire();

    {
        lock_guard<mutex> lock(mtx);
        cout << "Student " << id << " has borrowed a book.\n";
    }

    this_thread::sleep_for(chrono::seconds(2)); 

    {
        lock_guard<mutex> lock(mtx);
        cout << "Student " << id << " is returning the book.\n";
    }

    sem.release();

    {
        lock_guard<mutex> lock(mtx);
        cout << "Student " << id << " has returned the book.\n";
    }
}

int main() {
    int choice;
    string student_id;

    while (true) {
        cout << "\n----------------------------\n";
        cout << "    Library Book Access";
        cout << "\n----------------------------\n";
        cout << "1. Borrow a Book Manually\n";
        cout << "2. Return a Book Manually\n";
        cout << "3. Run Automated Simulation\n";
        cout << "4. Exit\n";
        cout << "Enter your choice(1-4): ";
        cin >> choice;

        switch (choice) {
            case 1: 
                cout << "Enter Student ID to borrow a book: ";
                cin >> student_id;
                thread(borrow_book, student_id).detach();
                this_thread::sleep_for(chrono::milliseconds(300));
                break;

            case 2: 
                cout << "Enter Student ID to return a book: ";
                cin >> student_id;
                thread(return_book, student_id).detach();
                this_thread::sleep_for(chrono::milliseconds(300)); 
                break;

            case 3: 
                automatedSimulation();
                break;

            case 4:
                cout << "Exiting the system. Goodbye!\n";
                return 0;

            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}
