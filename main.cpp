#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
#include <string>
#include <iomanip>
using namespace std;

struct patient{
    string name;
    int severity;
    int arrivalTime;

};

class MaxHeap{
    int size,capacity;
    int sumSeverity=0;
    patient * emergencyList;


    void resize(int new_capacity)
    {
        patient* new_data = new patient[new_capacity];
        for (int i = 0; i < size; ++i) {
            new_data[i] = emergencyList[i];
        }

        delete[] emergencyList;
        emergencyList = new_data;
        capacity = new_capacity;
    }

    void shiftDown(int index) {
        int largestIndex = index;
        int l = 2 * index + 1;
        int r = 2 * index + 2;

        if (l < size) {
            if (emergencyList[l].severity > emergencyList[largestIndex].severity ||
                (emergencyList[l].severity == emergencyList[largestIndex].severity &&
                 emergencyList[l].arrivalTime < emergencyList[largestIndex].arrivalTime)) {
                largestIndex = l;
            }
        }

        if (r < size) {
            if (emergencyList[r].severity > emergencyList[largestIndex].severity ||
                (emergencyList[r].severity == emergencyList[largestIndex].severity &&
                 emergencyList[r].arrivalTime < emergencyList[largestIndex].arrivalTime)) {
                largestIndex = r;
            }
        }

        if (largestIndex != index) {
            swap(emergencyList[index], emergencyList[largestIndex]);
            shiftDown(largestIndex);
        }
    }

    bool comparePatients(const patient& a, const patient& b) {
        return a.severity > b.severity ||
               (a.severity == b.severity && a.arrivalTime < b.arrivalTime);
    }

    void shiftUp(int current){
        while (current > 0) {
            int parent = (current - 1) / 2;
            if (comparePatients(emergencyList[current], emergencyList[parent])) {
                swap(emergencyList[current], emergencyList[parent]);
                current = parent;
            } else break;
        }
    }

    void saveTreatmentLog(patient&p){
        fstream treatment("TreatmentLog.txt",ios::app);
        if (!treatment) {
            cerr << "Error opening TreatmentLog!" << endl;
            return;
        }
        treatment<<"Patient name: "<<p.name<<" ,Severity: "<<p.severity<<" ,ArrivalTime: "<<p.arrivalTime<<endl;
    }


public:
    MaxHeap(int cap=2){
        this->size=0;
        capacity=cap;
        emergencyList= new patient[capacity];
    }

    void insert(patient& patient){

        if (size >= capacity) {
            resize(capacity * 2);
        }

        sumSeverity+=patient.severity;
        emergencyList[size] = patient;
        int current = size;
        size++;


        shiftUp(current);

    }

    patient extractMax() {
        if (size == 0) {
            patient p;
            p.name = "None";
            p.severity = 0;
            p.arrivalTime = 0;
            return p;
        }

        patient mx = emergencyList[0];
        emergencyList[0] = emergencyList[size - 1];
        sumSeverity -= mx.severity;
        size--;
        shiftDown(0);
        saveTreatmentLog(mx);
        return mx;
    }

    patient peek() {
        if (size == 0) {
            patient p;
            p.name = "None";
            p.severity = 0;
            p.arrivalTime = 0;
            return p;
        }
        return emergencyList[0];
    }


    void printHeap(){
        cout<<"Emergency List: [";
        for(int i=0; i<size; i++){
            if(i!=size-1)cout << emergencyList[i].name<<", ";
            else cout <<emergencyList[i].name;
        }
        cout <<"]";
        cout<<endl;
    }

    void updateSeverity(const string& name,int& severity){
        for(int i=0; i<size; i++){
            if(emergencyList[i].name==name){
                if(emergencyList[i].severity>severity){
                    emergencyList[i].severity=severity;
                    shiftDown(i);
                    cout << "Severity updated successfully" << endl;
                    return;
                }
                else if(emergencyList[i].severity<severity){
                    emergencyList[i].severity=severity;
                    shiftUp(i);
                    cout << "Severity updated successfully" << endl;
                    return;
                }
                else{
                    cout << "Severity updated successfully" << endl;
                    return;
                }
            }
        }
        cout << "No patient with this name found" << endl;
    }

    patient findPatient(const string& name){
        for(int i=0; i<size; i++){
            if(name == emergencyList[i].name){
                return emergencyList[i];
            }
        }
        patient p;
        p.name = "None";
        p.severity = 0;
        p.arrivalTime = 0;
        cout << "Patient not found" << endl;
        return p;
    }

    void removePatient(const string& name){
        for(int i=0; i<size; i++){
            if(emergencyList[i].name == name){
                sumSeverity-=emergencyList[i].severity;
                swap(emergencyList[i],emergencyList[size-1]);
                size--;
                shiftDown(i);
                shiftUp(i);
                cout << "Patient removed Successfully" << endl;
                return;
            }
        }

        cout << "Patient not found" << endl;
    }

    double getAverage(){
        if (size == 0) return 0.0;
        return sumSeverity*1.0/size;
    }

    void status(){
        double averageSeverity=getAverage();
        if(averageSeverity>=90 && averageSeverity<=100){
            cout << "Status is critical" << endl;
        }

        else if(averageSeverity>=70 && averageSeverity<=89){
            cout << "Status is Urgent" << endl;
        }
        else if(averageSeverity>=50 && averageSeverity<=69){
            cout << "Status is Moderate" <<endl;
        }
        else {
            cout << "Status is Low" << endl;
        }
    }

    void loadHistory(){
        ifstream history("EmergencyList.txt");
        if(history.is_open()){
            string holder;
            history>>holder>>holder;
            patient p;
            while (history >> p.name >> p.severity >> p.arrivalTime) {
                insert(p);
            }
        }
        else {
            cout << "Failed to load HistoryLog" << endl;
            return;
        }
    }

    void saveHistory(){
        ofstream history("EmergencyList.txt");
        history<<"Patients"<<endl<<"========================================================="<<endl;
        for(int i=0; i<size; i++){
            history<<emergencyList[i].name<<" "<<emergencyList[i].severity<<" "<<emergencyList[i].arrivalTime<<endl;
        }
    }

    void displayTreatmentLog(){
        ifstream Treatment("TreatmentLog.txt");
        string holder;
        if(Treatment){
            while (getline(Treatment,holder)) {
                cout << holder<<endl;
            }
        }
    }

    ~MaxHeap() {
        delete[] emergencyList;
    }

};

void testCases(MaxHeap& emergencyQueue) {
    ifstream testFile("testcases.txt");
    if (!testFile.is_open()) {
        cout << "Failed to open TestCases.txt" << endl;
        return;
    }

    string line;
    while (getline(testFile, line)) {
        if (line.empty()) continue;

        if (line[0] == '#') {
            cout << "\n" << line << endl;
            continue;
        }

        stringstream ss(line);
        string command;
        ss >> command;

        if (command == "INSERT") {
            patient p;
            ss >> p.name >> p.severity >> p.arrivalTime;
            emergencyQueue.insert(p);
            cout << "Inserted: " << p.name << " (Severity: " << p.severity << ", Arrival: " << p.arrivalTime << ")" << endl;
            emergencyQueue.printHeap();
        }

        else if (command == "EXTRACT") {
            patient p = emergencyQueue.extractMax();
            cout << "Extracted: " << p.name << " (Severity: " << p.severity << ", Arrival: " << p.arrivalTime << ")" << endl;
        }

        else if (command == "PEEK") {
            patient p = emergencyQueue.peek();
            cout << "Next Patient: " << p.name << " (Severity: " << p.severity << ", Arrival: " << p.arrivalTime << ")" << endl;
        }

        else if (command == "UPDATE") {
            string name;
            int severity;
            ss >> name >> severity;
            emergencyQueue.updateSeverity(name, severity);
        }

        else if (command == "REMOVE") {
            string name;
            ss >> name;
            emergencyQueue.removePatient(name);
        }

        else if (command == "PRINT") {
            emergencyQueue.printHeap();
        }

        else if (command == "STATUS") {
            emergencyQueue.status();
        }

        else if (command == "AVERAGE") {
            cout << "Average Severity: " << emergencyQueue.getAverage() << endl;
        }

        else if (command == "SAVE") {
            emergencyQueue.saveHistory();
            cout << "Patient history saved." << endl;
        }

        else if (command == "DISPLAYLOG") {
            emergencyQueue.displayTreatmentLog();
        }

        else {
            cout << "Unknown command: " << command << endl;
        }
    }
}

//check if a string contains only digits
bool isDigitsOnly(const string &s) {
    for (char c : s) {
        if (!isdigit(c)) {
            return false;
        }
    }
    return !s.empty();
}

// get validated integer input
int getValidatedInt(const string &prompt, int minVal, int maxVal) {
    string input;
    int value;

    while (true) {
        cout << prompt;
        getline(cin, input);

        if (!isDigitsOnly(input)) {
            cout << "Invalid input! Please enter numbers only (" << minVal << "-" << maxVal << ") " << endl;
            continue;
        }

        try {
            value = stoi(input);
        } catch (...) {
            cout << "Invalid input! Please enter a valid number (" << minVal << "-" << maxVal << ") " << endl;
            continue;
        }

        // Check range
        if (value >= minVal && value <= maxVal) {
            return value;
        }

        cout << "Input out of range! Please enter between " << minVal << " and " << maxVal << endl;
    }
}

int main() {
    MaxHeap emergencyQueue;
    int loadChoice;
    int mainChoice;
    string name;
    int severity, arrivalTime;
    patient p;
    string input;

    while(true) {
        cout << "Do you want to:\n";
        cout << "1. Load from history file\n";
        cout << "2. Start with an empty list\n";
        cout << "3. Load test cases from 'TestCases.txt'\n";
        cout << "4. Exit\n";

        loadChoice = getValidatedInt("Enter your choice (1-4): ", 1, 4);

        if (loadChoice == 4) {
            cout << "==============================================================="<< endl;
            cout << "Exiting program. Goodbye!" << endl;
            cout << "==============================================================="<< endl;
            break;
        }
        if (loadChoice == 1) {
            emergencyQueue.loadHistory();
            cout << "==============================================================="<< endl;
            cout << "History loaded successfully!" << endl;
            cout << "==============================================================="<< endl;
        } else if (loadChoice == 2) {
            cout << "==============================================================="<< endl;
            cout << "Starting with an empty patient list." << endl;
            cout << "==============================================================="<< endl;
        } else if (loadChoice == 3) {
            testCases(emergencyQueue);
            cout << "==============================================================="<< endl;
            cout << "TestCases Finished Successfully"<< endl;
            cout << "==============================================================="<< endl;
            continue;
        }

        do {
            cout << "\n****************** Emergency Room MaxHeap Priority System ******************" << endl<<endl;
            cout << setw(10) << "1.  Add New Patient"
                 << setw(55) << "2.  Treat Next Patient (Extract Max)"
                 << setw(40)<< "3.  View Next Patient (Peek Max)" << endl;

            cout << setw(20) << "4.  Update Patient Severity"
                 << setw(39) << "5.  Find Patient Information"
                 << setw(44)<< "6.  Remove Patient from Heap" << endl;

            cout << setw(20) << "7.  View Current EmergencyList"
                 << setw(33) << "8.  View Average Severity"
                 <<setw(34)<< "9.  View Status" << endl;

            cout << setw(20) << "10. View Treatment Log"
                 << setw(40) << "11. Save Patient History"
                 << setw(36)<<"12. Exit Program" << endl<<endl;

            mainChoice = getValidatedInt("Enter your choice (1-12): ", 1, 12);

            switch (mainChoice) {
            case 1:
                cout << "Enter patient name: ";
                getline(cin, p.name);
                while (p.name.empty()) {
                    cout << "Name cannot be empty! Please enter patient name: ";
                    getline(cin, p.name);
                }

                p.severity = getValidatedInt("Enter severity (1-100): ", 1, 100);
                p.arrivalTime = getValidatedInt("Enter arrival time (positive number): ", 1, INT_MAX);

                emergencyQueue.insert(p);
                cout << "Patient added successfully!" << endl;
                emergencyQueue.printHeap();
                break;

            case 2:
                p = emergencyQueue.extractMax();
                if (p.name == "None") {
                    cout << "No patients in the queue!" << endl;
                }
                else {
                    cout << "Treated patient: " << p.name
                        << " (Severity: " << p.severity
                        << ", Arrival Time: " << p.arrivalTime << ")" << endl;
                }
                break;

            case 3:
                p = emergencyQueue.peek();
                if (p.name == "None") {
                    cout << "No patients in the queue!" << endl;
                }
                else {
                    cout << "Next patient to treat: " << p.name
                        << " (Severity: " << p.severity
                        << ", Arrival Time: " << p.arrivalTime << ")" << endl;
                }
                break;

            case 4:
                cout << "Enter patient name: ";
                getline(cin, name);
                while (name.empty()) {
                    cout << "Name cannot be empty! Please enter patient name: ";
                    getline(cin, name);
                }

                severity = getValidatedInt("Enter new severity (1-100): ", 1, 100);
                emergencyQueue.updateSeverity(name, severity);
                break;

            case 5:
                cout << "Enter patient name: ";
                getline(cin, name);
                while (name.empty()) {
                    cout << "Name cannot be empty! Please enter patient name: ";
                    getline(cin, name);
                }

                p = emergencyQueue.findPatient(name);
                if (p.name != "None") {
                    cout << "Patient found: " << p.name
                        << " (Severity: " << p.severity
                        << ", Arrival Time: " << p.arrivalTime << ")" << endl;
                } else {
                    cout << "Patient not found in the queue." << endl;
                }
                break;

            case 6:
                cout << "Enter patient name to remove: ";
                getline(cin, name);
                while (name.empty()) {
                    cout << "Name cannot be empty! Please enter patient name: ";
                    getline(cin, name);
                }

                emergencyQueue.removePatient(name);
                break;

            case 7:
                emergencyQueue.printHeap();
                break;

            case 8:
                cout << "Average severity: " << emergencyQueue.getAverage() << endl;
                break;

            case 9:
                emergencyQueue.status();
                break;

            case 10:
                cout << "\n===== Treatment History =====" << endl;
                emergencyQueue.displayTreatmentLog();
                break;

            case 11:
                emergencyQueue.saveHistory();
                cout << "Patient history saved to 'EmergencyList'." << endl;
                break;

            case 12:
                cout << "==============================================================="<< endl;
                cout << "12. Exiting Program" << endl;
                cout << "==============================================================="<< endl;
                exit(0);
                break;

            default:
                cout << "Invalid Choice"<<endl;
                break;
            }


        } while (true);
    }
    return 0;
}
