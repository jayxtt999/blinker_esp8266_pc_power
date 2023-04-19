#define BLINKER_WIFI

#include <Blinker.h>
#include <ESP8266Ping.h>

char auth[] = "";//Blinker 秘钥
char ssid[] = "TP-LINK_xxxxxx";//你的wifi名称
char pswd[] = "";//你的wufi密码
#define PCIP "192.168.6.100"//你的电脑ip地址
uint32_t read_time = 0;
uint16_t pc_status = 0;
String power_msg = "";
uint32_t gap_time = 200000; // 默认检测时长，20分钟检测一次
// 新建组件对象
BlinkerButton Button1("btn-power"); //btn-power为你新增按钮的组件键名
#define TEXTE_1 "tex-power"//tex-power为你新增按钮的组件键名
BlinkerText Text1(TEXTE_1);
WiFiServer server(6000);//开启一个tcp服务用于第三方控制，你也可以调整为http服务

// 按下按键即会执行该函数
void button1_callback(const String &state)
{
    BLINKER_LOG("get button state: ", state);
    power();
}

void power()
{

    digitalWrite(0, 0);
    delay(500);
    digitalWrite(0, 1);
    if (pc_status == 1)
    {
        power_msg = "关机中";
    }
    else
    {
        power_msg = "开机中";
    }
    Text1.print("状态:", power_msg);
    // 人工点击，缩短默认检测时间为8s
    gap_time = 8000;
}

// 如果未绑定的组件被触发，则会执行其中内容
void dataRead(const String &data)
{
    BLINKER_LOG("Blinker readString: ", data);
    Blinker.vibrate();
    uint32_t BlinkerTime = millis();
    Blinker.print("millis", BlinkerTime);
}

void setup()
{
    // 初始化串口
    Serial.begin(115200);
    // 初始化有LED的IO
    pinMode(0, OUTPUT);
    // BLINKER_DEBUG.stream(Serial);
    // BLINKER_DEBUG.debugAll();
    // 初始化blinker
    Blinker.begin(auth, ssid, pswd);
    Blinker.attachData(dataRead);
    Button1.attach(button1_callback);
    // 启动时检测一次
    powerStatus();

    server.begin();
}

void powerStatus()
{

    if (Ping.ping(PCIP,1))
    {
        pc_status = 1;
        power_msg = "已开机";
    }
    else
    {
        pc_status = 0;
        power_msg = "已关机";
    }
    Text1.print("状态:", power_msg);
}

void loop()
{
    Blinker.run();
    unsigned long current_time = millis();
    if (read_time == 0 || (current_time - read_time) >= gap_time)
    {
        read_time = current_time;
        gap_time = 200000;
        powerStatus();
    }
    WiFiClient client = server.available();
    if (client)
    {
        BLINKER_LOG("New client connected");
        while (client.connected())
        {
            if (client.available())
            {
                String request = client.readStringUntil('\r');
                BLINKER_LOG(request);
                // 处理客户端发送的数据
                if (request.indexOf("POWER") != -1)
                {
                    power();
                    powerStatus();
                    BLINKER_LOG("POWER Success");
                    client.println("Success");
                }
                else
                {
                    BLINKER_LOG("POWER Success");
                    client.println("Error");
                }
                break;
            }
        }

        client.stop();
        BLINKER_LOG("Client disconnected");
    }

}
