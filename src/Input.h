#ifndef INPUT_H
#define INPUT_H

#include <string>

// 控制台输入工具。
// 统一在这里处理读行、去空白和整数解析，保证“空输入 / 格式错误”不会让程序退出或写入非法数据。
namespace input {

// 读取一整行并去掉首尾空白。遇到输入结束（EOF）返回 false。
bool readLine(const std::string& prompt, std::string& out);

// 去掉首尾空白字符。
std::string trim(const std::string& text);

// 判断去掉空白后是否为空。
bool isBlank(const std::string& text);

// 把整段文本解析成 int，要求整段都是数字（允许一个正负号）。
bool parseInt(const std::string& text, int& value);

// 读取整数：格式错误时提示并重试，不会退出程序；遇到 EOF 返回 false。
bool readInt(const std::string& prompt, int& value);

// 读取 [minValue, maxValue] 闭区间内的整数：格式错误或越界时提示并重试；遇到 EOF 返回 false。
bool readIntInRange(const std::string& prompt, int minValue, int maxValue, int& value);

// 读取正整数：格式错误或非正数时提示并重试；遇到 EOF 返回 false。
bool readPositiveInt(const std::string& prompt, int& value);

}  // namespace input

#endif
