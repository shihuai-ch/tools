## 高通总线类文档
    sm4350 linux android peripheral

## 开启或关闭uart
    getprop | grep console
    start console
    stop console

## 查看gpio配置
    cat /d/gpio
    cat /d/pinctrl/400000.pinctrl/pinmux-pins

## 查看串口传输模式
    cd /sys/devices/platform/soc
    find . -name xfer_mode
    cat ./4c80000.hsuart/xfer_mode

## WIFI-adb使用
    adb tcpip 5555
    adb connect 172.16.4.211:5555
    pad先连接上wifi
    进入settings，最上面有search settings ，输入ip，进行搜索。
    搜索后，点击进入 IP address&Port，然后打开功能* 
    可以看到有IP地址和端口号* 
    adb界面下输入adb connect IP:port* 
    连接成功，输入adb shell ,即可进入adb调试

## 解锁 OEM lock
    步骤：
    ①开机后进入settings-> About phone ->Build number->多次点击->进入开发者模式
    ②settings->System->Advanced->Developer options->打开OEM unlocking选项
    ③连接调试线，输入以下指令：
        adb reboot bootloader
        fastboot flashing unlock
        fastboot reboot
    至此，OEM解锁成功。

## pinctrl 函数定义
    kernel/msm-5.4/Documentation/devicetree/bindings
    vim pinctrl/pinctrl-bindings.txt +274

## 查看pm芯片的配置
    cd /d/regmap/spmi0-01 
    查看pm芯片的gpio配置，寄存器地址等。走spmi总线的gpio
## 装载ko
    push进vendor/lib/modules
    进入文件夹  insmod xxx.ko
    在代码中查看tlmm-gpio寄存器的值