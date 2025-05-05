#include <iostream>
#include <fstream>
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
        fstream treatment("TreatmentLog",ios::app);
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
        ifstream history("EmergencyList");
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
        ofstream history("EmergencyList");
        history<<"Patients"<<endl<<"========================================================="<<endl;
        for(int i=0; i<size; i++){
            history<<emergencyList[i].name<<" "<<emergencyList[i].severity<<" "<<emergencyList[i].arrivalTime<<endl;
        }
    }

    void displayTreatmentLog(){
        ifstream Treatment("TreatmentLog");
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

int main() {
//    MaxHeap queue(2); // Small initial capacity to test resizing
//
//    // Simulated patient data
//    patient patients[] = {
//            {"Tom", 60, 1},
//            {"Linda", 85, 2},
//            {"Steve", 85, 1},   // Same severity as Linda but earlier arrival
//            {"Rachel", 90, 3},
//            {"Mike", 75, 4},
//            {"Nina", 70, 5},
//            {"Oscar", 95, 6},
//            {"Paul", 65, 7},
//            {"Quincy", 85, 8},  // Same severity as Linda/Steve, later arrival
//            {"Sophie", 100, 9}
//    };
//
//    // Insert patients
//    cout << "Heap After Insertions:" << endl;
//    for (int i = 0; i < 10; ++i) {
//        cout << "Inserting: " << patients[i].name << endl;
//        queue.insert(patients[i]);
//        queue.printHeap();
//    }

//
//    // Extract patients in treatment order
//    cout << "\nTreatment Order :" << endl;
//    for (int i = 0; i < 10; ++i) {
//        patient treated = queue.extractMax();
//        cout << "Treating: " << treated.name << endl;
//    }
//
//    queue.displayTreatmentLog();
//    queue.saveHistory();
}

