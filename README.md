# custom-taser
Hardware and software needed for my custom [TENS unit](https://en.wikipedia.org/wiki/Transcutaneous_electrical_nerve_stimulation). Made with the purpose of making it easier and faster to integrate a TENS unit with other hardware / software for custom projects.

### Hardware
The hardware essentially boils down to 4 H-bridges, and a boost converter. The H-bridges are driven by 2 shift registers, and the boost converter is driven by a dedicated arduino nano. 2 separate boards are used because the timing needed for a boost converter is relatively sensitive, and if the H-bridges are activating, the boost converter timing is thrown off, resulting in voltages that are not consistent. There is a voltage divider on the output of the boost converter that gives feedback to the arduino board. The analogRead value should not be near either of the value extremities for the arduino board, as the readings are not perfectly linear, and tend to snap to the max and min read values.

### Software
The software creates an AC square wave on the channel that is asked of it. Frequency should be somewhere from [30 - 50Hz](https://enrichedhealthcare.com/library/resources/tens/#:~:text=80%20to%20120Hz%2Dacute%20pain%0A%0A35%2D50Hz%2Dmuscle%20stimulation%0A%0A2%20to%2010Hz%20%E2%80%93%20chronic%20pain) in order to stimulate the muscle properly. The boost converter uses extremely simple controls as of now, but I plan to give it PID control in the future.

### Usage
I used this project to shock users in [this](https://youtu.be/dzjZxOKlJjM) video of my friends playing mario kart. I have plans to use it more in the coming years.

### Materials
Too lazy to create a dedicated BOM file

|Item Name|Part # / spec|Quantity|Link|Notes|
|:---|:---:|:---|---|---|
|Shift Register|74HC595D,118|2|||
|MicroController|ARDUINO_NANO|2|[Ali Express](https://www.aliexpress.com/item/1005002966043359.html?spm=a2g0o.order_list.order_list_main.52.64fb1802VRt5Sn)||
|Diode|1N4007|1|[Ali Express](https://www.aliexpress.com/item/1005001456610390.html?spm=a2g0o.order_list.order_list_main.104.64fb1802VRt5Sn)||
|N-Channel MOSFET|IRLZ44NPBF|17|[Ali Express](https://www.aliexpress.com/item/32714396199.html?spm=a2g0o.order_list.order_list_main.119.64fb1802VRt5Sn)|Interchangable with other logic level N-Channel Mosfets|
|P-Channel MOSFET|IRF9540|8|[Ali Express](https://www.aliexpress.com/item/32549579856.html?spm=a2g0o.order_list.order_list_main.159.64fb1802VRt5Sn)|Schematic says HSP0115 but the IRF9540 is cheaper. Pretty much any logic level Mosfet with the same pinout works.|
|C1 capacitor|47uF 5V|1||only 5V, doesn't need to be huge. 47uF works well on mine|
|C2 capacitor|470uF 100V|1|[Ali Express](https://www.aliexpress.com/item/1005002297880326.html?spm=a2g0o.order_list.order_list_main.215.64fb1802VRt5Sn)|Required to handle the shock voltage. Mine is 100V 470uF|
|Power Inductor|100mH, 1W MINIMUM|1|[Ali Express](https://www.aliexpress.com/item/1005003650001800.html?spm=a2g0o.order_list.order_list_main.169.64fb1802VRt5Sn)|I have burned many low power inductors by not respecting the current this takes. 100mH but could benifet from a larger value|
|10k Resistors|0.25W|8|||
|R9, R10, R11 Resistors|0.25W|1||These create a voltage divider to scale down the shock voltage so the uC can have closed loop control|
|R12, R13 Resistors|0.25W|1||I2C bus resistors. mine are 3.3kΩ|
|TIE5 / TIEOUT|Just some solder|0||Needs to be soldered across to decide is the boost converter should run off of internal 5V or external power. I always use external|
