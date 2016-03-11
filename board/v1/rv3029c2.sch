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
Sheet 4 4
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text HLabel 1600 1700 0    60   Input ~ 0
SCL
Wire Wire Line
	1600 1700 1700 1700
Text HLabel 1600 1800 0    60   BiDi ~ 0
SDA
Wire Wire Line
	1600 1800 1700 1800
$Comp
L +3V3 #PWR026
U 1 1 55F98236
P 1200 850
F 0 "#PWR026" H 1200 700 50  0001 C CNN
F 1 "+3V3" H 1200 990 50  0000 C CNN
F 2 "" H 1200 850 60  0000 C CNN
F 3 "" H 1200 850 60  0000 C CNN
	1    1200 850 
	1    0    0    -1  
$EndComp
$Comp
L C_Small C15
U 1 1 55F98261
P 1200 1100
F 0 "C15" H 1210 1170 50  0000 L CNN
F 1 "10n" H 1210 1020 50  0000 L CNN
F 2 "Capacitors_SMD:C_0603" H 1200 1100 60  0001 C CNN
F 3 "" H 1200 1100 60  0000 C CNN
	1    1200 1100
	1    0    0    -1  
$EndComp
Wire Wire Line
	1200 850  1200 1000
Wire Wire Line
	1200 900  3000 900 
Wire Wire Line
	2400 900  2400 1400
Connection ~ 1200 900 
Wire Wire Line
	2200 1400 2200 1350
Wire Wire Line
	2200 1350 1200 1350
Wire Wire Line
	1200 1200 1200 2550
$Comp
L GND #PWR027
U 1 1 55F98317
P 2300 2650
F 0 "#PWR027" H 2300 2400 50  0001 C CNN
F 1 "GND" H 2300 2500 50  0000 C CNN
F 2 "" H 2300 2650 60  0000 C CNN
F 3 "" H 2300 2650 60  0000 C CNN
	1    2300 2650
	1    0    0    -1  
$EndComp
Connection ~ 1200 1350
Wire Wire Line
	2300 2300 2300 2650
Connection ~ 2300 2550
Wire Wire Line
	1700 2000 1200 2000
Connection ~ 1200 2000
NoConn ~ 2900 2000
Text HLabel 3200 1700 2    60   Output ~ 0
INT
Wire Wire Line
	2900 1700 3200 1700
$Comp
L R R9
U 1 1 55F993EB
P 3000 1300
F 0 "R9" V 3080 1300 50  0000 C CNN
F 1 "10K" V 3000 1300 50  0000 C CNN
F 2 "Resistors_SMD:R_0603" H 3000 1300 60  0001 C CNN
F 3 "" H 3000 1300 60  0000 C CNN
	1    3000 1300
	1    0    0    -1  
$EndComp
Connection ~ 2400 900 
Wire Wire Line
	3000 1550 3000 1700
Connection ~ 3000 1700
Wire Wire Line
	3000 900  3000 1050
Wire Wire Line
	1200 2550 2300 2550
$Comp
L RV-3029-C2 U7
U 1 1 55F9C7CF
P 2300 1850
F 0 "U7" H 1950 2200 60  0000 C CNN
F 1 "RV-3029-C2" H 2650 1500 60  0000 C CNN
F 2 "Project Libraries:rv3029" H 2200 1800 60  0001 C CNN
F 3 "" H 2200 1800 60  0000 C CNN
	1    2300 1850
	1    0    0    -1  
$EndComp
$EndSCHEMATC
