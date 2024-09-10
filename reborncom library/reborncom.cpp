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
#include <cassert>
#include <codecvt>
#include <wininet.h>
#pragma comment(lib, "wininet.lib")


class rccryptclass final {
public:
    _inline auto getSetuped() -> bool { return this->setuped; }
    _inline auto getAllowXor() -> bool { return this->allowXor; }
    _inline auto setSetuped(bool val) -> bool { return this->setuped = val; }
    _inline auto getMov(int index) -> int { return this->mov[index]; }
    _inline auto setMov(int index, int val) -> int { return this->mov[index] = val; }
    _inline auto getBase(int index) -> const char { return this->base[index]; }
    _inline auto getBaseLenght() -> int { return this->base.length(); }
    _inline auto setupMov() -> void {
        for (unsigned int sa = 0; sa < 10000; sa++) {
            this->mov[sa] = 1;
            this->mov[sa + 1] = 2;
            sa = sa + 1;
        }
        this->setuped = true;
    }
    _inline auto getSpace(const std::string& input) -> std::string {
        std::string result = input;
        for (char& c : result) {
            if (c == ' ') { c = '_'; }
        }
        return result;
    }
    _inline auto setSpace(const std::string& input) -> std::string {
        std::string result = input;
        for (char& c : result) {
            if (c == '_') { c = ' '; }
        }
        return result;
    }

private:
    bool setuped = false;
    int mov[10000] = {};
    std::string base = "A3Bn@op=q1rOPClm2Q0Rst4u!vwE&Fghij7kTGHI6J$a/bcd5ef-UVW)Yx8yzKL9M_NSDZ.+(;";
private:
    bool allowXor = false;

} cryptclass;
class rcrandomclass final {
public:
    _inline auto getCharacters() -> std::string { return this->characters; }
private:
    std::string characters = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
} randomclass; std::random_device rd;
time_t t = time(nullptr);
tm* now = localtime(&t);



namespace rc {
    namespace crypt {
        auto xorStr(const std::wstring& input) -> std::wstring {
            std::wstring output = input;
            if (cryptclass.getAllowXor()) {
                for (wchar_t& c : output) { c ^= 'q'; }
            }
            return output;
        }
        auto encStr(std::string& input) -> std::string {
            if (cryptclass.getSetuped() == false) { cryptclass.setupMov(); }

            std::string text = cryptclass.getSpace(input);
            std::vector<char> word(text.begin(), text.end());
            int baseLength = cryptclass.getBaseLenght();

            for (int input_words = 0; input_words < text.length(); input_words++) {
                if (word[input_words] == ' ') { continue; }
                for (int base_words = 0; base_words < baseLength; base_words++) {
                    if (word[input_words] == cryptclass.getBase(base_words)) {
                        int mov_size = (base_words + cryptclass.getMov(input_words)) % baseLength;
                        word[input_words] = cryptclass.getBase(mov_size);
                        break;
                    }
                }
            }
            return std::string(word.begin(), word.end());
        }
        auto decStr(std::string& input) -> std::string {
            if (cryptclass.getSetuped() == false) { cryptclass.setupMov(); }

            std::string text = std::string(input.begin(), input.end());
            std::vector<char> word(text.begin(), text.end());
            int baseLength = cryptclass.getBaseLenght();

            for (int input_words = 0; input_words < text.length(); input_words++) {
                if (word[input_words] == ' ') { continue; }
                for (int base_words = 0; base_words < baseLength; base_words++) {

                    if (word[input_words] == cryptclass.getBase(base_words)) {
                        int mov_size = (base_words - cryptclass.getMov(input_words)) % baseLength;
                        word[input_words] = cryptclass.getBase(mov_size);
                        break;
                    }
                }
            }
            return cryptclass.setSpace(std::string(word.begin(), word.end()));
        }
    }
    namespace random {
        auto genWstr(unsigned int length) -> std::wstring {
            std::mt19937 gen(rd());
            std::uniform_int_distribution<int> dist(0, randomclass.getCharacters().length());
            std::wstring randomString;
            for (int i = 0; i < length; ++i) { randomString += randomclass.getCharacters()[dist(gen)]; }
            return randomString;
        }
        auto genStr(unsigned int length) -> std::string {
            std::mt19937 gen(rd());
            std::uniform_int_distribution<int> dist(0, randomclass.getCharacters().length());
            std::string randomString;
            for (int i = 0; i < length; ++i) { randomString += randomclass.getCharacters()[dist(gen)]; }
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
        auto getYear() -> unsigned int {
            return now->tm_year + 1900;
        }
        auto getMonth() -> unsigned int {
            return now->tm_mon + 1;
        }
        auto getDay() -> unsigned int {
            return now->tm_mday;
        }
        auto getHour() -> unsigned int {
            return now->tm_hour;
        }
        auto getMinute() -> unsigned int {
            return now->tm_min;
        }
        auto getSecond() -> unsigned int {
            return now->tm_sec;
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
    namespace network {
        auto isInternetConnected() -> bool {
            BOOL result = InternetCheckConnection(L"https://www.google.com/", FLAG_ICC_FORCE_CONNECTION, 0);
            return result == TRUE;
        }
        auto isDomainResponding(const std::wstring domain) -> bool {
            BOOL result = InternetCheckConnection(domain.c_str(), FLAG_ICC_FORCE_CONNECTION, 0);
            return result == TRUE;
        }
    }
}