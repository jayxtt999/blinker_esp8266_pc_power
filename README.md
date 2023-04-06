# blinker_esp8266_pc_power
blinker控制电脑开机

## 所需材料
 - 继电器底板智能插座WIFI模块
 - esp01s
 - 线材若干，开关若干
 - binker app

![image](https://raw.githubusercontent.com/jayxtt999/blinker_esp8266_pc_power/main/img/esp01s.jpg)

![image](https://raw.githubusercontent.com/jayxtt999/blinker_esp8266_pc_power/main/img/Snipaste_2023-04-06_13-33-40.jpg)
## 开始



### 接线
`建议先烧录好程序，app端调试成功后在做接线调整`

首先需要找到你电脑主板对应开关供电的接口，可以去品牌主板查询，如
![image](https://raw.githubusercontent.com/jayxtt999/blinker_esp8266_pc_power/main/img/zhuban.jpg)
`PWR_BTN` 就是开关机按钮，你随便拿个螺丝刀短接一下，看能否开机就可以了。一般是短接1s开关机，短接3秒强制关机


将原来的开关线做简单改造
原来应该是开关线（PWR GUD）直连一个前置面板的自锁电源开关

![image](https://raw.githubusercontent.com/jayxtt999/blinker_esp8266_pc_power/main/img/pc.jpg)

你可以把这条线的连接直接接入继电器模块，等于将自锁开关的控制权移交到继电器，但是不建议直接这样，我们需要前置面板的开关和继电器都能使用

那么可以在原来的接线基础上新增2条线，接入继电器，如图示

![image](https://raw.githubusercontent.com/jayxtt999/blinker_esp8266_pc_power/main/img/jiantu01.jpg)





### 修改烧录程序

#### Blinker
 - 下载Blinker app
 - 新增设备
 - 为设备新增一个按钮，用于控制继电器开关
 - 为设备新增一个文本，用于显示开机状态
![image](https://raw.githubusercontent.com/jayxtt999/blinker_esp8266_pc_power/main/img/blink.jpg)

### 代码

在main.ino中修改
```
char auth[] = "";//Blinker key

char ssid[] = "";//你的wifi名称
char pswd[] = "";//你的wufi密码

BlinkerButton Button1("btn-power");//btn-power为你新增按钮的组件键名
#define TEXTE_1 "tex-power" //tex-power为你新增按钮的组件键名

```

### 测试

![image](https://raw.githubusercontent.com/jayxtt999/blinker_esp8266_pc_power/main/img/blink2.jpg)
`尝试开关是否有反应`


### 代码说明
 - 代码中开启了一个tcp服务用于与其它模块通信，比如你不想用app进行控制，当然你也可以使用http服务，需要注意不要暴露到公网。
 - 开机状态，因为使用ping指令来当前电脑的开机状态，所以连接的wifi与电脑使用的需要在同一局域网，因为是ip，你需要在路由器中对电脑静态分配一个ip（mac绑定），否则可能有判断问题
 - 这部分的代码高低电平切换才开关机，你需要自行测试一下，理论上应该只要置高或者置低就行，可能是我电脑的问题，或者有知道的可以说一下原因，情况是单置高会在3s后关机
 ```
    digitalWrite(0, 0);
    delay(1000);
    digitalWrite(0, 1);
 ```

 ### 供电问题

此项目我使用的供电为在usb面板取点，刚好是5v。主板可能需要开启关机时usb供电，具体操作可以百度`电脑关机时usb供电`


### 其它方案

检测开机状态：你可以找下主板的供电位置，比如存在3v的。用开发板的引脚去检测高地电平（注意降压，否则可能烧板），关机低电平，开机高电平，这个比较准确（其实ping也还可以，就是存在功耗问题，你可以修改代码中对ping间隔时间的逻辑）
供电：可以整一个18650电池，然后esp01s可以开启休眠模式降低功耗，再上一个充电模块当电脑在开机状态下为电池充电


### TODO
- [ ] 与天问开发板语音交互

### 相关参考
https://blog.csdn.net/CDL_LuFei/article/details/124496972
