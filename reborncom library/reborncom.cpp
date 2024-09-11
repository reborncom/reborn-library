#include "build/reborncom.h"
#include <string>
#include <vector>
#include <random>
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <wincrypt.h>
#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <shlobj_core.h>
#include <filesystem>
#include <cstdint>
#include <cassert>
#include <codecvt>
#include <source_location>
#include <wininet.h>
#pragma comment(lib, "wininet.lib")



std::random_device rd;



namespace rc {
    namespace cryptsys {
        bool setuped = false;
        int mov[10000] = {};
        std::string base = "A3Bn@op=q1rOPClm2Q0Rst4u!vwE&Fghij7kTGHI6J$a/bcd5ef-UVW)Yx8yzKL9M_NSDZ.+(;";


        auto xorStr(const std::wstring& input) -> std::wstring {
            std::wstring output = input;
            for (wchar_t& c : output) { c ^= 'q'; }
            return output;
        }
        auto setupMov() -> void {
            for (unsigned int sa = 0; sa < 10000; sa++) {
                mov[sa] = 1;
                mov[sa + 1] = 2;
                sa = sa + 1;
            }
            setuped = true;
        }
        auto getSpace(const std::string& input) -> std::string {
            std::string result = input;
            for (char& c : result) { if (c == ' ') { c = '_'; } }
            return result;
        }
        auto setSpace(const std::string& input) -> std::string {
            std::string result = input;
            for (char& c : result) { if (c == '_') { c = ' '; } }
            return result;
        }
        auto encStr(std::string& input) -> std::string {
            if (setuped == false) { setupMov(); }

            std::string text = getSpace(input);
            std::vector<char> word(text.begin(), text.end());
            int baseLength = base.length();

            for (int input_words = 0; input_words < text.length(); input_words++) {
                if (word[input_words] == ' ') { continue; }
                for (int base_words = 0; base_words < baseLength; base_words++) {
                    if (word[input_words] == base[base_words]) {
                        int mov_size = (base_words + mov[input_words]) % baseLength;
                        word[input_words] = base[mov_size];
                        break;
                    }
                }
            }
            return std::string(word.begin(), word.end());
        }
        auto decStr(std::string& input) -> std::string {
            if (setuped == false) { setupMov(); }

            std::string text = std::string(input.begin(), input.end());
            std::vector<char> word(text.begin(), text.end());
            int baseLength = base.length();

            for (int input_words = 0; input_words < text.length(); input_words++) {
                if (word[input_words] == ' ') { continue; }
                for (int base_words = 0; base_words < baseLength; base_words++) {

                    if (word[input_words] == base[base_words]) {
                        int mov_size = (base_words - mov[input_words]) % baseLength;
                        word[input_words] = base[mov_size];
                        break;
                    }
                }
            }
            return setSpace(std::string(word.begin(), word.end()));
        }
        
    }
    namespace randsys {
        std::string characters = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

        auto genWstr(unsigned int length) -> std::wstring {
            std::mt19937 gen(rd());
            std::uniform_int_distribution<int> dist(0, characters.length());
            std::wstring randomString;
            for (int i = 0; i < length; ++i) { randomString += characters[dist(gen)]; }
            return randomString;
        }
        auto genStr(unsigned int length) -> std::string {
            std::mt19937 gen(rd());
            std::uniform_int_distribution<int> dist(0, characters.length());
            std::string randomString;
            for (int i = 0; i < length; ++i) { randomString += characters[dist(gen)]; }
            return randomString;
        }
        auto genWstrByAlphabet(std::wstring alphabet, unsigned int length) -> std::wstring {
            std::mt19937 gen(rd());
            std::uniform_int_distribution<int> dist(0, alphabet.size() - 1);
            std::wstring randomString;
            for (int i = 0; i < length; ++i) { randomString += alphabet[dist(gen)]; }
            return randomString;
        }
        auto genStrByAlphabet(std::string alphabet, unsigned int length) -> std::string {
            std::mt19937 gen(rd());
            std::uniform_int_distribution<int> dist(0, alphabet.size() - 1);
            std::string randomString;
            for (int i = 0; i < length; ++i) { randomString += alphabet[dist(gen)]; }
            return randomString;
        }
        auto genInt(unsigned int minimum, unsigned int max) -> int {
            std::mt19937 gen(rd());
            std::uniform_int_distribution<int> uniformDistribution(minimum, max);
            return uniformDistribution(gen);
        }
        auto genBool() -> bool {
            std::mt19937 gen(rd());
            std::uniform_int_distribution<int> dis(0, 1);
            return dis(gen);
        }
    }
    namespace strsys {
        auto getDesktopPath() -> std::string {
            CHAR desktopPath[MAX_PATH];
            if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_DESKTOP, NULL, 0, desktopPath))) {
                return std::string(desktopPath) + "\\";
            }
            return "";
        }
        auto getTempPath() -> std::string {
            CHAR tempPath[MAX_PATH];
            if (GetTempPathA(MAX_PATH, tempPath) != 0) {
                return std::string(tempPath);
            }
            return "";
        }
        auto getAppdataPath() -> std::string {
            CHAR myPath[MAX_PATH];
            if (SHGetSpecialFolderPathA(0, myPath, CSIDL_APPDATA, FALSE)) {
                return std::string(myPath) + "\\";
            }
            return "";
        }
        auto clearStr(std::string& input) -> void {
            input.clear();
            input.shrink_to_fit();
        }
        auto clearStr(std::wstring& input) -> void {
            input.clear();
            input.shrink_to_fit();
        }
        auto strToWstr(std::string input) -> std::wstring {
            std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
            return converter.from_bytes(input);
        }
        auto wstrToStr(std::wstring input) -> std::string {
            std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
            return converter.to_bytes(input);
        }
    }
    namespace timesys {
        auto getYear() -> std::string {
            time_t t = time(nullptr);
            tm* now = localtime(&t);

            std::ostringstream oss;
            oss << std::setw(4) << std::setfill('0') << (now->tm_year + 1900);
            return oss.str();
        }
        auto getMonth(bool returnText = false) -> std::string {
            time_t t = time(nullptr);
            tm* now = localtime(&t);

            std::ostringstream oss;
            oss << std::setw(2) << std::setfill('0') << (now->tm_mon + 1);

            if (returnText) {
                static const std::string months[] = {
                    "Invalid",      
                    "January",      
                    "February",     
                    "March",        
                    "April",        
                    "May",          
                    "June",         
                    "July",         
                    "August",       
                    "September",    
                    "October",      
                    "November",     
                    "December"      
                };
                return months[std::stoi(oss.str())];
            }

            return oss.str();
        }
        auto getDay() -> std::string {
            time_t t = time(nullptr);
            tm* now = localtime(&t);

            std::ostringstream oss;
            oss << std::setw(2) << std::setfill('0') << now->tm_mday;
            return oss.str();
        }
        auto getHour() -> std::string {
            time_t t = time(nullptr);
            tm* now = localtime(&t);

            std::ostringstream oss;
            oss << std::setw(2) << std::setfill('0') << now->tm_hour;
            return oss.str();
        }
        auto getMinute() -> std::string {
            time_t t = time(nullptr);
            tm* now = localtime(&t);

            std::ostringstream oss;
            oss << std::setw(2) << std::setfill('0') << now->tm_min;
            return oss.str();
        }
        auto getSecond() -> std::string {
            time_t t = time(nullptr);
            tm* now = localtime(&t);

            std::ostringstream oss;
            oss << std::setw(2) << std::setfill('0') << now->tm_sec;
            return oss.str();
        }
        auto formattedDate(bool returnText = false) -> std::string {
            std::string day = getDay();

            if (returnText) return getMonth(true) + " " + day + ", " + getYear();
            return day + "." + getMonth(false) + "." + getYear();
        }
        auto formattedTime(bool showSeconds = true) -> std::string {
            std::string hour = getHour();
            std::string min = getMinute();
            std::string sec = getSecond();

            return hour + ":" + min + (showSeconds ? (":" + sec) : (""));
        }
    }
    namespace filesys {
        auto fileExists(const std::string path) -> bool {
            return std::filesystem::exists(path);
        }
        auto fileCreate(const std::string path, std::string text) -> bool {
            std::ofstream outputFile(path);
            if (outputFile.is_open()) {
                outputFile << text << std::endl;
                outputFile.close();
                return true;
            }
            return false;
        }
        auto fileDelete(const std::string path) -> bool {
            if (std::filesystem::exists(path)) {
                if (std::filesystem::remove(path)) { return true; }
                return false;
            }
            return false;
        }
        auto fileWrite(const std::string path, std::string text) -> bool {
            std::ofstream outFile(path, std::ios::app);
            if (outFile.is_open()) {
                outFile << text << std::endl;
                outFile.close();
                return true;
            }
            return false;
        }
        auto dirCreate(const std::string path) -> bool {
            if (!std::filesystem::exists(path)) {
                if (std::filesystem::create_directory(path)) { return true; }
                return false;
            }
            return false;
        }
        auto dirDelete(const std::string path) -> bool {
            if (std::filesystem::exists(path) && std::filesystem::is_directory(path)) {
                if (std::filesystem::remove(path)) { return true; }
                return false;
            }
            return false;
        }
        auto dirClear(const std::string path) -> bool {
            if (std::filesystem::exists(path) && std::filesystem::is_directory(path)) {
                for (const auto& entry : std::filesystem::directory_iterator(path)) {
                    if (std::filesystem::is_regular_file(entry.path())) {
                        if (!std::filesystem::remove(entry.path())) { return false; }
                    }
                }
                return true;
            }
            return false;
        }
    }
    namespace netsys {
        auto isInternetConnected() -> bool {
            BOOL result = InternetCheckConnection(L"https://www.google.com/", FLAG_ICC_FORCE_CONNECTION, 0);
            return result == TRUE;
        }
        auto isDomainResponding(const std::wstring domain) -> bool {
            BOOL result = InternetCheckConnection(domain.c_str(), FLAG_ICC_FORCE_CONNECTION, 0);
            return result == TRUE;
        }
    }
    namespace memsys {
        template<typename TYPE> auto read(uintptr_t address = 0) -> TYPE {
            if (!address) return TYPE(); 
            else if (address < 0xffffff) return TYPE(); 
            else if (address > 0x7fffffff0000) return TYPE(); 
            else return *reinterpret_cast<TYPE*>(address); 
        }
        template<typename TYPE> auto write(uintptr_t address, TYPE value) -> bool {
            *reinterpret_cast<TYPE*>(address) = value;
            return true;
        }
        auto getModHandle(const std::wstring& moduleName) -> uintptr_t {
            return reinterpret_cast<uintptr_t>(GetModuleHandleW(moduleName.c_str()));
        }
        auto hexToByte(const std::string& hexStr, std::vector<uint8_t>& byteArray) -> bool {
            byteArray.clear();
            size_t len = hexStr.length();
            for (size_t i = 0; i < len; i += 3) {
                std::string ByteString = hexStr.substr(i, 2);
                if (ByteString == "??" || ByteString == "?? ") byteArray.push_back(0);
                else {
                    char* End;
                    long byte = std::strtol(ByteString.c_str(), &End, 16);
                    if (*End != 0)  return false;
                    byteArray.push_back(static_cast<uint8_t>(byte));
                }
            }

            return true;
        }
        auto findPattern(uintptr_t startAddress, uintptr_t endAddress, const std::string& patternStr, bool extractAddress, bool extractOffset, int offsetPosition) -> uintptr_t {
            std::vector<uint8_t> patternBytes;
            if (!hexToByte(patternStr, patternBytes)) return 0;

            uintptr_t rangeEnd = endAddress - patternBytes.size();
            for (uintptr_t addr = startAddress; addr <= rangeEnd; ++addr) {
                bool match = true;
                for (size_t i = 0; i < patternBytes.size(); ++i) {
                    if (patternBytes[i] == 0x00) continue;
                    if (*(uint8_t*)(addr + i) != patternBytes[i]) {
                        match = false;
                        break;
                    }
                }

                if (match) {
                    if (extractAddress) {
                        auto relativeOffset = *reinterpret_cast<int32_t*>(addr + offsetPosition);
                        return addr + 7 + relativeOffset;
                    }
                    if (extractOffset) return *reinterpret_cast<int32_t*>(addr + offsetPosition);
                    return addr;
                }
            }

            return 0;
        }
    }
    namespace colsys {
        namespace colored_cout_impl {
            inline uint16_t getConsoleTextAttr() {
                CONSOLE_SCREEN_BUFFER_INFO buffer_info;
                GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &buffer_info);
                return buffer_info.wAttributes;
            }
            inline void setConsoleTextAttr(const uint16_t attr) {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), attr);
            }
        }
        template <typename type> type& operator<<(type& ostream, const setcol color) {
            static const uint16_t initial_attributes = static_cast<uint16_t>(setcol::grey);
            static uint16_t background = initial_attributes & 0x00F0;
            static uint16_t foreground = initial_attributes & 0x000F;
            if (color == setcol::reset) {
                ostream.flush();
                colored_cout_impl::setConsoleTextAttr(initial_attributes);
                background = initial_attributes & 0x00F0;
                foreground = initial_attributes & 0x000F;
            }
            else {
                uint16_t set = 0;
                const uint16_t colorCode = static_cast<uint16_t>(color);
                if (colorCode & 0x00F0) { background = colorCode; set = background | foreground; }
                else if (colorCode & 0x000F) { foreground = colorCode; set = background | foreground; }
                ostream.flush();
                colored_cout_impl::setConsoleTextAttr(set);

            }
            return ostream;
        }
    }
    namespace logsys {
        bool first = true;
        std::string logPath = strsys::getDesktopPath();

        auto setupLogPath(std::string path) -> void {
            logPath = path;
        }
        auto printLog(const std::string tag, const colsys::setcol color, const std::string text) -> void {
            std::string fileName = timesys::formattedDate(false) + ".log";
            std::string filePath = logPath + fileName;
            std::string timeTab = "[" + timesys::formattedTime() + "]";
            std::string tagTab = "[" + tag + "]";

            if (first) { std::cout << std::endl; first = false; }

            if (!filesys::fileExists(filePath)) filesys::fileCreate(filePath);
            filesys::fileWrite(filePath, timeTab + " " + tagTab + " " + text);

            std::cout << timeTab << " " << color << tagTab << " " << colsys::setcol::reset << text << std::endl;
        }
        auto infoLog(const std::string text) -> void {
            rc::logsys::printLog("INFO", rc::colsys::setcol::white, text);
        }
        auto warningLog(const std::string text) -> void {
            rc::logsys::printLog("WARNING", rc::colsys::setcol::orange, text);
        }
        auto errorLog(const std::string text) -> void {
            rc::logsys::printLog("ERROR", rc::colsys::setcol::red, text);
        }
        auto goodLog(const std::string text) -> void {
            rc::logsys::printLog("GOOD", rc::colsys::setcol::green, text);
        }
    }
}

//auto sourceLoc = std::source_location::current();

//int main() {
//
//    system("pause");
//}
