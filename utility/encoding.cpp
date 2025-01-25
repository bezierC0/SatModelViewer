#if defined(_WIN64)

#include "encoding.hpp"
#include <iostream>
#include <Windows.h>

namespace utility
{
    namespace encoding
    {
        boost::optional<std::string> utf8_to_current_code_page(const std::string& utf8_text)
        {
            auto utf8_text_length = static_cast<int>(utf8_text.length());

            auto utf16_text_length = MultiByteToWideChar(CP_UTF8, 0, utf8_text.data(), utf8_text_length, nullptr, 0);
            auto utf16_text = std::wstring(utf16_text_length, 0);

            if (MultiByteToWideChar(CP_UTF8, 0, utf8_text.data(), utf8_text_length, &utf16_text[0], utf16_text_length) != utf16_text_length)
            {
                std::cerr << "Cannot convert text from UTF-8 to UTF-16. Error: 0x" << std::hex << GetLastError() << std::endl;

                return boost::none;
            }

            auto current_code_page_text_length = WideCharToMultiByte(CP_ACP, 0, utf16_text.data(), utf16_text_length, nullptr, 0, nullptr, nullptr);
            auto current_code_page_text = std::string(current_code_page_text_length, 0);

            if (WideCharToMultiByte(CP_ACP, 0, utf16_text.data(), utf16_text_length, &current_code_page_text[0], current_code_page_text_length, nullptr, nullptr) != current_code_page_text_length)
            {
                std::cerr << "Cannot convert text from UTF-16 to current code page. Error: 0x" << std::hex << GetLastError() << std::endl;

                return boost::none;
            }

            return std::move(current_code_page_text);
        }
    }
}

#endif