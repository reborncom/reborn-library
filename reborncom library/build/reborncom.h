#pragma once
#include <string>
#include <vector>
#include <Windows.h>
#include <cstdint>


namespace rc {
    namespace types {
        typedef unsigned __int64    address;
        typedef unsigned __int64    rcptr;
        typedef unsigned __int64    rcsize;
        typedef __int64             rcptrdiff;
        typedef std::string         rcstring;
        typedef unsigned int        rcuint;
    }
}
#define USE_RCLIB using namespace rc; using namespace rc::types;


namespace rc {
    namespace cryptsys {
        auto xorStr(const std::wstring& input) -> std::wstring;
        auto encStr(std::string& input) -> std::string;
        auto decStr(std::string& input) -> std::string;
    }
    namespace randsys {
        auto genWstr(unsigned int length) -> std::wstring;
        auto genStr(unsigned int length) -> std::string;
        auto genWstrByAlphabet(std::wstring alphabet, unsigned int length) -> std::wstring;
        auto genStrByAlphabet(std::string alphabet, unsigned int length) -> std::string;
        auto genInt(unsigned int minimum, unsigned int max) -> int;
        auto genBool() -> bool;
    }
    namespace strsys {
        auto getDesktopPath() -> std::string;
        auto getTempPath() -> std::string;
        auto getAppdataPath() -> std::string;
        auto clearStr(std::string& input) -> void;
        auto clearStr(std::wstring& input) -> void;
        auto strToWstr(std::string input) -> std::wstring;
        auto wstrToStr(std::wstring input) -> std::string;
    }
    namespace timesys {
        auto getYear() -> std::string;
        auto getMonth() -> std::string;
        auto getDay() -> std::string;
        auto getHour() -> std::string;
        auto getMinute() -> std::string;
        auto getSecond() -> std::string;
    }
    namespace filesys {
        auto fileExists(const std::string path) -> bool;
        auto fileCreate(const std::string path, std::string text = "") -> bool;
        auto fileDelete(const std::string path) -> bool;
        auto fileWrite(const std::string path, std::string text) -> bool;
        auto dirCreate(const std::string path) -> bool;
        auto dirDelete(const std::string path) -> bool;
        auto dirClear(const std::string path) -> bool;
    }
    namespace netsys {
        auto isInternetConnected() -> bool;
        auto isDomainResponding(const std::wstring domain) -> bool;
    }
    namespace memsys {
        auto findPattern(uintptr_t startAddress, uintptr_t endAddress, const std::string& patternStr, bool extractAddress, bool extractOffset, int offsetPosition) -> uintptr_t;
        auto hexToByte(const std::string& hexStr, std::vector<uint8_t>& byteArray) -> bool;
        auto getModHandle(const std::wstring& moduleName) -> uintptr_t;
        template<typename TYPE> auto write(uintptr_t address, TYPE value) -> bool;
        template<typename TYPE = uintptr_t> auto read(uintptr_t address) -> TYPE;
    }
    namespace colsys {
        enum class setcol : uint8_t;
        namespace colored_cout_impl {
            inline uint16_t getConsoleTextAttr();
            inline void setConsoleTextAttr(const uint16_t attr);
        }
        template <typename type> type& operator<<(type& ostream, const setcol color);
    }
    namespace logsys {
        auto setupLogPath(std::string path) -> void;
        auto printLog(const std::string tag, const colsys::setcol color, const std::string text) -> void;
        auto infoLog(const std::string text) -> void;
        auto warningLog(const std::string text) -> void;
        auto goodLog(const std::string text) -> void;
        auto errorLog(const std::string text) -> void;
    }
}