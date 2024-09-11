#pragma once
#include <string>
#include <vector>
#include <Windows.h>
#include <cstdint>
#include <source_location>


namespace rc {
    namespace types {
        typedef unsigned __int64    address;
        typedef unsigned __int64    rcptr;
        typedef unsigned __int64    rcsize;
        typedef __int64             rcptrdiff;
        typedef std::string         rcstring;
        typedef std::wstring        rcwstring;
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
        auto _fastcall disableThreadCalls(HMODULE& moduleHandle) -> void;
        auto findPattern(uintptr_t startAddress, uintptr_t endAddress, const std::string& patternStr, bool extractAddress, bool extractOffset, int offsetPosition) -> uintptr_t;
        auto hexToByte(const std::string& hexStr, std::vector<uint8_t>& byteArray) -> bool;
        auto getModPtr(const std::wstring& moduleName) -> uintptr_t;
        auto getModHandle(const std::wstring& moduleName) -> HMODULE;
        template<typename TYPE> auto write(uintptr_t address, TYPE value) -> bool;
        template<typename TYPE = uintptr_t> auto read(uintptr_t address) -> TYPE;

        struct rcmodule {
            std::wstring name;
            uintptr_t address, end;
            size_t size;

            constexpr rcmodule(std::wstring modName) : name(modName), address(0), end(0), size(0) { }

            bool process() {
                this->address = getModPtr(this->name);
                if (!this->address) return false;

                auto dosH = reinterpret_cast<IMAGE_DOS_HEADER*>(this->address);
                auto ntH = reinterpret_cast<IMAGE_NT_HEADERS*>(this->address + dosH->e_lfanew);

                this->size = ntH->OptionalHeader.SizeOfImage;
                if (this->size == 0) return false;

                this->end = this->address + this->size;

                return true;
            }
        };
    }
    namespace colsys {
        enum class setcol : uint8_t {
            p_blue = 0x0001,
            p_green = 0x0002,
            p_red = 0x0004,
            p_light = 0x0008,



            grey = p_blue | p_green | p_red,
            red = p_red | p_light,
            green = p_green | p_light,
            blue = p_blue | p_light,
            yellow = p_green | p_red | p_light,
            orange = p_red | p_green,
            purple = p_red | p_blue,


            white = 15,
            reset = 0xFF,
        };
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
        auto errorLog(const std::string text, std::source_location loc = std::source_location::current(), bool needloc = 0) -> void;
    }
}