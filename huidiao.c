#include <stdio.h>
#include <stdlib.h>

// 定义回调函数的类型
typedef void (*CallbackFunc)(int event_type, void *user_data);

// 或者更通用的形式
typedef int (*ProcessCallback)(const char *data, size_t len, void *context);

// 定义包含回调函数的结构体
typedef struct {
    char name[64];
    CallbackFunc on_event;      // 回调函数指针
    void *user_data;            // 用户数据，传递给回调函数
    int enabled;
} EventHandler;

// 示例回调函数实现
void my_event_handler(int event_type, void *user_data) {
    printf("Event %d received, user_data: %s\n", 
           event_type, (char *)user_data);
}

// 初始化结构体并设置回调
void init_event_handler(EventHandler *handler, const char *name, 
                        CallbackFunc callback, void *user_data) {
    snprintf(handler->name, sizeof(handler->name), "%s", name);
    handler->on_event = callback;
    handler->user_data = user_data;
    handler->enabled = 1;
}

// 触发回调
void trigger_event(EventHandler *handler, int event_type) {
    if (handler && handler->enabled && handler->on_event) {
        handler->on_event(event_type, handler->user_data);
    }
}

int main() {
    EventHandler handler;
    char user_msg[] = "Hello from main";
    
    // 设置回调
    init_event_handler(&handler, "MyHandler", my_event_handler, user_msg);
    
    // 触发事件
    trigger_event(&handler, 42);
    
    return 0;
}