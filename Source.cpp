#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <ctime>
#include <stdlib.h>
#include <cstdlib>
#include <list>
#include <vector>
#include <conio.h>
#include <stdio.h>

using namespace std;

struct Time{
	time_t t = time(0);
	tm now;

	int Date() {
		localtime_s(&now, &t);
		return now.tm_mday;
	}

	int Month() {
		localtime_s(&now, &t);
		return now.tm_mon + 1;
	}

	int Year() {
		localtime_s(&now, &t);
		return now.tm_year + 1900;
	}
	
};

Time Now;
int CID_Count = 0;

class Date {

private:
	
	int date;
	int month;
	int year;

public :

	Date() {
		date = Now.Date();
		month = Now.Month();;
		year = Now.Year();
	}

	Date(int date, int month, int year) {
		this->date = date;
		this->month = month;
		this->year = year;
	}

	bool IsBetween(int SD, int SM, int SY, int ED, int EM, int EY) {
		int start = SY * 10000 + SM * 100 + SD;
		int end = EY * 10000 + EM * 100 + ED;
		int current = year * 10000 + month * 100 + date;

		return (current >= start) && (current <= end);
	}

	 friend ostream& operator<<(ostream& output,const Date& D){
		output << D.date << "/" << D.month << "/" << D.year;
		return output;
	}

	~Date() {

	}
};

class Employee;

class Complaint {

private:

	int CID;
	string TID;
	string Issue;
	string Dep;
	string Status;
	Date CreationDate; 
	Date * AssignmentDate=nullptr;
	vector < Employee *> AssignedTo;
	string *Solution=nullptr;

public:

	Complaint(string Issue,string Dep,string TID) : CreationDate(Now.Date(), Now.Month(), Now.Year()) {
		CID = ++CID_Count;
		this->Issue = Issue;
		this->Dep = Dep;
		Status = "New";
		this->TID = TID;
	}

	void AssignedComplaintTo(Employee *E) {
		if (AssignmentDate == nullptr) {
			AssignmentDate = new Date(Now.Date(), Now.Month(), Now.Year());
		}
		AssignedTo.push_back(E);
	}

	void ResolveIssue(string Sol,string Emp_ESN);
	
	void RejectComplaint() {
		delete Solution;
		Solution = nullptr;
		Status = "Unresolved";
	}

	void CloseComplaint() {
		AssignedTo.clear();
		Status = "Closed";
	}

	
	string GetStatus() {
		return Status;
	}

	void SetStatus(string Status) {
		this->Status= Status;
	}

	int GetComplaintID( ) {
		return CID;
	}

	void PrintComplaintForTeacher() {
		cout << "Complaint ID : "<< CID << " | Complaint Issue : " << Issue << " | Department : " << Dep << " | Status : " << Status << " | Date : " <<CreationDate;
		if (Solution!=nullptr){
			cout << " | Proposed Solution : " << *Solution;
		}
	}

	Date GetCreationDate() {
		return CreationDate;
	}

	friend ostream& operator<<(ostream& output, const Complaint C) {
		output << "Complaint ID : " << C.CID <<" | Teacher ID : " << C.TID << " | Complaint Issue : " << C.Issue << " | Creation Date : " << C.CreationDate << " | Status : " << C.Status;
		return output;
	}
	
	~Complaint() {

	}

};

class Person {

private:

	string Name;

public:

	Person() {

	}

	Person(string Name) {
		this->Name = Name;
	}

	void ChangeName(string Name) {
		this->Name = Name;
	}

	string GetName() {
		return Name;
	}

	~Person() {

	}

};

class Teacher : public Person {

private:
	
	string TID;
	vector<Complaint*> ComplaintsFiled;

public :

	Teacher(string Name, string TID) :Person(Name) {
		this->TID = TID;
	}

	bool CheckTeacherID(string TID) {
		if (this->TID == TID) {
			return true;
		}
		return false;
	}

	void FileComplaint(Complaint* C) {
		ComplaintsFiled.push_back(C);
	}

	void CloseComplaint(int CID_Index) {
		ComplaintsFiled[CID_Index]->CloseComplaint();
	}
	
	void RejectComplaint(int CID_Index) {
		ComplaintsFiled[CID_Index]->RejectComplaint();
	}
	
	int GetComplaintIndex(int CID, string Status) {
		int ind = -1;
		for (int i = 0; i < ComplaintsFiled.size(); i++) {
				if (ComplaintsFiled[i]->GetStatus() == Status && ComplaintsFiled[i]->GetComplaintID() == CID) {
						ind = i;
					}
		}
		return ind;
	}
	
	string GetTeacherID() {
		return TID;
	}

	void PrintComplaints(string Status) {
		cout << "----------------------------------------------| Complaints |----------------------------------------------" << endl; 
		if (ComplaintsFiled.size() == 0) {
			cout << "No Complaints Found";
		}
		else 
		{
			for (int i = 0; i < ComplaintsFiled.size(); i++) {
				if (Status == "All") {
				ComplaintsFiled[i]->PrintComplaintForTeacher();
				}
				else if (ComplaintsFiled[i]->GetStatus() == Status) {
					ComplaintsFiled[i]->PrintComplaintForTeacher();
				}
				cout << endl;
			}
		}
		cout << endl;
	}

	void ViewComplaint(int ind) {
		ComplaintsFiled[ind]->PrintComplaintForTeacher();
	}

	void StoreInFile(ofstream& fout) {
		fout << "Teacher | " << TID << " | " << GetName()<<endl;
	}

	friend ostream& operator<<(ostream& output, Teacher T) {
		output << T.TID << " " << T.GetName();
		return output;
	}


	~Teacher() {
	
	}

};

class Director : public Person {

public:

	Director(string Name) : Person(Name) {

	}

	void StoreInFile(ofstream& fout) {
		fout << "Director | " << GetName() << endl;
	}

	friend ostream& operator<<(ostream& output, Director D) {
		output << D.GetName() ;
		return output;
	}

	~Director() {

	}

};

class Employee : public Person {

private :

	string ESN;
	string Dep;
	vector<Complaint*> ComplaintsAssigned;

public :

	Employee(string Name, string ESN , string Dep) : Person(Name) {
		this->ESN = ESN;
		this->Dep = Dep;
	}

	void StoreInFile(ofstream& fout,string Dep) {
		fout << "Employee | " << Dep << " | "<<ESN<<" | "<< GetName() << endl;
	}

	bool CheckEmployeeESN(string ESN) {
		if (this->ESN == ESN) {
			return true;
		}
		return false;
	}

	string GetESN() {
		return ESN;
	}

	void AssignComplaint(Complaint* C) {
		ComplaintsAssigned.push_back(C);
	}

	void UnassignComplaint(int CID_Index) {
		ComplaintsAssigned.erase(ComplaintsAssigned.begin() + CID_Index);
	}

	void PrintComplaints() {
		cout << "----------------------------------------------| Assigned Complaints | ----------------------------------------------" << endl;
		for (int i = 0; i < ComplaintsAssigned.size(); i++) {
				cout << (*ComplaintsAssigned[i]);
			}
		}
	
	int GetComplaintIndex(int CID) {
		int ind = -1;
		for (int i = 0; i < ComplaintsAssigned.size(); i++) {
			if (ComplaintsAssigned[i]->GetComplaintID() == CID) {
						ind = i;
					}
				}
		return ind;
	}

	friend ostream& operator<<(ostream& output,Employee E) {
		output << E.ESN << " " << E.GetName();
		return output;
	}

	~Employee() {

	}

};

void Complaint::ResolveIssue(string Sol,string Emp_ESN) {
	if (Solution == nullptr) {
		Solution = new string(Sol);
	}
	Status = "Resolved";
	if (AssignedTo.size() != 0) {
		for (int i = 0; i < AssignedTo.size(); i++) {
			if (AssignedTo[i]->CheckEmployeeESN(Emp_ESN)) {
				AssignedTo.erase(AssignedTo.begin() + i);
			}
		}
	}
}

class Manager : public Person {

private:

	string Dep;

public:

	Manager() {

	}

	Manager(string Name, string Dep) : Person(Name) {
		this->Dep = Dep;
	}

	void ChangeManager(string Name){
		ChangeName(Name);
	}

	void StoreInFile(ofstream& fout,string Dep) {
		fout << "Manager | " << Dep << " | " << GetName()<<endl;
	}

	friend ostream& operator<<(ostream& output,Manager M) {
		output << M.GetName();
		return output;
	}

	~Manager() {

	}

};

class University {

private:

	Director* director=nullptr;
	vector<Teacher*> teachers;

public:

	University() {

	}

	void SetDirector(string Name) {
		if (director == nullptr) {
			director = new Director(Name);
		}
		else {
			director->ChangeName(Name);
		}
	}

	void AddTeacher(string Name,string TID) {
		teachers.push_back(new Teacher(Name, TID));
	}

	bool RemoveTeacher(string TID) {
		for (int i = 0; i < teachers.size(); i++) {
			if (teachers[i]->CheckTeacherID(TID) == true) {
				teachers.erase(teachers.begin() + i);
				return true;
			}
		}
			return false;
		}

	void CloseComplaint(int TID_Index, int CID_Index) {
		teachers[TID_Index]->CloseComplaint(CID_Index);
	}
	
	void RejectComplaint(int TID_Index, int CID_Index) {
		teachers[TID_Index]->RejectComplaint(CID_Index);
	}

	int GetTeacherIndex(string TID) {
		int ind = -1;
		for (int i = 0; i < teachers.size(); i++) {
			if (teachers[i]->CheckTeacherID(TID) == true) {
				ind = i;
			}
		}
		return ind;
	}

	void FileComplaintOfTeacher(int index, Complaint* C) {
		teachers[index]->FileComplaint(C);
	}
	
	int GetComplaintIndex(int Index,int CID, string Status) {
		return teachers[Index]->GetComplaintIndex(CID,Status);
	}

	void ViewComplaint(int TID_Index,int CID_Index) {
		teachers[TID_Index]->ViewComplaint(CID_Index);
	}

	string GetTeacherID(int index) {
		return teachers[index]->GetTeacherID();
	}

	void PrintDirector() {
		cout << "----------------------------------------------| Director |----------------------------------------------" << endl;
		cout << "Name : " <<(*director);
			cout << endl;
		}

	void PrintTeachers() {
		cout << "----------------------------------------------| TEACHERS |----------------------------------------------" << endl;
		cout << "ID    Name" << endl;
		for (int i = 0; i < teachers.size(); i++) {
			cout << (*teachers[i]);
			cout << endl;
		}
	}

	void PrintTeacherComplaints(int Index,string Status) {
		teachers[Index]->PrintComplaints(Status);
	}

	void StoreTeachersInFile(ofstream& fout) {
		for(int i=0;i<teachers.size();i++){
			teachers[i]->StoreInFile(fout);
		}
	}

	void StoreDirectorInFile(ofstream& fout) {
		director->StoreInFile(fout);
	}

	~University() {

	}

};

class Department {

private :

	string Name;
	Manager *manager=nullptr;
	vector<Employee*> employees;
	vector<Complaint*>ComplaintsReceived;

public :

	Department() {

	}

	Department(string Name) {
		this->Name = Name;
	}

	void SetManager(string Name,  string Dep) {
		if (manager == nullptr) {
			manager = new Manager(Name, Dep);
		}
		else {
			manager->ChangeManager(Name);
		}
	}

	void AddEmployee(string Name, string ESN, string Dep) {
		employees.push_back(new Employee(Name,ESN,Dep));
	}

	bool RemoveEmployee(string ESN) {
		for (int i = 0; i < employees.size(); i++) {
			if (employees[i]->CheckEmployeeESN(ESN) == true) {
				employees.erase(employees.begin() + i);
				return true;
			}
		}
			return false;
		}

	int GetEmployeeIndex(string SSN) {
		int ind = -1;
		for (int i = 0; i < employees.size(); i++) {
			if (employees[i]->CheckEmployeeESN(SSN)) {
						ind = i;
					}
				}
		return ind;
	}

	void AssignComplaintToEmployee(int CID_Index, int Emp_Index) {
		employees[Emp_Index]->AssignComplaint(ComplaintsReceived[CID_Index]);
		ComplaintsReceived[CID_Index]->AssignedComplaintTo(employees[Emp_Index]);
		ComplaintsReceived[CID_Index]->SetStatus("Assigned");
	}

	void ResolveComplaintIssue(int Emp_Ind,int CID_Ind,string Sol) {
		employees[Emp_Ind]->UnassignComplaint(CID_Ind);
		ComplaintsReceived[CID_Ind]->ResolveIssue(Sol, employees[Emp_Ind]->GetESN());
	}

	void CompleteComplaintIssue(int ComplaintIndex) {
		ComplaintsReceived[ComplaintIndex]->SetStatus("Complete");
	}
	
	void RejectComplaintIssue(int ComplaintIndex) {
		ComplaintsReceived[ComplaintIndex]->RejectComplaint();
	}
	
	Complaint* ReceiveComplaint(string Issue,string TID) {
		ComplaintsReceived.push_back(new Complaint(Issue, Name,TID));
		return ComplaintsReceived.back();
	}

	int GetComplaintIndex(int CID,string Status) {
		int ind = -1;
		for (int i = 0; i < ComplaintsReceived.size(); i++) {
			if (Status == "Open") {
				if (ComplaintsReceived[i]->GetStatus() != "Closed") {
					if (ComplaintsReceived[i]->GetComplaintID() == CID) {
						ind = i;
					}
				}
			}
			else if (ComplaintsReceived[i]->GetStatus() == Status && ComplaintsReceived[i]->GetComplaintID() == CID) {
						ind = i;
			}
		}
		return ind;
	}

	void PrintManager() {
		cout << "----------------------------------------------| "<<Name<<" Department "<< "|----------------------------------------------" << endl;
		cout << "----------------------------------------------| Manager |----------------------------------------------" << endl;
		cout << "Name : " << (*manager);
		cout << endl;
	}

	void PrintEmployees() {
		cout << "----------------------------------------------| " << Name << " Department " << "|----------------------------------------------" << endl;
		cout << "----------------------------------------------| Employees |----------------------------------------------" << endl;
		cout << "ID    Name" << endl;
		for (int i = 0; i < employees.size(); i++) {
			cout << (*employees[i]);
			cout << endl;
		}
	}

	void PrintEmployeeComplaints(int Index) {
		employees[Index]->PrintComplaints();
	}

	int GetEmployeeComplaintIndex(int Emp_Index,int CID){
		return employees[Emp_Index]->GetComplaintIndex(CID);
	}

	void PrintComplaints(string Status) {
		cout << "----------------------------------------------| " << Name << " Department " << "|----------------------------------------------" << endl;
		cout << "----------------------------------------------| " << Status << " Complaints |----------------------------------------------" << endl;
		for (int i = 0; i < ComplaintsReceived.size(); i++) {
			if (Status == "All") {
				cout << (*ComplaintsReceived[i]);
			}
			else if (Status == "Unresolved") {
				if (Status == ComplaintsReceived[i]->GetStatus() || "New" == ComplaintsReceived[i]->GetStatus() || "Resolved" == ComplaintsReceived[i]->GetStatus()) {
					cout << (*ComplaintsReceived[i]);
				}
			}
			else if (Status == "Pending") {
				if ("Unresolved" == ComplaintsReceived[i]->GetStatus() || "New" == ComplaintsReceived[i]->GetStatus() || "Resolved" == ComplaintsReceived[i]->GetStatus() || "Assigned" == ComplaintsReceived[i]->GetStatus()) {
				cout << (*ComplaintsReceived[i]);
				}
			}
			else if (Status == ComplaintsReceived[i]->GetStatus()) {
				cout << (*ComplaintsReceived[i]);
			}
		}
	}

	void PrintComplaintsSummary(int SD,int SM,int SY,int ED,int EM,int EY) {
		cout << "----------------------------------------------| " << Name << " Department " << "|----------------------------------------------" << endl;
		cout << "----------------------------------------------| Complaints Summary |----------------------------------------------" << endl;
		int ClosedCount = 0, OpenCount = 0, NewCount = 0;
		for (int i = 0; i < ComplaintsReceived.size(); i++) {
			if (ComplaintsReceived[i]->GetCreationDate().IsBetween(SD, SM, SY, ED, EM, EY)) {
				if (ComplaintsReceived[i]->GetStatus() == "Closed") {
					ClosedCount++;
				}
				else if (ComplaintsReceived[i]->GetStatus() == "New") {
					NewCount++;
					OpenCount++;
				}
				else {
					OpenCount++;
				}
			}
		}
		cout << "Total Complaints : " << OpenCount + ClosedCount << " Open Complaints : " << OpenCount << " Closed Complaint : " << ClosedCount << " New Complaints : " << NewCount << endl << endl;
	}

	void ViewComplaint(int ind) {
		cout << (*ComplaintsReceived[ind]);
	}

	void StoreEmployeesInFile(ofstream& fout) {
		for (int i = 0; i < employees.size(); i++) {
			employees[i]->StoreInFile(fout,Name);
		}
	}

	void StoreManagerInFile(ofstream& fout) {
		manager->StoreInFile(fout, Name);
	}

	~Department() {

	}

};

void PopulateData(University &FAST, Department& IT_Dept, Department& Admin_Dept, Department& Accounts_Dept) {

	ifstream fin;

	fin.open("Test.txt");

	while (!fin.eof()) {
		string TempType, TempName;
		fin >> TempType;
		fin.ignore();
		fin.ignore();
		fin.ignore();
		if (TempType == "Teacher") {
			string TempID;
			fin >> TempID;
			fin.ignore();
			fin.ignore();
			fin.ignore();
			getline(fin, TempName);
			FAST.AddTeacher(TempName,TempID);
		}
		else if (TempType == "Director") {
			getline(fin, TempName);
			FAST.SetDirector(TempName);
		}
		else if (TempType == "Manager") {
			string TempDep;
			fin >> TempDep;
			fin.ignore();
			fin.ignore();
			fin.ignore();
			getline(fin, TempName);
			if (TempDep == "IT") {
				IT_Dept.SetManager(TempName, TempDep);
			}
			else if (TempDep == "Accounts") {
				Accounts_Dept.SetManager(TempName, TempDep);
			}
			else if (TempDep == "Admin") {
				Admin_Dept.SetManager(TempName, TempDep);
			}
		}
		else if (TempType == "Employee") {
			string TempDep,TempESN;
			fin >> TempDep;
			fin.ignore();
			fin.ignore();
			fin.ignore();
			fin >> TempESN;
			fin.ignore();
			fin.ignore();
			fin.ignore();
			getline(fin, TempName);	
			if (TempDep == "IT") {
				IT_Dept.AddEmployee(TempName, TempESN ,TempDep);
			}
			else if (TempDep == "Accounts") {
				Accounts_Dept.AddEmployee(TempName, TempESN, TempDep);
			}
			else if (TempDep == "Admin") {
				Admin_Dept.AddEmployee(TempName, TempESN, TempDep);
			}
		}
	}
	fin.close();

	fin.open("Complaints.txt");

	while (!fin.eof()) {
		string TempStatus, TempDep, TempTID, TempDate, TempIssue, TempSol;
		int TempCid, CDay, CMon, CYear;
		int ADay = 0, AMon = 0, AYear = 0;

		fin >> TempStatus;
		fin.ignore(2);
		fin >> TempCid;
		fin.ignore(3);
		fin >> TempDep;
		fin.ignore(3);
		fin >> TempTID;
		fin.ignore(3);
		fin >> TempDate;
		size_t firstDelimiter = TempDate.find('/');
		size_t secondDelimiter = TempDate.find('/', firstDelimiter + 1);
		CDay = stoi(TempDate.substr(0, firstDelimiter));
		CMon = stoi(TempDate.substr(firstDelimiter + 1, secondDelimiter - firstDelimiter - 1));
		CYear = stoi(TempDate.substr(secondDelimiter + 1));
		fin.ignore(4);
		getline(fin, TempIssue, '*');

		if (TempStatus != "New") {
			fin.ignore(3);
			fin >> TempDate;
			firstDelimiter = TempDate.find('/');
			secondDelimiter = TempDate.find('/', firstDelimiter + 1);
			ADay = stoi(TempDate.substr(0, firstDelimiter));
			AMon = stoi(TempDate.substr(firstDelimiter + 1, secondDelimiter - firstDelimiter - 1));
			AYear = stoi(TempDate.substr(secondDelimiter + 1));
		}
		if (TempStatus == "Resolved" || TempStatus == "Complete" || TempStatus == "Closed") {
			fin.ignore(4);
			getline(fin, TempSol, '*');
			fin.ignore();
		}
		if (TempDep == "IT") {
	
		}
		else if (TempDep == "Accounts")
		{

		}
		else if (TempDep == "Admin") {

		}
	}




}

void StoreDataInFile(University& FAST, Department& IT_Dept, Department& Admin_Dept, Department& Accounts_Dept) {
	
	ofstream fout;

	fout.open("Test.txt");

	FAST.StoreTeachersInFile(fout);
	FAST.StoreDirectorInFile(fout);
	IT_Dept.StoreEmployeesInFile(fout);
	Admin_Dept.StoreEmployeesInFile(fout);
	Accounts_Dept.StoreEmployeesInFile(fout);
	IT_Dept.StoreManagerInFile(fout);
	Admin_Dept.StoreManagerInFile(fout);
	Accounts_Dept.StoreManagerInFile(fout);

	fout.close();
}

int main() {
	Department IT_Dept("IT");
	Department Admin_Dept("Admin");
	Department Accounts_Dept("Accounts");
	University FAST;
	PopulateData(FAST, IT_Dept, Admin_Dept, Accounts_Dept);
	string Input;
	string mode;
	Department* DepPtr = nullptr;
	bool Login = false;
	int Index;

	while (true) {
		if (Input == "0") {
			break;
		}
		cout << "Welcome To Complaint Management System" << endl << endl;
		cout << "Press 1 For Admin Mode " << endl;
		cout << "Press 2 For Teacher Mode " << endl;
		cout << "Press 3 For Employee Mode " << endl;
		cout << "Press 4 For Manager Mode " << endl;
		cout << "Press 5 For Director Mode " << endl;
		cout << "Press 0 To Exit " << endl;
		mode = _getch();
		system("CLS");
		while (true) {
			if (mode == "1") {
				cout << "Press 1 To Add/Change User" << endl;
				cout << "Press 2 To Remove User" << endl;
				cout << "Press 3 For Viewing Users" << endl;
				cout << "Press M To Change Mode" << endl;
				cout << "Press 0 To Exit" << endl;
				Input = _getch();
				system("CLS");
				if (Input == "0") {
					break;
				}
				else if (Input == "M") {
					break;
				}
				else if (Input == "1") {
					string TempName;
					cout << "Press 1 To Add Teacher " << endl;
					cout << "Press 2 To Add Employee" << endl;
					cout << "Press 3 To Change Director" << endl;
					cout << "Press 4 To Change Manager" << endl;
					cout << "Press M To Go To Main Menu" << endl;
					Input = _getch();
					system("CLS");
					if (Input == "M") {
						continue;
					}
					else if (Input == "1") {
						string TempID;
						cout << "Enter Teacher Name : ";
						getline(cin, TempName);
						cout << "Enter Teacher ID : ";
						cin >> TempID;
						cin.ignore();
						FAST.AddTeacher(TempName, TempID);
						system("CLS");
						cout << "Entry is Successful" << endl << endl;
						system("PAUSE");
						system("CLS");
					}
					else if (Input == "2") {
						string TempDep, TempESN;
						cout << "Enter Department Name ( IT , Accounts , Admin ) : ";
						cin >> TempDep;
						cin.ignore();
						if (TempDep == "IT") {
							cout << "Enter Employee Serial Number : ";
							cin >> TempESN;
							cout << "Enter Employee Name : ";
							getline(cin, TempName);
							cin.ignore();
							system("CLS");
							IT_Dept.AddEmployee(TempName, "Employee", TempESN);;
							cout << "Entry is Successful" << endl << endl;
						}
						else if (TempDep == "Admin") {
							cout << "Enter Employee Serial Number : ";
							cin >> TempESN;
							cout << "Enter Employee Name : ";
							getline(cin, TempName);
							cin.ignore();
							system("CLS");
							Admin_Dept.AddEmployee(TempName, "Employee", TempESN);
							cout << "Entry is Successful" << endl << endl;
						}
						else if (TempDep == "Accounts") {
							cout << "Enter Employee Serial Number : ";
							cin >> TempESN;
							cout << "Enter Employee Name : ";
							getline(cin, TempName);
							cin.ignore();
							system("CLS");
							Accounts_Dept.AddEmployee(TempName, "Employee", TempESN);
							cout << "Entry is Successful" << endl << endl;
						}
						else {
							cout << "Department is Invalid" << endl << endl;
						}
						system("PAUSE");
						system("CLS");
					}
					else if (Input == "3") {
						cout << "Enter New Director Name : ";
						getline(cin, TempName);
						FAST.SetDirector(TempName);
						system("CLS");
						cout << "Change is Successful" << endl << endl;
						system("PAUSE");
						system("CLS");
					}
					else if (Input == "4") {
						string TempDep;
						cout << "Enter Department Name ( IT , Accounts , Admin ) : ";
						cin >> TempDep;
						cin.ignore();
						if (TempDep == "IT") {
							cout << "Enter New Manager Name : ";
							getline(cin, TempName);
							IT_Dept.SetManager(TempName, "Manager");
							system("CLS");
							cout << "Change is Successful" << endl << endl;
						}
						else if (TempDep == "Admin") {
							cout << "Enter New Manager Name : ";
							getline(cin, TempName);
							Admin_Dept.SetManager(TempName, "Manager");
							system("CLS");
							cout << "Change is Successful" << endl << endl;
						}
						else if (TempDep == "Accounts") {
							cout << "Enter New Manager Name : ";
							getline(cin, TempName);
							Accounts_Dept.SetManager(TempName, "Manager");
							system("CLS");
							cout << "Change is Successful" << endl << endl;
						}
						else {
							cout << "Department is Invalid" << endl << endl;
						}
						system("PAUSE");
						system("CLS");
					}
				}
				else if (Input == "2") {
					cout << "Press 1 To Remove Teacher " << endl;
					cout << "Press 2 To Remove Employee" << endl;
					cout << "Press M To Go To Main Menu" << endl;
					Input = _getch();
					cout << endl;
					system("CLS");
					if (Input == "M") {
						continue;
					}
					else if (Input == "1") {
						string TempID;
						cout << "Enter ID of Teacher to Remove : ";
						cin >> TempID;
						cin.ignore();
						system("CLS");
						if (FAST.RemoveTeacher(TempID) == true) {
							cout << "Teacher has been Removed Successfully" << endl << endl;
						}
						else
							cout << "Teacher with this ID Does Not Exist" << endl << endl;
						system("PAUSE");
						system("CLS");
					}
					else if (Input == "2") {
						string TempDep, TempESN;
						cout << "Enter Department of Employee To Remove  ( IT , Accounts , Admin ) : ";
						cin >> TempDep;
						cin.ignore();
						if (TempDep == "IT") {
							cout << "Enter Employee Serial Number : ";
							cin >> TempESN;
							cin.ignore();
							system("CLS");
							if (IT_Dept.RemoveEmployee(TempESN) == true) {
								cout << "Employee has been Removed Successfully" << endl << endl;
							}
							else
								cout << "Employee with this ESN Does Not Exist" << endl << endl;
						}
						else if (TempDep == "Admin") {
							cout << "Enter Employee Serial Number : ";
							cin >> TempESN;
							cin.ignore();
							system("CLS");
							if (Admin_Dept.RemoveEmployee(TempESN) == true) {
								cout << "Employee has been Removed Successfully" << endl << endl;
							}
							else
								cout << "Employee with this ESN Does Not Exist" << endl << endl;
						}
						else if (TempDep == "Accounts") {
							cout << "Enter Employee Serial Number : ";
							cin >> TempESN;
							cin.ignore();
							system("CLS");
							if (Accounts_Dept.RemoveEmployee(TempESN) == true) {
								cout << "Employee has been Removed Successfully" << endl;
							}
							else
								cout << "Employee with this ESN Does Not Exist" << endl << endl;
						}
						else {
							cout << "Department is Invalid" << endl << endl;
						}
						system("PAUSE");
						system("CLS");
					}
				}
				else if (Input == "3") {
					cout << "Press 1 To View All Users" << endl;
					cout << "Press 2 To View Director" << endl;
					cout << "Press 3 To View All Teachers" << endl;
					cout << "Press 4 To View Employees" << endl;
					cout << "Press 5 To View Managers" << endl;
					cout << "Press M To Go To Main Menu" << endl;
					Input = _getch();
					cout << endl;
					system("CLS");
					if (Input == "M") {
						continue;
					}
					else if (Input == "1") {
						FAST.PrintDirector();
						FAST.PrintTeachers();
						Admin_Dept.PrintManager();
						Admin_Dept.PrintEmployees();
						IT_Dept.PrintManager();
						IT_Dept.PrintEmployees();
						Accounts_Dept.PrintManager();
						Accounts_Dept.PrintEmployees();
					}
					else if (Input == "2") {
						FAST.PrintDirector();
					}
					else if (Input == "3") {
						FAST.PrintTeachers();
					}
					else if (Input == "4") {
						cout << "Press 1 To View Employees of a Specific Department" << endl;
						cout << "Press 2 To View Employees of All Department" << endl;
						cout << "Press M To Go To Main Menu" << endl;
						Input = _getch();
						cout << endl;
						system("CLS");
						if (Input == "M") {
							continue;
						}
						else if (Input == "1") {
							string TempDep;
							cout << "Enter Department Name ( IT , Accounts , Admin ) : ";
							cin >> TempDep;
							cin.ignore();
							if (TempDep == "IT") {
								IT_Dept.PrintEmployees();
							}
							else if (TempDep == "Admin") {
								Admin_Dept.PrintEmployees();
							}
							else if (TempDep == "Accounts") {
								Accounts_Dept.PrintEmployees();
							}
							else {
								cout << "Department is Invalid";
							}
						}
						else if (Input == "2") {
							IT_Dept.PrintEmployees();
							Admin_Dept.PrintEmployees();
							Accounts_Dept.PrintEmployees();
						}
					}
					else if (Input == "5") {
						cout << "Press 1 To View Manager of a Specific Department" << endl;
						cout << "Press 2 To View Manager of All Department" << endl;
						cout << "Press 0 To Go " << endl;
						Input = _getch();
						cout << endl;
						system("CLS");
						if (Input == "0") {
							continue;
						}
						else if (Input == "1") {
							string TempDep;
							cout << "Enter Department Name ( IT , Accounts , Admin ) : ";
							cin >> TempDep;
							cin.ignore();
							if (TempDep == "IT") {
								IT_Dept.PrintManager();
							}
							else if (TempDep == "Admin") {
								Admin_Dept.PrintManager();
							}
							else if (TempDep == "Accounts") {
								Accounts_Dept.PrintManager();
							}
							else {
								cout << "Department is Invalid" << endl << endl;
							}
						}
						else if (Input == "2") {
							IT_Dept.PrintManager();
							Admin_Dept.PrintManager();
							Accounts_Dept.PrintManager();
						}
					}
					cout << endl << endl;
					system("PAUSE");
					system("CLS");
				}
			}
			else if (mode == "2") {
				if (Login == false) {
					string TID;
					cout << "Enter Teaher ID : ";
					cin >> TID;
					cin.ignore();
					Index = FAST.GetTeacherIndex(TID);
					if (Index == -1) {
						cout << "Teacher ID is Invalid";
						cout << endl << endl;
						system("PAUSE");
						system("CLS");
						break;
					}
					else {
						Login = true;
					}
				}
				system("CLS");
				if (Login == true) {
					cout << "Enter 1 To View Your Complaints" << endl;
					cout << "Enter 2 To File a Complaint or Requirement of Any Service or Equipment" << endl;
					cout << "Enter 3 To Provide Feedback of Complete Complaints" << endl;
					cout << "Enter M To Logout And Change Mode" << endl;
					cout << "Enter 0 To Logout And Exit" << endl;
					Input = _getch();
					system("CLS");
					if (Input == "0") {
						Login = false;
						break;
					}
					else if (Input == "M") {
						Login = false;
						break;
					}
					else if (Input == "1") {
						cout << "Enter 1 To View All Complaints" << endl;
						cout << "Enter 2 To View New Complaints" << endl;
						cout << "Enter 3 To View Complete Complaints" << endl;
						cout << "Enter 4 To View Closed Complaints" << endl;
						cout << "Enter M To Go To Main Menu" << endl;
						Input = _getch();
						system("CLS");
						if (Input == "M") {
							continue;
						}
						else if (Input == "1") {
							FAST.PrintTeacherComplaints(Index, "All");
						}
						else if (Input == "2") {
							FAST.PrintTeacherComplaints(Index, "New");
						}
						else if (Input == "3") {
							FAST.PrintTeacherComplaints(Index, "Complete");
						}
						else if (Input == "4") {
							FAST.PrintTeacherComplaints(Index, "Closed");
						}
					}
					else if (Input == "2") {
						string TempIssue, TempDep;
						cout << "Enter Support Department (Admin , IT , Accounts ) : ";
						cin >> TempDep;
						cin.ignore();
						system("CLS");
						if (TempDep == "IT") {
							cout << "Write Details of Complaint or Requirment of Any Service or Equipment : ";
							getline(cin, TempIssue);
							FAST.FileComplaintOfTeacher(Index, IT_Dept.ReceiveComplaint(TempIssue, FAST.GetTeacherID((Index))));
							system("CLS");
							cout << "Your Complaint/Message has been sent to IT Department";
						}
						else if (TempDep == "Accounts") {
							cout << "Write Details of Complaint or Requirment of Any Service or Equipment : ";
							getline(cin, TempIssue);
							system("CLS");
							FAST.FileComplaintOfTeacher(Index, Accounts_Dept.ReceiveComplaint(TempIssue, FAST.GetTeacherID((Index))));
							cout << "Your Complaint/Message has been sent to Accounts Department";
						}
						else if (TempDep == "Admin") {
							cout << "Write Details of Complaint or Requirment of Any Service or Equipment : ";
							getline(cin, TempIssue);
							system("CLS");
							FAST.FileComplaintOfTeacher(Index, Admin_Dept.ReceiveComplaint(TempIssue, FAST.GetTeacherID((Index))));
							cout << "Your Complaint/Message has been sent to Admin Department";
						}
						else {
							cout << "Department is Invalid";
						}
					}
					else if (Input == "3") {
						int TempCID, CID_Index;
						FAST.PrintTeacherComplaints(Index, "Complete");
						cout << endl << endl;
						cout << "Enter Complaint ID of Complaint You Want To Provide Feeback : ";
						cin >> TempCID;
						cin.ignore();
						system("CLS");
						CID_Index = FAST.GetComplaintIndex(Index, TempCID, "Complete");
						if (CID_Index == -1) {
							cout << "Complaint ID is Invalid" << endl;
						}
						else {
							FAST.ViewComplaint(Index, CID_Index);
							cout << endl << endl;
							cout << "Press 1 if You are Satisfied" << endl;
							cout << "Press 2 if You are Not Satisfied" << endl;
							Input = _getch();
							system("CLS");
							if (Input == "1") {
								FAST.CloseComplaint(Index, CID_Index);
								cout << "Thanks For Your Feedback . Your Complaint has Been Closed" << endl;
							}
							else if (Input == "2") {
								FAST.RejectComplaint(Index, CID_Index);
								cout << "Thanks For Your Feedback . Your Complaint has Been Sent To Relevant Department Again" << endl;
							}
						}
					}
					cout << endl << endl;
					system("PAUSE");
					system("CLS");
				}
			}
			else if (mode == "3") {
				if (Login == false) {
					string SSN, TempDep;
					int EmpIndex = -1;
					cout << "Enter Your Department ( Admin , IT , Accounts ) : ";
					cin >> TempDep;
					cin.ignore();
					if (TempDep == "IT") {
						DepPtr = &IT_Dept;
					}
					else if (TempDep == "Accounts") {
						DepPtr = &Accounts_Dept;
					}
					else if (TempDep == "Admin") {
						DepPtr = &Admin_Dept;
					}
					else {
						cout << "Department is Invalid" << endl;
					}
					cout << "Enter Employee Serial Number : ";
					cin >> SSN;
					cin.ignore();
					Index = DepPtr->GetEmployeeIndex(SSN);
					if (Index == -1) {
						cout << "Employee Serial Number is Invalid";
						cout << endl << endl;
						system("PAUSE");
						system("CLS");
						break;
					}
					else {
						Login = true;
					}
				}
				system("CLS");
				if (Login == true) {
					cout << "Enter 1 To View Assigned Complaints " << endl;
					cout << "Enter 2 To Resolve Issue of Complaints" << endl;
					cout << "Enter M To Logout And Change Mode" << endl;
					cout << "Enter 0 To Logout And Exit" << endl;
					Input = _getch();
					system("CLS");
					if (Input == "0") {
						Login = false;
						break;
					}
					else if (Input == "M") {
						Login = false;
						break;
					}
					else if (Input == "1") {
						DepPtr->PrintEmployeeComplaints(Index);
						cout << endl << endl;
					}
					else if (Input == "2") {
						int CID_Index;
						int TempCID;
						string TempSolution;
						DepPtr->PrintEmployeeComplaints(Index);
						cout << endl << endl;
						cout << "Enter Complaint ID To Reslove its Issue : ";
						cin >> TempCID;
						cin.ignore();
						system("CLS");
						CID_Index = DepPtr->GetEmployeeComplaintIndex(Index, TempCID);
						if (CID_Index == -1) {
							cout << "Complaint ID is Invalid" << endl;
						}
						else {
							cout << "Enter Resolve Details of Issue : ";
							getline(cin, TempSolution);
							DepPtr->ResolveComplaintIssue(Index, CID_Index, TempSolution);
							system("CLS");
							cout << "Complaint has been Resloved and Sent To Manager For Approval";
						}
					}
				}
				cout << endl << endl;
				system("PAUSE");
				system("CLS");
			}
			else if (mode == "4") {
				if (Login == false) {
					string TempDep;
					cout << "Enter Your Department ( Admin , IT , Accounts ) : ";
					cin >> TempDep;
					cin.ignore();
					if (TempDep == "IT") {
						DepPtr = &IT_Dept;
						Login = true;
					}
					else if (TempDep == "Accounts") {
						DepPtr = &Accounts_Dept;
						Login = true;
					}
					else if (TempDep == "Admin") {
						DepPtr = &Admin_Dept;
						Login = true;
					}
					else {
						cout << "Department is Invalid" << endl;
					}
				}
				system("CLS");
				if (Login == true) {
					cout << "Enter 1 To View Complaints" << endl;
					cout << "Enter 2 To Assign Complaints To Employees" << endl;
					cout << "Enter 3 To Approve and Complete Resolved Complaints " << endl;
					cout << "Enter M To Logout And Change Mode" << endl;
					cout << "Enter 0 To Logout And Exit" << endl;
					Input = _getch();
					system("CLS");
					if (Input == "0") {
						Login = false;
						DepPtr = nullptr;
						break;
					}
					else if (Input == "M") {
						Login = false;
						DepPtr = nullptr;
						break;
					}
					else if (Input == "1") {
						cout << "Enter 1 To View All Complaints" << endl;
						cout << "Enter 2 To View New Complaints" << endl;
						cout << "Enter 3 To View Assigned Complaints" << endl;
						cout << "Enter 4 To View Resolved Complaints" << endl;
						cout << "Enter 5 To View Complete Complaints" << endl;
						cout << "Enter 6 To View Unresolved Complaints" << endl;
						cout << "Enter 7 To View Closed Complaints" << endl;
						cout << "Enter M To Go To Main Menu" << endl;
						Input = _getch();
						system("CLS");
						if (Input == "M") {
							continue;
						}
						else if (Input == "1") {
							DepPtr->PrintComplaints("All");
						}
						else if (Input == "2") {
							DepPtr->PrintComplaints("New");
						}
						else if (Input == "3") {
							DepPtr->PrintComplaints("Assigned");
						}
						else if (Input == "4") {
							DepPtr->PrintComplaints("Resolved");
						}
						else if (Input == "5") {
							DepPtr->PrintComplaints("Complete");
						}
						else if (Input == "6") {
							DepPtr->PrintComplaints("Unresolved");
						}
						else if (Input == "7") {
							DepPtr->PrintComplaints("Closed");
						}
					}
					else if (Input == "2") {
						int TempCID, CID_Index;
						string TempESN;
						DepPtr->PrintComplaints("Pending");
						cout << endl << endl;
						cout << "Enter Complaint ID To Assign : ";
						cin >> TempCID;
						cin.ignore();
						system("CLS");
						Index = DepPtr->GetComplaintIndex(TempCID, "Open");
						if (Index == -1) {
							cout << "Complaint ID is Invalid" << endl;
						}
						else {
							CID_Index = Index;
							system("CLS");
							DepPtr->ViewComplaint(Index);
							cout << endl;
							DepPtr->PrintEmployees();
							cout << endl << endl;
							cout << "Enter Employee Serial Number To Assign Complaint : ";
							cin >> TempESN;
							cin.ignore();
							system("CLS");
							Index = DepPtr->GetEmployeeIndex(TempESN);
							if (Index == -1) {
								cout << "Employee Serial Number is Invalid" << endl;
							}
							else {
								DepPtr->AssignComplaintToEmployee(CID_Index, Index);
								cout << "Complaint has been Assigned To Employee Successfully" << endl;
							}
						}
					}
					else if (Input == "3") {
						int TempCID;
						DepPtr->PrintComplaints("Resolved");
						cout << endl << endl;
						cout << "Enter Complaint ID To Complete its Approval And Sent To Teacher : ";
						cin >> TempCID;
						cin.ignore();
						system("CLS");
						Index = DepPtr->GetComplaintIndex(TempCID, "Resolved");
						if (Index == -1) {
							cout << "Complaint ID is Invalid" << endl;
						}
						else {
							DepPtr->ViewComplaint(Index);
							cout << endl << endl;
							cout << "Press 1 To Approve" << endl;
							cout << "Press 2 To Reject" << endl;
							Input = _getch();
							system("CLS");
							if (Input == "1") {
								DepPtr->CompleteComplaintIssue(Index);
								cout << "Complaint has been Approved and Sent To Teacher";
							}
							else if (Input == "2") {
								DepPtr->RejectComplaintIssue(Index);
								cout << "Complaint has been Rejected ";
							}
						}
					}
				}
				cout << endl << endl;
				system("PAUSE");
				system("CLS");
			}
			else if (mode == "5") {
				Login = true;
				cout << "Enter 1 To View Summary of the Complaints within a Given Period of Time" << endl;
				cout << "Enter 2 To View Complaints " << endl;
				cout << "Enter M To Logout And Change Mode" << endl;
				cout << "Enter 0 To Logout And Exit" << endl;
				Input = _getch();
				system("CLS");
				if (Input == "0") {
					Login = false;
					DepPtr = nullptr;
					break;
				}
				else if (Input == "M") {
					Login = false;
					break;
				}
				else if (Input == "1") {
					int TempSDay, TempSMon, TempSYear, TempEDay, TempEMon, TempEYear;
					cout << "Enter Starting Date : " << endl;
					cout << "Enter Date of Month : ";
					cin >> TempSDay;
					cin.ignore();
					cout << "Enter Month ( Numerical Value ) : ";
					cin >> TempSMon;
					cin.ignore();
					cout << "Enter Year ( Numerical Value ) : ";
					cin >> TempSYear;
					cin.ignore();
					cout << endl;
					cout << "Enter Ending Date : " << endl;
					cout << "Enter Date of Month : ";
					cin >> TempEDay;
					cin.ignore();
					cout << "Enter Month ( Numerical Value ) : ";
					cin >> TempEMon;
					cin.ignore();
					cout << "Enter Year ( Numerical Value ) : ";
					cin >> TempEYear;
					cin.ignore();
					IT_Dept.PrintComplaintsSummary(TempSDay, TempSMon, TempSYear, TempEDay, TempEMon, TempEYear);
					cout << endl;
					Admin_Dept.PrintComplaintsSummary(TempSDay, TempSMon, TempSYear, TempEDay, TempEMon, TempEYear);
					cout << endl;
					Accounts_Dept.PrintComplaintsSummary(TempSDay, TempSMon, TempSYear, TempEDay, TempEMon, TempEYear);
					cout << endl;
					cout << endl;
					IT_Dept.PrintComplaints("All");
					cout << endl;
					Admin_Dept.PrintComplaints("All");
					cout << endl;
					Accounts_Dept.PrintComplaints("All");
					cout << endl;
				}
				if (Input == "2") {
					cout << "Enter 1 To View All Complaints" << endl;
					cout << "Enter 2 To View New Complaints" << endl;
					cout << "Enter 3 To View Assigned Complaints" << endl;
					cout << "Enter 4 To View Resolved Complaints" << endl;
					cout << "Enter 5 To View Complete Complaints" << endl;
					cout << "Enter 6 To View Unresolved Complaints" << endl;
					cout << "Enter 7 To View Closed Complaints" << endl;
					cout << "Enter M To Go To Main Menu" << endl;
					Input = _getch();
					system("CLS");
					if (Input == "M") {
						continue;
					}
					else if (Input == "1") {
						IT_Dept.PrintComplaints("All");
						cout << endl;
						Admin_Dept.PrintComplaints("All");
						cout << endl;
						Accounts_Dept.PrintComplaints("All");
						cout << endl;
					}
					else if (Input == "2") {
						IT_Dept.PrintComplaints("New");
						cout << endl;
						Admin_Dept.PrintComplaints("New");
						cout << endl;
						Accounts_Dept.PrintComplaints("New");
						cout << endl;
					}
					else if (Input == "3") {
						IT_Dept.PrintComplaints("Assigned");
						cout << endl;
						Admin_Dept.PrintComplaints("Assigned");
						cout << endl;
						Accounts_Dept.PrintComplaints("Assigned");
						cout << endl;
					}
					else if (Input == "4") {
						IT_Dept.PrintComplaints("Resolved");
						cout << endl;
						Admin_Dept.PrintComplaints("Resolved");
						cout << endl;
						Accounts_Dept.PrintComplaints("Resolved");
						cout << endl;
					}
					else if (Input == "5") {
						IT_Dept.PrintComplaints("Complete");
						cout << endl;
						Admin_Dept.PrintComplaints("Complete");
						cout << endl;
						Accounts_Dept.PrintComplaints("Complete");
						cout << endl;
					}
					else if (Input == "6") {
						IT_Dept.PrintComplaints("Unresolved");
						cout << endl;
						Admin_Dept.PrintComplaints("Unresolved");
						cout << endl;
						Accounts_Dept.PrintComplaints("Unresolved");
						cout << endl;
					}
					else if (Input == "7") {
						IT_Dept.PrintComplaints("Closed");
						cout << endl;
						Admin_Dept.PrintComplaints("Closed");
						cout << endl;
						Accounts_Dept.PrintComplaints("Closed");
						cout << endl;
					}
				}
			}
			else if (mode == "0") {
				Input = '0';
				break;
			}
			else {
				Input = '0';
				break;
			}
			
		}
		StoreDataInFile(FAST, IT_Dept, Admin_Dept, Accounts_Dept);
	}
	}


	




