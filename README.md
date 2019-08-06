# robotr1.github.io
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
+ 5mm RGB LED
+ 3mm 單色 LED
+ 段數開關
+ 電木板
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

本專案採用郡士綠罐的水性消光保護漆，避免因溼度和操作關係會導致的白化現象。

![噴完保護漆](https://github.com/NEXTJun/robotr1/blob/master/src_md/picture/P_20190415_234603_vHDR_Auto.jpg?raw=true)

---
## 4. 電路製作

---
## 5. 程式燒錄