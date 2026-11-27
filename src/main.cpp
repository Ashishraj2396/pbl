#include <algorithm>

#include <cctype>

#include <ctime>

#include <fstream>
#include <filesystem>
#include <cstdlib>

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

        std::cerr << "Could not open location file: " << PLACES_FILE << '\n';

return;

    }

    std::cout << "\n======= AUTHORIZED PLACES =======\n";

    std::string place;

int count = 0;

while (std::getline(file, place)) {

        place = trim(place);

if (!place.empty()) std::cout << ++count << ". " << place << '\n';

    }

if (count == 0) std::cout << "No authorized places found.\n";

    std::cout << "Total: " << count << "\n=================================\n";

}



void addAuthorizedPlace() {

const std::string place = readRequiredLine("Enter place/area to authorize: ");

    std::ifstream input(PLACES_FILE);

    std::string existing;

while (std::getline(input, existing)) {

if (normalize(existing) == normalize(place)) {

            std::cout << "This place is already in the authorized list.\n";

return;

        }

    }

    std::ofstream output(PLACES_FILE, std::ios::app);

if (!output) {

        std::cerr << "Could not update " << PLACES_FILE << ".\n";

return;

    }

    output << place << '\n';

    std::cout << "Added " << place << " to the authorized places list.\n";

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




void deleteEntry(const std::string& path, const std::string& heading) {
    std::ifstream input(path);
    if (!input) { std::cerr << "Could not open file: " << path << '\n'; return; }
    std::vector<std::string> entries;
    std::string line;
    while (std::getline(input, line)) {
        line = trim(line);
        if (!line.empty()) entries.push_back(line);
    }
    if (entries.empty()) { std::cout << "No entries available to delete.\n"; return; }
    std::cout << "\n===== " << heading << " =====\n";
    for (std::size_t i = 0; i < entries.size(); ++i)
        std::cout << i + 1 << ". " << entries[i] << '\n';
    std::cout << "Choose entry number (0 to cancel): ";
    std::string raw;
    std::getline(std::cin, raw);
    std::size_t index = 0;
    try { index = static_cast<std::size_t>(std::stoul(raw)); }
    catch (...) { std::cout << "Invalid selection.\n"; return; }
    if (index == 0) { std::cout << "Deletion cancelled.\n"; return; }
    if (index > entries.size()) { std::cout << "Invalid selection.\n"; return; }
    if (!readYesNo("Delete '" + entries[index - 1] + "'? Confirm")) {
        std::cout << "Deletion cancelled.\n"; return;
    }
    std::ofstream output(path, std::ios::trunc);
    if (!output) { std::cerr << "Could not write to file: " << path << '\n'; return; }
    for (std::size_t i = 0; i < entries.size(); ++i)
        if (i != index - 1) output << entries[i] << '\n';
    std::cout << "Deleted successfully.\n";
}

void deletePersonnel() { deleteEntry(PERSONNEL_FILE, "AUTHORIZED PERSONNEL"); }
void deleteAuthorizedPlace() { deleteEntry(PLACES_FILE, "AUTHORIZED PLACES"); }

const std::string IMAGE_DIRECTORY = "data/images";

bool isSupportedImage(const std::filesystem::path& path) {
    std::string ext = path.extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return ext == ".jpg" || ext == ".jpeg" || ext == ".png" ||
           ext == ".bmp" || ext == ".webp";
}

std::string shellQuote(const std::string& value) {
    std::string quoted = "'";
    for (char c : value) {
        if (c == '\'') quoted += "'\\''";
        else quoted += c;
    }
    quoted += "'";
    return quoted;
}

void addImage() {
    namespace fs = std::filesystem;
    const std::string source = readRequiredLine("Enter image path to add (example: test.png): ");
    const fs::path sourcePath(source);
    std::error_code error;
    if (!fs::is_regular_file(sourcePath, error)) {
        std::cout << "Image file not found: " << source << '\n'; return;
    }
    if (!isSupportedImage(sourcePath)) {
        std::cout << "Unsupported format. Use JPG, JPEG, PNG, BMP or WEBP.\n"; return;
    }
    fs::create_directories(IMAGE_DIRECTORY, error);
    if (error) { std::cerr << "Could not create image folder: " << error.message() << '\n'; return; }
    const fs::path destination = fs::path(IMAGE_DIRECTORY) / sourcePath.filename();
    if (fs::exists(destination)) {
        std::cout << "An image with this filename already exists.\n"; return;
    }
    fs::copy_file(sourcePath, destination, fs::copy_options::none, error);
    if (error) { std::cerr << "Could not copy image: " << error.message() << '\n'; return; }
    std::cout << "Image added successfully: " << destination.string() << '\n';
}

void detectImage() {
    namespace fs = std::filesystem;
    std::vector<fs::path> images;
    std::error_code error;
    if (fs::exists(IMAGE_DIRECTORY, error)) {
        for (const auto& entry : fs::directory_iterator(IMAGE_DIRECTORY, error)) {
            if (!error && entry.is_regular_file() && isSupportedImage(entry.path()))
                images.push_back(entry.path());
        }
    }
    std::sort(images.begin(), images.end());
    if (images.empty()) { std::cout << "No saved images. Use option 9 to add an image first.\n"; return; }
    std::cout << "\n========== SAVED IMAGES ==========\n";
    for (std::size_t i = 0; i < images.size(); ++i)
        std::cout << i + 1 << ". " << images[i].filename().string() << '\n';
    std::cout << "Choose image number (0 to cancel): ";
    std::string raw; std::getline(std::cin, raw);
    std::size_t index = 0;
    try { index = static_cast<std::size_t>(std::stoul(raw)); }
    catch (...) { std::cout << "Invalid selection.\n"; return; }
    if (index == 0) { std::cout << "Detection cancelled.\n"; return; }
    if (index > images.size()) { std::cout << "Invalid selection.\n"; return; }
    if (!fs::is_regular_file("yolo11n.pt")) {
        std::cerr << "Model file yolo11n.pt not found in project root.\n"; return;
    }
    const std::string output = "detection_output.jpg";
    const std::string command = "./.venv/bin/python scripts/yolo_image_demo.py --model " +
        shellQuote("yolo11n.pt") + " --image " + shellQuote(images[index - 1].string()) +
        " --output " + shellQuote(output);
    std::cout << "Running YOLO detection...\n";
    const int status = std::system(command.c_str());
    if (status != 0) { std::cerr << "YOLO detection failed. Check the Python error above.\n"; return; }
    std::cout << "Detection completed. Opening " << output << "...\n";
    std::system(("open " + shellQuote(output)).c_str());
}


std::filesystem::path selectLocalFile(
    const std::string& directory,
    const std::vector<std::string>& extensions
) {
    namespace fs = std::filesystem;

    std::vector<fs::path> files;
    std::error_code error;

    if (!fs::is_directory(directory, error)) {
        std::cout << "Folder not available: " << directory << '\n';
        return {};
    }

    fs::directory_iterator it(directory, error), end;

    while (!error && it != end) {
        std::error_code fileError;

        if (it->is_regular_file(fileError)) {
            std::string ext = normalize(
                it->path().extension().string()
            );

            if (std::find(extensions.begin(), extensions.end(), ext)
                != extensions.end()) {
                files.push_back(it->path());
            }
        }

        it.increment(error);
    }

    if (error) {
        std::cerr << "Could not read folder.\n";
        return {};
    }

    std::sort(files.begin(), files.end());

    if (files.empty()) {
        std::cout << "No supported files available.\n";
        return {};
    }

    for (std::size_t i = 0; i < files.size(); ++i) {
        std::cout << i + 1 << ". "
                  << files[i].filename().string() << '\n';
    }

    std::cout << "Choose file number (0 to cancel): ";

    std::string raw;
    std::getline(std::cin, raw);
    raw = trim(raw);

    try {
        std::size_t used = 0;
        unsigned long index = std::stoul(raw, &used);

        if (used != raw.size() || index > files.size()) {
            std::cout << "Invalid selection.\n";
            return {};
        }

        if (index == 0) return {};

        return files[index - 1];

    } catch (...) {
        std::cout << "Invalid selection.\n";
        return {};
    }
}

void analyzeSampleVideo() {
    std::cout << "\n======= SAMPLE VIDEOS =======\n";

    const auto video = selectLocalFile(
        "data/videos", {".mp4", ".mov", ".avi"}
    );

    if (video.empty()) return;

    const std::string command =
        "./.venv/bin/python scripts/yolo_video_demo.py --video "
        + shellQuote(video.string())
        + " --model yolo11n.pt --stride 10 --conf 0.50 --report";

    std::cout << "Running sample video analysis...\n" << std::flush;

    const int status = std::system(command.c_str());

    if (status != 0) {
        std::cerr << "Video analysis failed.\n";
        return;
    }

    std::cout << "Video analysis completed successfully.\n";
}

void viewVideoReports() {
    std::cout << "\n======= VIDEO REPORTS =======\n";

    const auto report = selectLocalFile(
        "video_results", {".json"}
    );

    if (report.empty()) return;

    std::ifstream file(report);

    if (!file) {
        std::cerr << "Could not open report.\n";
        return;
    }

    std::cout << "\n======= ANALYSIS REPORT =======\n";
    std::cout << file.rdbuf();
    std::cout << "\n===============================\n";
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
              << "7. Delete authorized personnel\n"
              << "8. Delete authorized place\n"
              << "9. Add image to image library\n"
              << "10. Detect objects from saved images\n"
              << "11. Review AI detection summary\n"
              << "12. View AI review history\n"
              << "13. Analyze sample video\n"
              << "14. View video analysis reports\n"
              << "15. Launch GUI Dashboard\n"
              << "16. Exit\n"
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
        else if (choice == "7") deletePersonnel();
        else if (choice == "8") deleteAuthorizedPlace();
        else if (choice == "9") addImage();
        else if (choice == "10") detectImage();
        else if (choice == "11") {
            int status = std::system(
                "./.venv/bin/python scripts/review_detection.py"
            );
            if (status != 0)
                std::cerr << "Review failed. Check the error above.\n";
        }
        else if (choice == "12") {
            int status = std::system(
                "./.venv/bin/python scripts/view_review_history.py"
            );
            if (status != 0)
                std::cerr << "History viewer failed.\n";
        }
        else if (choice == "13") analyzeSampleVideo();
        else if (choice == "14") viewVideoReports();
        else if (choice == "15") {
            std::cout << "Preparing dashboard database...\n" << std::flush;

            int status = std::system(
                "./.venv/bin/python scripts/import_reports.py"
            );

            if (status != 0) {
                std::cerr << "Report import failed.\n";
                continue;
            }

            std::cout << "Launching GUI Dashboard...\n"
                      << "Open http://127.0.0.1:8501\n"
                      << std::flush;

            status = std::system(
                "./.venv/bin/python -m streamlit run dashboard.py "
                "--server.address 127.0.0.1 "
                "--server.port 8501 "
                "--browser.gatherUsageStats false"
            );

            if (status != 0)
                std::cerr << "Dashboard stopped or failed.\n";
        }
        else if (choice == "16") {
            std::cout << "System closed. Thank you.\n";
            break;
        } else {
            std::cout << "Invalid choice. Enter a number from 1 to 16.\n";
        }
    }
    return 0;
}
