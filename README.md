# robotr1.github.io

[![圖片 ALT 文字放在這裡](http://img.youtube.com/vi/Kwwezf4AD-o/0.jpg)](http://www.youtube.com/watch?v=Kwwezf4AD-o)

影片連結: https://youtu.be/Kwwezf4AD-o

>預覽網頁:      
>https://nextjun.github.io/robotr1/data

---

## 1. 前言

此專題起源於參加某次模型活動入手之機器人模型, 由於造型簡約、幾何、科幻風格不錯, 在第一時間就入手。 有感於此模型僅為3D列印成形, 賣家販售的素體並未做表面處理, 特此改造一番。 除了打磨、補土、筆塗上漆外, 並擴增原有LED開關亮燈之功能, 加入了具wifi模組之MCU和替換成RGB LED做電控。

整體花費上, 硬體材料費能壓到300以下, 但改造手工製作所耗時間甚多, 往後製作如非必要, 會分享以模組對接的專案為主。

![mini-Q](https://github.com/NEXTJun/robotr1/blob/master/src_md/picture/mini-Q.jpg?raw=true)

>圖片、模型來源 -
>**泉源studio**   
>https://www.facebook.com/kuuhakuprojects/

---

## 2. 所需材料

+ 中空之機器人模型
+ ESP8266 ESP-12
+ 5V to 3V 電源轉換模組
+ 5mm 共陽極 RGB LED
+ 3mm 單色 LED
+ 段數開關
+ 電木板
+ 2.0mm to 2.54mm 轉接版
+ Type-A USB公 轉 micro USB母線材
+ 1/4電阻、電線、排針數個
+ MPU-6050 (非必要)

---

## 3. 模型處理

### (1) 打磨

3D列印製作之模型, 即便精度在高的列印機, 成品表面具有階層觸感。 為了使表面更加光滑, 可以採用丙酮腐蝕外, 再者就是用砂紙、銼刀拋光。

因化學處理危險且仍具有不平整的凹陷, 建議新手使用粗製銼刀或400號左右之砂紙做初部打磨。

![素體](https://github.com/NEXTJun/robotr1/blob/master/src_md/picture/19574974_837766483040011_1601073209781201049_o.jpg?raw=true)
圖片取至 泉源studio

### (2) 補土

打磨是屬於減法加工的技巧, 如需更加平整, 尚需要用補土做加法加工, 去填補細微的凹槽以及打磨的痕跡。 建議使用500號底漆補土噴灌, 在每次打磨告一段落後噴漆, 再用更高號的砂紙打磨。

這裡筆者使用進階的筆塗補土的技巧(空間不能噴漆的悲哀), 捨棄追求平滑感, 刻意去製造出表面觸感。

### (3) 打磨、補土 Loop

所謂模型做到好, 打磨打到老。反覆進行(1)、(2)步驟, 並根據對完美的追求, 增加更細緻、更高號的砂紙與底漆補土做加工。總有一天材料費噴光或妥協後, 就算是初步完成基本的表面打底處理。

![未上漆](https://github.com/NEXTJun/robotr1/blob/master/src_md/picture/line_1548343270453.jpg?raw=true)

### (4) 上漆

模型根本性的大坑, 能選用的技巧有噴漆或筆塗, 如追求平整度和方便性, 建議使用噴漆做大面積處理。如對表面凹凸感的質地和部要求色澤均勻下, 可使用筆塗, 直接在模型上做畫。 

此專案筆者採用的是筆塗技巧, 刻意去製造金屬髮絲紋和舊化感, 採用的漆料為壓克力戰旗漆。

![筆塗上漆後](https://github.com/NEXTJun/robotr1/blob/master/src_md/picture/line_1551545618066.jpg?raw=true)

![基本組裝](https://github.com/NEXTJun/robotr1/blob/master/src_md/picture/P_20190315_210858_vHDR_Auto.jpg?raw=true)

### (5) 保護漆

最後當表面處理完成後, 畫龍點睛(或毀於一旦)的步驟便是噴塗保護漆。噴塗保護漆的優點有很多：防掉漆、刮傷、潑水、統一光澤等。 如希望外表能閃亮亮反光, 可選用亮光漆; 如希望低調穩重可選用消光漆; 小孩子才選擇兩個都要的(X), 不想光澤改變過大(O), 也有半消光漆的可選擇。

基本模型觀念講解: 保護漆成分分為油性和水性, 須根據表面顏色漆種做選擇, 以避免腐蝕。消光漆原理為亮光漆加上白色微粒, 因此上完亮光漆可再噴上消光漆, 但上完消光漆再噴亮光漆則無作用。

本專案採用郡士綠罐的水性消光保護漆, 避免因溼度和操作關係會導致的白化現象。

![噴完保護漆](https://github.com/NEXTJun/robotr1/blob/master/src_md/picture/P_20190415_234603_vHDR_Auto.jpg?raw=true)

---

## 4. 電路製作

### (1) 規劃配線

製作電子專題首先要確定好各模組的電源電壓和通訊介面, 並做初步的線路規劃, 電源部分會要求做好共地, 而通訊介面則須了解好協定的匯流排連結。

此專題的電路簡單, 即基礎的LED pin digital HIGH/LOW 和電源轉換供電, 相關電路規劃如下圖所示, 需要注意的地方有兩點:

1. RGB LED採用共陽極 LED, 在共同腳上連接正極端, 好處是供電源會來自電源端, 而非MCU, 不會讓pin腳負荷超出限制的電流, 壞處是拉高pin腳電位會是無電流通過, 拉低才是通路, 程式端需特別注意。
   
2. MPU6050為非必要模組, 該模組為六軸感測器, 是原先做專題規劃想加入的功能, 但目前專題的完成品並未用到此一功能。 

![電路圖](https://github.com/NEXTJun/robotr1/blob/master/src_md/picture/layout.png?raw=true)

### (2) 焊接

規劃總是理想的, 實際焊接就牽扯到配線、技術、物理限制等等, 以電子電路專題的規劃設計順序為: 電路圖規劃->麵包版驗證->電木板測試->PCB量產, 量不大的Maker作品最多是做到電木板好省成本, 以下講解會以電木板焊接為主。

此專題需要注意的重點有兩點:

1. 模型是從第三方購入, 內部的空間有限, 致使電路設計上要以最小空間設計做考量, 相關模組的尺寸都要考慮, 也請注意密集線路下容易發生的短路危險。 原先專題在規劃上, 採置入鋰電池供電, 但因空間的物理限制因而作罷, 所以才採用另外從模型延伸出電源供應線的不美觀作法。 ~~不然什麼東西都塞外部就都沒事啦~~

2. 因第一點的空間限制, 此專題才會只使用原生的ESP-12, 而非使用另外做好供電電路設計之類的ESP模組。ESP-12在使用上需注意腳位的間距為2.0mm, 非市面上電子元件常見的2.54mm, 因此需要使用2.0mm to 2.54mm 轉接版, 如果技術高超, 也可以直接飛線。

---

## 5. 程式燒錄

### (1) 環境建置

首先介紹下主角ESP8266, ESP8266是中國IC公司樂鑫所推出的Wifi晶片, 雖說缺乏硬體加密功能、時脈較低、可接受同時連線數少等缺點, 但因價格便宜(不到100台幣)且具有常用的基本功能, 廣受電子愛好者歡迎, 比起其他Wifi晶片擁有更多Open source。 ESP8266常用的用法有兩種: 早期會將ESP8266配上Arduino以UART溝通, 當成Wifi模組外掛來使用。 後來有人發現到ESP8266內部32位元晶片可直接燒入可編成指令使用, 比起Arduino的AVR 8位元晶片效能更好, 因此發展出以Arduino IDE配上Arduino語法的使用方法(當然用到的轉換library會吃掉一定效能和空間)。 

ESP8266指的是IC名稱, 市面上可買到做好引腳的模組有ESP-01、ESP-03、ESP-12，其中ESP-01最常見, 但可用的腳位數最少，ESP-12可用腳位最多, 但需另外再加工才有排針腳位, 其他還有已把ESP-12做好加工並附上電源模組和USB to TTL的NodeMCU、WeMos D1、webduino(使用加強版的ESP32)等, 筆者建議在初次接觸ESP8266 to Arduino IDE的可以從這幾塊開發版入手。

此專案是採用ESP8266 to Arduino IDE, 將ESP8266當MCU編譯操作, 而非原廠預設的單獨Wifi模組以AT Command操作。 因使用方法與原廠相異, 使用到第三方資源, 事前的環境建置需下點功夫。 此專案因為是單獨使用ESP-12模組, 並未像Node MCU具有USB to TTL的轉換晶片，因此任何燒入設定都需另外接上USB TO TTL通訊轉換模組才可和晶片溝通。

相關建置的步驟可分為兩個階段: ESP8266 to Arduino IDE編譯、 File to ESP8266, 如單純網路上的常見操作, 執行第一階段即可, 因此專題還想塞入圖片等額外大容量資源, 建議做完到第二階段。詳細步驟這裡不多做敘述, 請自行參考附上的網路資源。

1.  ESP8266 to Arduino IDE編譯

+ [使用Arduino IDE 開發ESP8266 應用(一) : 環境設定與韌體上傳 - 小狐狸事務所](http://yhhuang1966.blogspot.com/2017/09/arduino-ide-esp8266.html)  
+ [[ ESP8266 & NodeMCU ] 在Arduino IDE 設定NodeMCU - Makee.io Blog](https://oranwind.org/-esp8266-nodemcu-zai-arduino-ide-she-ding-nodemcu/)
+ [Installing - ESP8266 Arduino Core documentation - Read the Docs](https://arduino-esp8266.readthedocs.io/en/latest/installing.html)
+ [Install the ESP8266 Board in Arduino IDE in less than 1 minute (Windows, Mac OS X, and Linux)](https://randomnerdtutorials.com/how-to-install-esp8266-board-arduino-ide/)

2. File to ESP8266
   
+ [在ESP8266的SPIFFS檔案系統存放網頁檔案（一） - 網昱多媒體](https://swf.com.tw/?p=905)
+ [File system - ESP8266 Arduino Core documentation - Read the Docs](https://arduino-esp8266.readthedocs.io/en/latest/filesystem.html)

### (2) 燒錄

本專案在燒錄前, 可更改部分程式碼做自訂設定

>更改Wifi AP SSID顯示名稱
>
>```C
>#define Project_SSID      "Robot_R1"
>```

>更改Wifi 登入密碼
>
>```C
>#define Project_PASSWORD  "12345678"
>```

>更改連接網頁網址
>
>```C
>#define Project_HOST      "www.robot.com"
>```


燒錄的步驟有兩點, 分別是ESP8266操作程式碼燒錄和相關檔案燒錄。

1. ESP8266操作程式碼燒錄，在download完本專案後, 點開 "ESP8266_R1.ino", 並執行燒錄。
補充一點, 由於此燒錄方式是在本機端重新做編譯, 請確認Arduino是否有預載用到的函式庫。安裝路徑為「草稿碼->匯入程式庫->管理程式庫」。

    ```C
    #include <ESP8266WiFi.h>
    #include <DNSServer.h>
    #include <ESP8266WebServer.h>
    #include <WebSocketsServer.h>
    #include <ArduinoJson.h>
    #include <FS.h>
    ```

    本專案採用的library版本為
    + ArduinoJson by Benoit Blanchon version 6.10.0
    + DNSServer Built-In by Kristijan Novoselic version 1.1.1
    + WebSockets by Markus Sattler version 2.1.1

    相關細節設定如下圖所示，特別要注意的是開發板可選擇NodeMCU 0.9, 因要執行步驟2的燒錄檔案, 須預留空間給資料存放用, 因此Flash Size選擇4M(1M SPIFFS)。

![Upload Setting](https://github.com/NEXTJun/robotr1/blob/master/src_md/picture/upload_setting.png?raw=true)

2. 執行「工具→ESP8266 Sketch Data Upload」, 該程序會將ino檔同目錄的data資料夾上傳至ESP8266 flash內, 並可在程式內以路徑方式調用該資源。 本專案data資料夾所存放之檔案為網頁相關檔案, 如需更改請自行學習前端開發相關知識。

---

## 6. 上電展示

完成硬體製作和程式燒錄後, 即可接上USB 5V供電確認專案完成, 先假定並未更改自訂設定, 上電後ESP8266會建立SSID為 "Robot_R1" 的Wifi, 以筆電或手機連線至該Wifi, 預設密碼為 "12345678", 並使用瀏覽器進入 "www.robot.com"或 "192.168.0.1"即可進入專案網頁, 便可透過網頁遠端控制專案硬體。

![Final Project](https://github.com/NEXTJun/robotr1/blob/master/src_md/picture/IMG_0767.jpg?raw=true)