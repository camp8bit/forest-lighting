# Lamp posts

Lamp posts with 1 meter of ws2812b with a PIR sensor and powered by a 12 volt battery.

## Parts list

Arduino nano

12v SLA battery
 * 20ah
 * From recovered servers - Lumos said he can hook us up with some

WS2812b strip
 * black pcb
 * with ip67 coating

PIR sensor
  * SR501

Switch
 * Toggle switch with ip67 coating to make waterproof

[Enclosure](http://www.aliexpress.com/item/1pc-Waterproof-Clear-Cover-Plastic-Electronic-Project-Box-Enclosure-Case-100x68x50mm)
 * Waterproof with clear plastic lid
 * 100x68x50mm

Buck converter
 * Downsample 12+ volts to 5 volts for the rgb strip

[Voltmeter](http://www.aliexpress.com/item/1pc-LCD-digital-voltmeter-ammeter-voltimetro-Red-LED-Amp-amperimetro-Volt-Meter-Gauge-voltage-meter-DC/32582100596.html)
 * 23ma current

12 volt jack
 * 2.1mm power jack

[3 pin JST plug](http://www.aliexpress.com/item/20-Pair-3-Pin-JST-SM-Connectors-For-WS2812B-WS2811-WS2812-LED-Strip-Female-Male/1535958145.html)
 * 3 pin plug for LED

Structure
 * 2m of wood for vertical
 * 50cm of wood for diagonal
 * Bolt to secure diagonal to vertical
 * Rebar to hammer into ground as a stand
 * Predrill hole for bolt and rebar

## History

New for kiwiburn 2017.

## Constants

N(leds): 60
V(max): 14.4V
V(shutoff): 12.0V
T(sundown): 21:30
T(shutdown): 04:00
Pin(data): 8
Pin(pir): 9

## Programming

Arduino powers up, the led strip goes into 'attract' mode, a low power sparkle that lights up only a few leds in firefly fashion. It samples the PIR so that when someone walks past, the full strip fades up. It stays at full brightness until someone walks away again. Only fades in colours, not full white, to save power usage.

Arduino shuts off at T(shutdown) automatically to prevent wasting power.

## Construction

Led strip is cut to length and a 1m 3 core wire is attached. Led strip is weather sealed and top and bottom with hot glue.

Power jack, switch and JST are all mounted on bottom face of box. Electronics are mounted inside box. Box has two tabs glued on.

Vertical support is cut with a mitre at 30° and has hole predrilled for bolt to secure diagonal.

Support is assembled on site, and led strip mounted, using clear duct tape.

Box is screwed to lamp post through tabs. Cables are connected. Battery mounted at base of lamp post.

## Wiring

Power jack earth goes to buck converter earth.

Power jack positive goes to switch pole 1. 

Switch pole 2 goes to buck converter. 

Voltmeter is connected across buck converter input.

Buck converter output positive goes to JST plug, arduino Vin.

Buck converter output earth goes to JST plug, arduino earth.

Arduino Vin goes to PIR Vin.

Arduino Earth goes to PIR Earth.

JST data line goes to pin 8.

PIR sensor in goes to pin 9.


## Notes

It would be possible to use a voltage divider to measure the battery voltage and display it on the strip, but it may be easier to use a standalone voltmeter unit. This is less wiring and testing, but means we don't get auto shutdown when the battery gets too low. It does make wandering around and inspecting the lamp post voltages a lot easier.
