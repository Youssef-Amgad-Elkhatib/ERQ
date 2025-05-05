#include <iostream>
#include <fstream>
#include <sstream>
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
        for(int i=0; i<size; i++){
            if(patient.name== emergencyList[i].name){
                cout << "This patient's name already exists" <<endl;
                return;
            }
        }

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

int main() {
    MaxHeap emergencyQueue;
    int loadChoice;
    int mainChoice;
    string name;
    int severity, arrivalTime;
    patient p;

 while(true){
    cout << "Do you want to:\n";
    cout << "1. Load from history file\n";
    cout << "2. Start with an empty list\n";
    cout << "3. Load test cases from 'TestCases.txt'\n";
    cout << "4. Exit\n";
    cout << "Enter your choice (1-4): ";
    cin >> loadChoice;

    while (loadChoice < 1 || loadChoice > 4) {
                 cout << "Invalid input! Please enter Numbers from 1-4: ";
                 cin >> loadChoice;
             }

             if (loadChoice == 4) {
                 cout << "Exiting program. Goodbye!" << endl;
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
        cout<< "TestCases Finished Successfully"<< endl;
        cout << "==============================================================="<< endl;
        continue;
    }

    do {
        cout << "\n****************** Emergency Room MaxHeap Priority System ******************" << endl;
        cout << "1. Add New Patient" << endl;
        cout << "2. Treat Next Patient (Extract Max)" << endl;
        cout << "3. View Next Patient (Peek Max)" << endl;
        cout << "4. Update Patient Severity" << endl;
        cout << "5. Find Patient Information" << endl;
        cout << "6. Remove Patient from Heap" << endl;
        cout << "7. View Current MaxHeap" << endl;
        cout << "8. View Average Severity" << endl;
        cout << "9. View Status of Average Severity" << endl;
        cout << "10. View Treatment Log" << endl;
        cout << "11. Save Patient History" << endl;
        cout << "12. Return to Main Menu" << endl;
        cout << "Enter your choice (1-12): ";
        cin >> mainChoice;

        while (mainChoice < 1 || mainChoice > 12) {
            cout << "Invalid input! Please enter a number between 1 and 12: ";
            cin >> mainChoice;
        }

        switch (mainChoice) {
        case 1:
            cout << "Enter patient name: ";
            cin >> p.name;
            cout << "Enter severity (1-100): ";
            cin >> p.severity;
            while (p.severity < 1 || p.severity > 100) {
                cout << "Severity must be between 1 and 100! Try again: ";
                cin >> p.severity;
            }
            cout << "Enter arrival time: ";
            cin >> p.arrivalTime;
            emergencyQueue.insert(p);
            cout << "Patient added successfully!" << endl;
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
            cin >> name;
            cout << "Enter new severity (1-100): ";
            cin >> severity;
            while (severity < 1 || severity > 100) {
                cout << "Severity must be between 1 and 100! Try again: ";
                cin >> severity;
            }
            emergencyQueue.updateSeverity(name, severity);
            break;

        case 5:
            cout << "Enter patient name: ";
            cin >> name;
            p = emergencyQueue.findPatient(name);
            if (p.name != "None") {
                cout << "Patient found: " << p.name
                    << " (Severity: " << p.severity
                    << ", Arrival Time: " << p.arrivalTime << ")" << endl;
            }
            break;

        case 6:
            cout << "Enter patient name to remove: ";
            cin >> name;
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
            cout << "12. Returning to Main Menu" << endl;
            cout << "==============================================================="<< endl;
            break;
        }

    } while (mainChoice != 12);

}
    return 0;
}
