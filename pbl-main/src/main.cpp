#include <algorithm>
#include <cctype>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

namespace {
const std::string PERSONNEL_FILE = "data/authorized_personnel.txt";
const std::string PLACES_FILE = "data/authorized_places.txt";
const std::string EVENTS_FILE = "events.csv";

std::string trim(const std::string& value) {
    const auto first = value.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    const auto last = value.find_last_not_of(" \t\r\n");
    return value.substr(first, last - first + 1);
}

std::string normalize(const std::string& value) {
    std::string result = trim(value);
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return result;
}

std::string csvEscape(const std::string& value) {
    std::string escaped = "\"";
    for (char c : value) {
        if (c == '"') escaped += "\"\"";
        else escaped += c;
    }
    escaped += '"';
    return escaped;
}

std::string currentTimestamp() {
    const std::time_t now = std::time(nullptr);
    std::tm localTime{};
#ifdef _WIN32
    localtime_s(&localTime, &now);
#else
    localtime_r(&now, &localTime);
#endif
    std::ostringstream output;
    output << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S");
    return output.str();
}

bool readYesNo(const std::string& prompt) {
    while (true) {
        std::cout << prompt << " (y/n): ";
        std::string answer;
        std::getline(std::cin, answer);
        answer = normalize(answer);
        if (answer == "y" || answer == "yes") return true;
        if (answer == "n" || answer == "no") return false;
        std::cout << "Please enter y or n.\n";
    }
}

std::string readRequiredLine(const std::string& prompt) {
    while (true) {
        std::cout << prompt;
        std::string value;
        std::getline(std::cin, value);
        value = trim(value);
        if (!value.empty()) return value;
        std::cout << "This field cannot be empty. Please try again.\n";
    }
}

class CCTVEvent {
public:
    std::string personName;
    std::string location;
    bool authorized = false;
    bool authorizedPlace = false;
    bool suspiciousObject = false;
    std::string riskLevel;
    std::string timestamp;

    void checkAuthorization() {
        std::ifstream file(PERSONNEL_FILE);
        if (!file) {
            std::cerr << "Warning: Could not open " << PERSONNEL_FILE
                      << ". Treating the person as unauthorized.\n";
            authorized = false;
            return;
        }

        std::string name;
        const std::string target = normalize(personName);
        while (std::getline(file, name)) {
            if (normalize(name) == target) {
                authorized = true;
                return;
            }
        }
        authorized = false;
    }

    void checkPlaceAuthorization() {
        std::ifstream file(PLACES_FILE);
        if (!file) {
            std::cerr << "Warning: Could not open " << PLACES_FILE
                      << ". Treating the location as unauthorized.\n";
            authorizedPlace = false;
            return;
        }
        std::string place;
        const std::string target = normalize(location);
        while (std::getline(file, place)) {
            if (normalize(place) == target) {
                authorizedPlace = true;
                return;
            }
        }
        authorizedPlace = false;
    }

    void calculateRisk() {
        if (suspiciousObject || !authorizedPlace) riskLevel = "HIGH RISK";
        else if (!authorized) riskLevel = "MEDIUM RISK";
        else riskLevel = "NORMAL";
    }

    void showResult() const {
        std::cout << "\n========== CCTV EVENT REPORT ==========\n"
                  << "Timestamp          : " << timestamp << '\n'
                  << "Person             : " << personName << '\n'
                  << "Location           : " << location << '\n'
                  << "Person access      : " << (authorized ? "AUTHORIZED" : "NOT AUTHORIZED") << '\n'
                  << "Place access       : " << (authorizedPlace ? "AUTHORIZED" : "RESTRICTED / NOT AUTHORIZED") << '\n'
                  << "Suspicious object  : " << (suspiciousObject ? "YES" : "NO") << '\n'
                  << "Risk level         : " << riskLevel << '\n';

        if (riskLevel == "HIGH RISK")
            std::cout << "ALERT: High-priority event. Notify the security team.\n";
        else if (riskLevel == "MEDIUM RISK")
            std::cout << "ALERT: Verify this person's access.\n";
        else
            std::cout << "Status: No alert required by the current demo rules.\n";
        std::cout << "=======================================\n";
    }

    bool saveLog() const {
        std::ifstream existing(EVENTS_FILE);
        const bool needsHeader = !existing.good() || existing.peek() == std::ifstream::traits_type::eof();
        existing.close();

        std::ofstream file(EVENTS_FILE, std::ios::app);
        if (!file) {
            std::cerr << "Error: Could not write to " << EVENTS_FILE << ".\n";
            return false;
        }

        if (needsHeader)
            file << "timestamp,person,location,person_authorized,place_authorized,suspicious_object,risk_level\n";

        file << csvEscape(timestamp) << ','
             << csvEscape(personName) << ','
             << csvEscape(location) << ','
             << (authorized ? "YES" : "NO") << ','
             << (authorizedPlace ? "YES" : "NO") << ','
             << (suspiciousObject ? "YES" : "NO") << ','
             << csvEscape(riskLevel) << '\n';
        return static_cast<bool>(file);
    }
};

void recordEvent() {
    CCTVEvent event;
    event.personName = readRequiredLine("Enter person name: ");
    event.location = readRequiredLine("Enter area/location: ");
    event.suspiciousObject = readYesNo("Was a suspicious/restricted object reported?");
    event.timestamp = currentTimestamp();

    event.checkAuthorization();
    event.checkPlaceAuthorization();
    event.calculateRisk();
    event.showResult();

    if (event.saveLog())
        std::cout << "Event saved to " << EVENTS_FILE << ".\n";
}

void listPersonnel() {
    std::ifstream file(PERSONNEL_FILE);
    if (!file) {
        std::cerr << "Could not open personnel file: " << PERSONNEL_FILE << '\n';
        return;
    }

    std::cout << "\n======= AUTHORIZED PERSONNEL =======\n";
    std::string name;
    int count = 0;
    while (std::getline(file, name)) {
        name = trim(name);
        if (!name.empty()) {
            std::cout << ++count << ". " << name << '\n';
        }
    }
    if (count == 0) std::cout << "No personnel entries found.\n";
    std::cout << "Total: " << count << "\n====================================\n";
}

void addPersonnel() {
    const std::string name = readRequiredLine("Enter full name to authorize: ");
    std::ifstream input(PERSONNEL_FILE);
    std::string existing;
    while (std::getline(input, existing)) {
        if (normalize(existing) == normalize(name)) {
            std::cout << "This person is already in the authorized list.\n";
            return;
        }
    }

    std::ofstream output(PERSONNEL_FILE, std::ios::app);
    if (!output) {
        std::cerr << "Could not update " << PERSONNEL_FILE << ".\n";
        return;
    }
    output << name << '\n';
    std::cout << "Added " << name << " to the authorized list.\n";
}

void listAuthorizedPlaces() {
    std::ifstream file(PLACES_FILE);
    if (!file) {
        std::cerr << "Could not open location file: " << PLACES_FILE << '\\n';
        return;
    }
    std::cout << "\\n======= AUTHORIZED PLACES =======\\n";
    std::string place;
    int count = 0;
    while (std::getline(file, place)) {
        place = trim(place);
        if (!place.empty()) std::cout << ++count << ". " << place << '\\n';
    }
    if (count == 0) std::cout << "No authorized places found.\\n";
    std::cout << "Total: " << count << "\\n=================================\\n";
}

void addAuthorizedPlace() {
    const std::string place = readRequiredLine("Enter place/area to authorize: ");
    std::ifstream input(PLACES_FILE);
    std::string existing;
    while (std::getline(input, existing)) {
        if (normalize(existing) == normalize(place)) {
            std::cout << "This place is already in the authorized list.\\n";
            return;
        }
    }
    std::ofstream output(PLACES_FILE, std::ios::app);
    if (!output) {
        std::cerr << "Could not update " << PLACES_FILE << ".\\n";
        return;
    }
    output << place << '\\n';
    std::cout << "Added " << place << " to the authorized places list.\\n";
}

void viewEventLog() {
    std::ifstream file(EVENTS_FILE);
    if (!file) {
        std::cout << "No event log exists yet. Record an event first.\n";
        return;
    }

    std::cout << "\n============= EVENT HISTORY =============\n";
    std::string line;
    while (std::getline(file, line)) std::cout << line << '\n';
    std::cout << "==========================================\n";
    std::cout << "Tip: Open events.csv in a spreadsheet app for filtering and reporting.\n";
}

void showMenu() {
    std::cout << "\n========================================\n"
              << "   INTELLIGENT CCTV SURVEILLANCE SYSTEM\n"
              << "========================================\n"
              << "1. Record a CCTV event (manual demo)\n"
              << "2. View authorized personnel\n"
              << "3. Add authorized personnel\n"
              << "4. View authorized places\n"
              << "5. Add authorized place\n"
              << "6. View event history\n"
              << "7. Exit\n"
              << "Choose an option: ";
}
} // namespace

int main() {
    while (true) {
        showMenu();
        std::string choice;
        std::getline(std::cin, choice);

        if (choice == "1") recordEvent();
        else if (choice == "2") listPersonnel();
        else if (choice == "3") addPersonnel();
        else if (choice == "4") listAuthorizedPlaces();
        else if (choice == "5") addAuthorizedPlace();
        else if (choice == "6") viewEventLog();
        else if (choice == "7") {
            std::cout << "System closed. Thank you.\n";
            break;
        } else {
            std::cout << "Invalid choice. Enter a number from 1 to 7.\n";
        }
    }
    return 0;
}
