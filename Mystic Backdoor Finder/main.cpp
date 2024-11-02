#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <thread>

namespace mystic = std::filesystem;

// Function Prototypes
void displayBanner();
void displayCredits();
void scanScripts(bool fixFiles);
bool isSuspicious(const std::string& line, std::string& reason);
void logSuspiciousCode(const mystic::path& filepath, int lineNum, const std::string& line, const std::string& reason, bool fixed);
void removeSuspiciousCode(const mystic::path& filepath, const mystic::path& outputPath);
void showProgressBar(int progress, int total);
void clearConsole();
void openURL(const std::string& url);

// Main Program
int main() {
    displayBanner();

    int choice;
    do {
        clearConsole();
        displayBanner();
        std::cout << "\n1. Scan the 'scripts' folder for obfuscated code, backdoors, and links\n";
        std::cout << "2. Scan and remove obfuscated code, backdoors, and links\n";
        std::cout << "3. Credits\n";
        std::cout << "Enter your choice (1-3): ";
        std::cin >> choice;

        switch (choice) {
        case 1:
            scanScripts(false);
            break;
        case 2:
            scanScripts(true);
            break;
        case 3:
            displayCredits();
            break;
        default:
            std::cout << "Invalid choice, please try again.\n";
            break;
        }

        std::cout << "\nPress any key to return to the main menu...";
        std::cin.get(); // Wait for user input before returning to the main menu
        std::cin.ignore();

    } while (choice != 0);

    return 0;
}

// ASCII Banner Display
void displayBanner() {
    std::cout << "\033[34m"; // Set color to Blue
    std::cout << R"(

  $$\      $$\                       $$\     $$\           
  $$$\    $$$ |                      $$ |    \__|          
  $$$$\  $$$$ |$$\   $$\  $$$$$$$\ $$$$$$\   $$\  $$$$$$$\ 
  $$\$$\$$ $$ |$$ |  $$ |$$  _____|\_$$  _|  $$ |$$  _____|
  $$ \$$$  $$ |$$ |  $$ |\$$$$$$\    $$ |    $$ |$$ /      
  $$ |\$  /$$ |$$ |  $$ | \____$$\   $$ |$$\ $$ |$$ |      
  $$ | \_/ $$ |\$$$$$$$ |$$$$$$$  |  \$$$$  |$$ |\$$$$$$$\ 
  \__|     \__| \____$$ |\_______/    \____/ \__| \_______|
               $$\   $$ |                                  
               \$$$$$$  |                                  
                \______/               
                                                                                                                               
    )" << "\n";
    std::cout << "                         \n";
    std::cout << "\033[0m"; // Reset color to default
}

// Clear console function
void clearConsole() {
#ifdef _WIN32
    system("cls");
#else
    std::cout << "\033[2J\033[H";
#endif
}

// Function to open a URL in the system's default browser
void openURL(const std::string& url) {
#ifdef _WIN32
    system(("start " + url).c_str());
#elif __APPLE__
    system(("open " + url).c_str());
#else
    system(("xdg-open " + url).c_str());
#endif
}

// Credits Display
void displayCredits() {
    int creditChoice;
    do {
        clearConsole();
        displayBanner();
        std::cout << "\n              Tool made by Mystic\n\n";
        std::cout << "              Press 1 Join my Discord Server\n";
        std::cout << "              Press 0 to return to main menu\n";
        std::cout << "Enter your choice: ";
        std::cin >> creditChoice;

        if (creditChoice == 1) {
            openURL("https://discord.gg/hd5pNVmZTu");
        }
        else if (creditChoice != 0) {
            std::cout << "Invalid choice, please try again.\n";
        }

    } while (creditChoice != 0);
}

// Scan Lua scripts for suspicious code and log or fix accordingly
void scanScripts(bool fixFiles) {
    std::string outputFolder = fixFiles ? "fixed" : "output";
    mystic::create_directories(outputFolder);
    int totalFiles = std::distance(mystic::recursive_directory_iterator("scripts"), mystic::recursive_directory_iterator());
    int processedFiles = 0;

    clearConsole();           // Clear console for clean progress display
    displayBanner();          // Show the banner at the top of the progress screen

    for (const auto& entry : mystic::recursive_directory_iterator("scripts")) {
        if (entry.path().extension() == ".lua") {
            mystic::path relativePath = mystic::relative(entry.path(), "scripts");
            mystic::path outputPath = mystic::path(outputFolder) / relativePath;

            if (fixFiles) {
                mystic::create_directories(outputPath.parent_path());
                removeSuspiciousCode(entry.path(), outputPath);
            }
            else {
                std::ifstream file(entry.path());
                std::string line;
                int lineNum = 1;

                while (std::getline(file, line)) {
                    std::string reason;
                    if (isSuspicious(line, reason)) {
                        logSuspiciousCode(entry.path(), lineNum, line, reason, false);
                        std::cout << "Suspicious code found in " << entry.path() << " at line " << lineNum << "\n";
                    }
                    ++lineNum;
                }
            }
        }
        showProgressBar(++processedFiles, totalFiles); // Update progress
    }

    std::cout << "\nScan completed. Check the '" << outputFolder << "' folder for logs.\n";
}

// Check if a line contains suspicious code patterns and provide a reason
bool isSuspicious(const std::string& line, std::string& reason) {
    // Pattern to detect obfuscated code, URLs, and backdoor functions
    std::regex urlPattern(R"((http:\/\/|https:\/\/|www\.)[^\s]+)");
    std::regex discordPattern(R"(discord\.gg|discord\.webhook)");
    std::regex obfuscationPattern(R"(loadstring|load\(\)|RunCode|decode)");

    if (std::regex_search(line, urlPattern) && !std::regex_search(line, discordPattern)) {
        reason = "Suspicious URL detected";
        return true;
    }
    if (std::regex_search(line, obfuscationPattern)) {
        reason = "Obfuscated or backdoor code detected";
        return true;
    }
    return false;
}

// Log suspicious line details to either output or fixed folders
void logSuspiciousCode(const mystic::path& filepath, int lineNum, const std::string& line, const std::string& reason, bool fixed) {
    std::string folder = fixed ? "fixed" : "output";
    std::ofstream logFile(folder + "/" + filepath.filename().string() + "_log.txt", std::ios::app);
    logFile << "Line " << lineNum << ": " << line << " (" << reason << ")\n";
}

// Remove suspicious code lines and save cleaned file
void removeSuspiciousCode(const mystic::path& filepath, const mystic::path& outputPath) {
    std::ifstream file(filepath);
    std::ofstream outputFile(outputPath);
    std::string line;
    int lineNum = 1;

    while (std::getline(file, line)) {
        std::string reason;
        if (isSuspicious(line, reason)) {
            logSuspiciousCode(filepath, lineNum, line, reason, true);
        }
        else {
            outputFile << line << "\n";
        }
        ++lineNum;
    }
}

// Display progress bar in console
void showProgressBar(int progress, int total) {
    int barWidth = 50;
    float ratio = (float)progress / total;
    int pos = barWidth * ratio;

    // Move to a new line so the banner stays visible at the top
    std::cout << "\nProgress: ";
    std::cout << "\r[";
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) std::cout << "\033[32m#\033[0m"; // Green bar
        else std::cout << " ";
    }
    std::cout << "] " << int(ratio * 100.0) << "%";
    std::cout.flush();

    std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Adjust for progress display
}
