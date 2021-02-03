# ourSpindel
Cheapo clone of iSpindel


# parts
- ESP01-S
- GY-521 with MPU 6050 chip
- TTL Serial Adapter board (with 3,3 V or 5V/3,3V switch), e.g. FTDI FT232R (https://www.az-delivery.de/products/ftdi-adapter-ft232rl)
- 18650 LiFePO4 battery (https://www.eremit.de/p/18650-3-2v-1-500mah-lifepo4-u-loetfahne)
- 1S 18650 battery protection board cause why not (https://www.ksheenakay.com/index.php?main_page=product_info&products_id=588971)
- 2 SPDT slide switches (1 for battery disconnector, 1 for switching between esp01 firmwareupdatemode and run mode)
- empty PCB, wires, cable tires, tape, solder
- stainless steel weights (~60 g), glass smoothie bottle as container and weights. will need experimental careful balancing/taring

# deep sleep
we need deep sleep in order to guarantee battery life up to 2 weeks. ESP01-S needs some soldering for that.
- connect reset pin to chip: https://www.instructables.com/Enable-DeepSleep-on-an-ESP8266-01/
- or use easier small MCU like Wemos D1
