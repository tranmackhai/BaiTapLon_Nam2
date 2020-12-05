#include <iostream>
#include <string>
#include <vector>
#include <windows.h>

using namespace std;

void DisableResizeWindow()
{
    HWND hWnd = GetConsoleWindow();
    SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & ~WS_SIZEBOX);
}

void SetWindowSize(SHORT width, SHORT height)
{
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    SMALL_RECT WindowSize;
    WindowSize.Top = 0;
    WindowSize.Left = 0;
    WindowSize.Bottom = height - 1;
    WindowSize.Right = width - 1;

    SetConsoleWindowInfo(hStdout, 1, &WindowSize);
}

void SetScreenBufferSize(SHORT width, SHORT height)
{
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

    COORD NewSize;
    NewSize.X = width;
    NewSize.Y = height;

    SetConsoleScreenBufferSize(hStdout, NewSize);
}

void DisableCtrButton(bool Close, bool Min, bool Max)
{
    HWND hWnd = GetConsoleWindow();
    HMENU hMenu = GetSystemMenu(hWnd, false);
    
    if (Close == 1)
    {
        DeleteMenu(hMenu, SC_CLOSE, MF_BYCOMMAND);
    }
    if (Min == 1)
    {
        DeleteMenu(hMenu, SC_MINIMIZE, MF_BYCOMMAND);
    }
    if (Max == 1)
    {
        DeleteMenu(hMenu, SC_MAXIMIZE, MF_BYCOMMAND);
    }
}

void GoTo(SHORT posX, SHORT posY)
{
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD Position;
    Position.X = posX;
    Position.Y = posY;

    SetConsoleCursorPosition(hStdout, Position);
}

void setFontSize(int FontSize)
{
    CONSOLE_FONT_INFOEX info = {0};
    info.cbSize = sizeof(info);
    info.dwFontSize.Y = FontSize;
    info.FontWeight = FW_NORMAL;
    wcscpy(info.FaceName, L"Lucida Console");
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), NULL, &info);
}

class Employee
{
protected:
    string id;
    string first_name;
    string last_name;
    int age;
    int sex; //(1. male, 2. female)
    string address;
    bool Check;
public:
    virtual void Input();
    virtual void Output();
    virtual int getworkday() = 0;
    string get_firstname()
    {
        return this->first_name;
    }
    string getid()
    {
        return this->id;
    }
    bool getEmployee()
    {
        return this->Check;
    }
    void setEmployee(bool check)
    {
        this->Check = check;
    }
};

void Employee::Input()
{
    cin.ignore();
    cout << "Enter ID of Employee: ";
    getline(cin, id);
    cout << "Enter first name of Employee: ";
    getline(cin, first_name);
    cout << "Enter last name of Employee: ";
    getline(cin, last_name);
    cout << "Enter age of Employee: ";
    cin >> age;
    cin.ignore();
    do
    {
        cout << "Enter sex of Employee (1.male; 2.female): ";
        cin >> sex;
    } while (sex > 2 || sex < 1);
    cout << "Enter address of Employee: ";
    cin.ignore();
    getline(cin, address);
}

void Employee::Output()
{
    cout << "ID: " << id << endl;
    cout << "Name: " << first_name << "\t" << last_name << endl;
    cout << "Age: " << age << endl;
    cout << "Sex: " << sex << endl;
    cout << "Address: " << address << endl;
}

class parTime_Employee : public Employee
{
private:
    int workday;
    int salary;

public:
    void Input();
    void Output();
    int getworkday()
    {
        return this->workday;
    }
};

void parTime_Employee::Input()
{
    Employee::Input();
    do
    {
        cout << "Enter workday: ";
        cin >> workday;
    } while (workday > 30 || workday < 1);
}

void parTime_Employee::Output()
{
    Employee::Output();
    cout << "Work day: " << workday << endl;
    cout << "Salary: " << workday * 300000 << endl;
}

class fullTime_Employee : public Employee
{
private:
    int workday;
    int salary;
    int social_Insurance;
    string position;

public:
    void Input();
    void Output();
    int paySocialInsurance();
    int getworkday()
    {
        return this->workday;
    }
};

void fullTime_Employee::Input()
{
    Employee::Input();
    cout << "Enter position: ";
    getline(cin, position);
    cout << "Enter number work day: ";
    cin >> workday;
    salary = workday * 600000;
}

int fullTime_Employee::paySocialInsurance()
{
    float social_Insurance = 1;
    if (salary < 500000)
        social_Insurance = (salary * 1.3) / 100;
    else
        social_Insurance = (salary * 1.5) / 100;
    return social_Insurance;
}

void fullTime_Employee::Output()
{
    Employee::Output();
    cout << "Position: " << position << endl;
    cout << "Work day: " << workday << endl;
    cout << "Salary: " << salary << endl;
    cout << "Pay Social Insurance: " << paySocialInsurance() << endl;
}

class ListEmployee
{
private:
    vector<Employee *> list_Employee;

public:
    ListEmployee();
    ~ListEmployee();
    void Input();
    void Output();
    void sort_FirstName();
    void sort_Workday();
    void search_Employee();
    void erase_Employee();
    void edit_Employee();
    void check_Expeljob();
};

ListEmployee::ListEmployee() {}
ListEmployee::~ListEmployee() {}

void ListEmployee::Input()
{
    int i = 1, seclect;
    Employee *x;
    while (i)
    {
        system("cls");
        GoTo(50, 0);
        cout << "1. Enter part time Employee";
        GoTo(50, 1);
        cout << "2. Enter full time Employee";
        GoTo(50, 2);
        cout << "3. Exit";
        GoTo(50, 3);
        cout << "Enter your seclect: ";
        cin >> seclect;
        system("cls");
        switch (seclect)
        {
        case 1:
        {
            x = new parTime_Employee;
            x->Input();
            x->setEmployee(true);
            list_Employee.push_back(x);
            break;
        }
        case 2:
        {
            x = new fullTime_Employee;
            x->Input();
            x->setEmployee(false);
            list_Employee.push_back(x);
            break;
        }
        case 3:
        {
            i = 0;
            break;
        }
        default:
        {
            cout << "Invalid seclect !!!";
            break;
        }
        }
    }
}

void ListEmployee ::Output()
{
    int count1 = 0, count2 = 0;
    for (int i = 0; i < list_Employee.size(); i++)
    {
        if (list_Employee[i]->getEmployee() == true)
        {
            cout << "\nPart time employee: " << ++count1 << endl;
            list_Employee[i]->Output();
        }
    }
    for (int i = 0; i < list_Employee.size(); i++)
    {
        if (list_Employee[i]->getEmployee() == false)
        {
            cout << "\nFull time employee: " << ++count2 << endl;
            list_Employee[i]->Output();
        }
    }
}

void ListEmployee::sort_FirstName()
{
    for (int i = 0; i < list_Employee.size() - 1; i++)
    {
        for (int j = i + 1; j < list_Employee.size(); j++)
        {
            if (list_Employee[i]->get_firstname() > list_Employee[j]->get_firstname())
            {
                Employee *temp;
                temp = list_Employee[i];
                list_Employee[i] = list_Employee[j];
                list_Employee[j] = temp;
            }
        }
    }
}

void ListEmployee::sort_Workday()
{
    for (int i = 0; i < list_Employee.size() - 1; i++)
    {
        for (int j = i + 1; j < list_Employee.size(); j++)
        {
            if (list_Employee[i]->getworkday() < list_Employee[j]->getworkday())
            {
                Employee *temp;
                temp = list_Employee[i];
                list_Employee[i] = list_Employee[j];
                list_Employee[j] = temp;
            }
        }
    }
}

void ListEmployee::search_Employee()
{
    string name;
    int found = 0;
    cin.ignore();
    cout << "Enter the name you want search: ";
    getline(cin, name);
    for (int i = 0; i < list_Employee.size(); i++)
    {
        if (name == list_Employee[i]->get_firstname())
        {
            found ++;
            if (list_Employee[i]->getEmployee())
            {
                cout << "\tThe employee to search is the part time employee: " << endl;
            }
            else
            {
                cout << "\tThe employee to search is the full time employee: " << endl;
            }
            list_Employee[i]->Output();
        }
    }
    if (found == 0)
        cout << "No employee found in list!\n";
}

void ListEmployee::erase_Employee()
{
    string ID;
    int found = 0;
    cout << "Enter the ID you want erase: ";
    cin >> ID;
    for (int i = 0; i < list_Employee.size(); i++)
    {
        if (ID == list_Employee[i]->getid())
        {
            found++;
            list_Employee.erase(list_Employee.begin() + i);
        }
    }
    if (found == 0)
        cout << "No ID found in list!\n";
}

void ListEmployee::edit_Employee()
{
    string ID;
    int found;
    cout << "Enter the ID you want edit: ";
    cin >> ID;
    for (int i = 0; i < list_Employee.size(); i++)
    {
        if (ID == list_Employee[i]->getid())
        {
            found++;
            list_Employee[i]->Input();
        }
    }
    if (found == 0)
        cout << "No employee found in list!\n";
}

void ListEmployee::check_Expeljob()
{
    for (int i = 0; i < list_Employee.size(); i++)
    {
        if (list_Employee[i]->getworkday() < 10)
            list_Employee[i]->Output();
    }
}

void menu()
{
    ListEmployee listemp;
    bool check;
    int seclect;
    while (true)
    {
        GoTo(51, 4);
        cout << "EMPLOYEE MANAGER";
        GoTo(50, 7);
        cout << "1. Enter information";
        GoTo(50, 8);
        cout << "2. Show information";
        GoTo(50, 9);
        cout << "3. Sort by first name";
        GoTo(50, 10);
        cout << "4. Sort by work day";
        GoTo(50, 11);
        cout << "5. Search by last name";
        GoTo(50, 12);
        cout << "6. Erase by ID";
        GoTo(50, 13);
        cout << "7. Edit information employee by ID";
        GoTo(50, 14);
        cout << "8. Show employee expel job";
        GoTo(50, 15);
        cout << "0. Exit ";
        GoTo(50, 16);
        cout << "Enter your choice: ";
        cin >> seclect;
        switch (seclect)
        {
        case 1:
        {
            listemp.Input();
            check = true;
            break;
        }
        case 2:
        {
            if (check)
            {
                listemp.Output();
            }
            else
            {
                cout << "You have not entered the information!" << endl;
            }
            break;
        }
        case 3:
        {
            system("cls");
            cout << "\tList Employee after sort by last name " << endl;
            listemp.sort_FirstName();
            listemp.Output();
            break;
        }
        case 4:
        {
            system("cls");
            cout << "\tList employee after sortby work day" << endl;
            listemp.sort_Workday();
            listemp.Output();
            break;
        }
        case 5:
        {
            system("cls");
            cout << "\tSearch employee" << endl;
            listemp.search_Employee();
            break;
        }
        case 6:
        {
            system("cls");
            listemp.erase_Employee();
            cout << "\tList employee after erase" << endl;
            listemp.Output();
            break;
        }
        case 7:
        {
            system("cls");
            listemp.edit_Employee();
            cout << "List employee after edit" << endl;
            listemp.Output();
            break;
        }
        case 8:
        {
            system("cls");
            cout << "List employee expel job";
            listemp.check_Expeljob();
            break;
        }
        case 0:
        {
            exit(0);
        }
        default:
            cout << "Invalid seclect!!!" << endl;
            break;
        }
    }
}

int main()
{
    SetWindowSize(120, 40);
    SetScreenBufferSize(120, 40);
    DisableResizeWindow();
    DisableCtrButton(1,1,1);
    system("color B4");
    setFontSize(16);
    menu();
    system("pause");
}