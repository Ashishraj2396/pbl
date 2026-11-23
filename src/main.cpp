#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
using namespace std;

class CCTVEvent {
public:
    string personName, location;
    bool authorized, suspiciousObject;
    string riskLevel;

    void checkAuthorization() {
        ifstream file("data/authorized_personnel.txt");
        string name;
        authorized = false;

        while (getline(file, name)) {
            if (name == personName) {
                authorized = true;
                break;
            }
        }
    }

    void calculateRisk() {
        if (suspiciousObject)
            riskLevel = "HIGH RISK";
        else if (!authorized)
            riskLevel = "MEDIUM RISK";
            else if (authorised)
                riskLevel = "NORMAL RISK";
        else
            riskLevel = "NO RISK";
    }

    void showResult() {
        cout << "\n----- CCTV EVENT REPORT -----\n";
        cout << "Person: " << personName << '\n';
        cout << "Location: " << location << '\n';
        cout << "Authorization: "
             << (authorized ? "AUTHORIZED" : "NOT AUTHORIZED") << '\n';
        cout << "Suspicious object: "
             << (suspiciousObject ? "YES" : "NO") << '\n';
        cout << "Risk Level: " << riskLevel << '\n';

        if (riskLevel != "NORMAL")
            cout << "ALERT: Inform the security team!\n";
        else
            cout << "No alert required.\n";
    }

    void saveLog() {
        ofstream file("events.csv", ios::app);
        time_t now = time(nullptr);
        file << '"' << personName << '"' << ','
             << '"' << location << '"' << ','
             << (authorized ? "YES" : "NO") << ','
             << (suspiciousObject ? "YES" : "NO") << ','
             << '"' << riskLevel << '"' << ','
             << '"' << ctime(&now) << '"' << '\n';
        file.close();
    }
};

int main() {
    cout << "====================================\n";
    cout << " INTELLIGENT CCTV SURVEILLANCE SYSTEM\n";
    cout << "====================================\n";
    cout << "Basic C++ demonstration prototype\n";
    cout << "(Person/object details are entered manually.)\n\n";

    char again = 'y';

    while (again == 'y' || again == 'Y') {
        CCTVEvent event;
        char objectChoice;

        cout << "Enter person name: ";
        getline(cin, event.personName);

        cout << "Enter restricted area/location: ";
        getline(cin, event.location);

        cout << "Was a suspicious/restricted object detected? (y/n): ";
        cin >> objectChoice;
        cin.ignore(10000, '\n');

        event.suspiciousObject =
            (objectChoice == 'y' || objectChoice == 'Y');

        event.checkAuthorization();
        event.calculateRisk();
        event.showResult();
        event.saveLog();

        cout << "\nDo you want to enter another event? (y/n): ";
        cin >> again;
        cin.ignore(10000, '\n');
        cout << '\n';
    }

    cout << "System closed. Event records saved in events.csv\n";
    return 0;
}
