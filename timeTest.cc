#include <ctime>
#include <iostream>

void c_style_time() {
    // time_t: 表示时间戳（自1970-01-01 00:00:00 UTC以来的秒数）
    std::time_t now = std::time(nullptr);
    std::cout << "当前时间戳: " << now << " 秒\n";
    
    // 转换为本地时间
    std::tm* local_time = std::localtime(&now);
    std::cout << "本地时间: " 
              << local_time->tm_year + 1900 << "-"  // 年（从1900开始）
              << local_time->tm_mon + 1 << "-"       // 月（0-11）
              << local_time->tm_mday << " "          // 日（1-31）
              << local_time->tm_hour << ":"          // 时（0-23）
              << local_time->tm_min << ":"           // 分（0-59）
              << local_time->tm_sec << "\n";         // 秒（0-60）
    
    // 转换为UTC时间
    std::tm* utc_time = std::gmtime(&now);
    
    // 格式化时间输出
    char buffer[100];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", local_time);
    std::cout << "格式化时间: " << buffer << "\n";
    
    // 从tm结构创建时间
    std::tm custom_time = {};
    custom_time.tm_year = 2024 - 1900;  // 2024年
    custom_time.tm_mon = 0;              // 1月
    custom_time.tm_mday = 1;             // 1日
    custom_time.tm_hour = 12;
    custom_time.tm_min = 0;
    custom_time.tm_sec = 0;
    
    std::time_t custom_timestamp = std::mktime(&custom_time);
    std::cout << "自定义时间戳: " << custom_timestamp << "\n";
}

int main() {
    c_style_time();
    return 0;
}