#include "Input.h"

#include <cctype>
#include <iostream>

namespace {

bool isSpaceChar(char ch) {
    return std::isspace(static_cast<unsigned char>(ch)) != 0;
}

}  // namespace

namespace input {

std::string trim(const std::string& text) {
    std::size_t begin = 0;
    std::size_t end = text.size();
    while (begin < end && isSpaceChar(text[begin])) {
        ++begin;
    }
    while (end > begin && isSpaceChar(text[end - 1])) {
        --end;
    }
    return text.substr(begin, end - begin);
}

bool isBlank(const std::string& text) {
    return trim(text).empty();
}

bool readLine(const std::string& prompt, std::string& out) {
    if (!prompt.empty()) {
        std::cout << prompt << std::flush;
    }
    std::string line;
    if (!std::getline(std::cin, line)) {
        out.clear();
        return false;
    }
    out = trim(line);
    return true;
}

bool parseInt(const std::string& text, int& value) {
    const std::string digits = trim(text);
    if (digits.empty()) {
        return false;
    }

    std::size_t pos = 0;
    bool negative = false;
    if (digits[pos] == '+' || digits[pos] == '-') {
        negative = (digits[pos] == '-');
        ++pos;
    }
    if (pos >= digits.size()) {
        return false;
    }

    long long accumulated = 0;
    for (; pos < digits.size(); ++pos) {
        if (digits[pos] < '0' || digits[pos] > '9') {
            return false;
        }
        accumulated = accumulated * 10 + (digits[pos] - '0');
        if (accumulated > 2147483648LL) {  // 超出 int 范围，直接判定为格式错误
            return false;
        }
    }
    if (negative) {
        accumulated = -accumulated;
    }
    if (accumulated > 2147483647LL || accumulated < -2147483647LL - 1) {
        return false;
    }
    value = static_cast<int>(accumulated);
    return true;
}

bool readInt(const std::string& prompt, int& value) {
    std::string line;
    while (true) {
        if (!readLine(prompt, line)) {
            return false;
        }
        if (parseInt(line, value)) {
            return true;
        }
        std::cout << "输入格式错误：请输入一个整数。" << std::endl;
    }
}

bool readPositiveInt(const std::string& prompt, int& value) {
    std::string line;
    while (true) {
        if (!readLine(prompt, line)) {
            return false;
        }
        int parsed = 0;
        if (!parseInt(line, parsed)) {
            std::cout << "输入格式错误：请输入一个正整数。" << std::endl;
            continue;
        }
        if (parsed <= 0) {
            std::cout << "输入无效：容量必须是正整数（大于 0）。" << std::endl;
            continue;
        }
        value = parsed;
        return true;
    }
}

}  // namespace input
