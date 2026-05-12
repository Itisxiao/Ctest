#include <iostream>
#include <string>

// 1. 抽象基类：定义“万能接口”
class TtsEngine {
public:
    // 纯虚函数：强制子类必须实现
    virtual void Speak(const std::string& text) = 0;
    // ⚠️ 基类析构函数必须 virtual，否则 delete 会内存泄漏
    virtual ~TtsEngine() = default;
};

// 2. 具体实现 1：云端 TTS
class EdgeTts : public TtsEngine {
public:
    void Speak(const std::string& text) override {
        std::cout << "[EdgeTTS] 联网合成播放: " << text << std::endl;
    }
};

// 3. 具体实现 2：本地 TTS
class SherpaTts : public TtsEngine {
public:
    void Speak(const std::string& text) override {
        std::cout << "[Sherpa] 本地离线播放: " << text << std::endl;
    }
};

// 4. 业务层：永远只依赖基类指针
void PlayResponse(TtsEngine* engine, const std::string& reply) {
    engine->Speak(reply); // 运行时自动找对实现
}

int main() {
    // 模拟读取 config.yaml 切换后端
    TtsEngine* my_engine = new EdgeTts(); 
    PlayResponse(my_engine, "小智已开灯");
    delete my_engine;

    my_engine = new SherpaTts();
    PlayResponse(my_engine, "小智已关灯");
    delete my_engine;
    return 0;
}