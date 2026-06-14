#include <stdio.h>
#include <stdbool.h>

// ==========================================
// 步骤 1：定义类型别名 (只需写一次)
// 含义：ws_status_cb_t 是一种“函数指针类型”
// ==========================================
typedef void (*ws_status_cb_t)(bool connected);


// ==========================================
// 步骤 2：实现具体的回调函数
// 注意：这个函数的签名（参数和返回值）必须和 typedef 定义的一模一样
// ==========================================
void my_wifi_status_handler(bool connected) {
    if (connected) {
        printf("✅ WiFi 连接成功！\n");
    } else {
        printf("❌ WiFi 连接断开！\n");
    }
}


// ==========================================
// 步骤 3：编写业务函数，使用这个类型作为参数
// ==========================================
// 这里的参数类型直接写 ws_status_cb_t，非常清爽
void set_status_callback(ws_status_cb_t user_callback) {
    printf("系统：正在注册用户的回调函数...\n");
    
    // 模拟某个时刻，网络状态发生了变化，我们调用这个回调函数通知用户
    bool current_status = true; // 假设现在连上了
    
    if (user_callback != NULL) {
        // 触发回调！就像调用普通函数一样使用它
        user_callback(current_status); 
    }
}


// ==========================================
// 步骤 4：在 main 函数中把它们串联起来
// ==========================================
int main() {
    // 把我们在步骤2写的函数，传递给步骤3的函数
    // 函数名 my_wifi_status_handler 本身就是一个地址（指针）
    set_status_callback(my_wifi_status_handler);
    
    return 0;
}