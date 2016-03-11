EESchema Schematic File Version 2
LIBS:handgelenk-rescue
LIBS:Zilog
LIBS:xilinx
LIBS:Xicor
LIBS:video
LIBS:valves
LIBS:ttl_ieee
LIBS:transistors
LIBS:transf
LIBS:texas
LIBS:switches
LIBS:supertex
LIBS:stm32
LIBS:stm8
LIBS:siliconi
LIBS:silabs
LIBS:sensors
LIBS:rfcom
LIBS:relays
LIBS:regul
LIBS:references
LIBS:pspice
LIBS:Power_Management
LIBS:powerint
LIBS:power
LIBS:philips
LIBS:Oscillators
LIBS:opto
LIBS:onsemi
LIBS:nxp_armmcu
LIBS:nordicsemi
LIBS:msp430
LIBS:motorola
LIBS:motor_drivers
LIBS:microcontrollers
LIBS:microchip_pic32mcu
LIBS:microchip_pic18mcu
LIBS:microchip_pic16mcu
LIBS:microchip_pic12mcu
LIBS:microchip_pic10mcu
LIBS:microchip_dspic33dsc
LIBS:microchip
LIBS:memory
LIBS:maxim
LIBS:logo
LIBS:linear
LIBS:Lattice
LIBS:ir
LIBS:interface
LIBS:intel
LIBS:hc11
LIBS:graphic
LIBS:gennum
LIBS:ftdi
LIBS:ESD_Protection
LIBS:elec-unifil
LIBS:dsp
LIBS:display
LIBS:diode
LIBS:digital-audio
LIBS:device
LIBS:dc-dc
LIBS:cypress
LIBS:contrib
LIBS:conn
LIBS:cmos_ieee
LIBS:cmos4000
LIBS:brooktre
LIBS:audio
LIBS:atmel
LIBS:analog_switches
LIBS:analog_devices
LIBS:Altera
LIBS:adc-dac
LIBS:actel
LIBS:ac-dc
LIBS:74xx
LIBS:74xgxx
LIBS:mcp73831
LIBS:esp8266ex
LIBS:ft232rq
LIBS:mpu9250
LIBS:w25q32bv
LIBS:lt1129cst
LIBS:rv3029c2
LIBS:tlv70233
LIBS:er-oled-ssd1306
LIBS:handgelenk-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 3 4
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L MPU-9250 U6
U 1 1 55F84634
P 3450 2550
F 0 "U6" H 3050 3050 60  0000 C CNN
F 1 "MPU-9250" H 3950 2050 60  0000 C CNN
F 2 "Housings_DFN_QFN:QFN-24-1EP_3x3mm_Pitch0.4mm" H 3150 2800 60  0001 C CNN
F 3 "" H 3150 2800 60  0000 C CNN
	1    3450 2550
	1    0    0    -1  
$EndComp
Text HLabel 2700 2300 0    60   Input ~ 0
SCL
Wire Wire Line
	2700 2300 2800 2300
Text HLabel 2700 2400 0    60   BiDi ~ 0
SDA
Wire Wire Line
	2700 2400 2800 2400
NoConn ~ 2800 2600
$Comp
L GND #PWR024
U 1 1 55F847F4
P 4150 3750
F 0 "#PWR024" H 4150 3500 50  0001 C CNN
F 1 "GND" H 4150 3600 50  0000 C CNN
F 2 "" H 4150 3750 60  0000 C CNN
F 3 "" H 4150 3750 60  0000 C CNN
	1    4150 3750
	1    0    0    -1  
$EndComp
$Comp
L C_Small C14
U 1 1 55F84959
P 3250 3450
F 0 "C14" H 3050 3500 50  0000 L CNN
F 1 "100n" H 3050 3350 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 3250 3450 60  0001 C CNN
F 3 "" H 3250 3450 60  0000 C CNN
	1    3250 3450
	1    0    0    -1  
$EndComp
Wire Wire Line
	3250 3200 3250 3350
Wire Wire Line
	4150 3700 4150 3750
Wire Wire Line
	2250 3700 4150 3700
Wire Wire Line
	3250 3700 3250 3550
Wire Wire Line
	3400 3200 3400 3700
Connection ~ 3400 3700
Wire Wire Line
	3500 3200 3500 3700
Connection ~ 3500 3700
Wire Wire Line
	3600 3200 3600 3700
Connection ~ 3600 3700
NoConn ~ 4100 2300
NoConn ~ 4100 2400
Text HLabel 4200 2600 2    60   Output ~ 0
INT
Wire Wire Line
	4200 2600 4100 2600
NoConn ~ 4100 2750
Wire Wire Line
	2250 2750 2800 2750
Connection ~ 3250 3700
$Comp
L +3V3 #PWR025
U 1 1 55F84D6F
P 1950 1750
F 0 "#PWR025" H 1950 1600 50  0001 C CNN
F 1 "+3V3" H 1950 1890 50  0000 C CNN
F 2 "" H 1950 1750 60  0000 C CNN
F 3 "" H 1950 1750 60  0000 C CNN
	1    1950 1750
	1    0    0    -1  
$EndComp
Wire Wire Line
	3700 3200 3700 3700
Connection ~ 3700 3700
$Comp
L C_Small C12
U 1 1 55F83B4A
P 1950 2000
F 0 "C12" H 1960 2070 50  0000 L CNN
F 1 "100n" H 1960 1920 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 1950 2000 60  0001 C CNN
F 3 "" H 1950 2000 60  0000 C CNN
	1    1950 2000
	1    0    0    -1  
$EndComp
$Comp
L C_Small C13
U 1 1 55F83D26
P 2250 2000
F 0 "C13" H 2260 2070 50  0000 L CNN
F 1 "10n" H 2260 1920 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 2250 2000 60  0001 C CNN
F 3 "" H 2250 2000 60  0000 C CNN
	1    2250 2000
	1    0    0    -1  
$EndComp
Wire Wire Line
	1950 1750 1950 1900
Wire Wire Line
	2250 2100 2250 3700
Connection ~ 2250 2750
Wire Wire Line
	1950 2100 1950 2200
Wire Wire Line
	1950 2200 2250 2200
Connection ~ 2250 2200
Wire Wire Line
	2250 1900 2250 1800
Wire Wire Line
	1950 1800 3550 1800
Connection ~ 1950 1800
Wire Wire Line
	3300 1800 3300 1900
Connection ~ 2250 1800
Wire Wire Line
	3400 1800 3400 1900
Connection ~ 3300 1800
Wire Wire Line
	3550 1800 3550 1900
Connection ~ 3400 1800
Text Notes 2150 1700 0    60   ~ 0
Place 10n cap near VDDIO,\n100n cap near VDD
Text Notes 4650 2750 0    60   ~ 0
Set interrupt pin active low:\nRegister INT_PIN_CFG:\nACTL 0\nOPEN 0\nLATCH_INT_EN 0
Text Notes 4650 3050 0    60   ~ 0
TODO: INT pin doesn't work, connection has to be removed
$EndSCHEMATC
