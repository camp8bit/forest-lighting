# Jars

Small jar hanging in trees, with strip of 15 RGB leds sequenced by an arduino powered by an 18650.

## Parts list

Arduino pro 
 * mini 3.3 volt

18650 cell 
 * ~2300mah
 * From laptop battery packs

WS2812b strip
 * black pcb
 * with ip67 coating to prevent shorts

Switch
 * Toggle switch with ip67 coating to make waterproof

Jar
 * $3 jar
 * Possible but not necessary to wax coat the inside of jars

Accessories:
 * Heatshrink wrap to encapsulate arduino
 * Single 18650 holder

## History

Version 1, each jar had a 433mhz reciever in it. These worked well with ~20m range on the test bench, but when put into the final form in the jar, they (we guess) suffered from terrible EM interference and couldn't recieve a signal from 5cm away.

Related project - the singing jars are a different design.

## Constants

V(max): 4.2V
V(shutoff): 3.2V
T(pattern): 5 * 60 seconds
T(sundown): 21:30
T(shutdown): 03:00
Pin(data): 8

## Programming

Arduino boots up, and uses the internal voltmeter to measure the supply voltage. If the voltage is too low, it will flash the first led of the led strip with a red/off pattern to indicate low voltage.

If voltage is sufficient, the led strip will be lit up in a red -> yellow -> green gradient to indicate the voltage. The jar then starts into patterns. Each pattern runs for T(pattern) time then goes to the next one. This way all jars should be relatively in sync.

After T(shutdown) the jar should shut off until T(sunset) the next day. In shut off mode, we turn off all the leds and turn the arduino into low power mode. This means we don't need to turn off the jars each morning at T(shutdown).

## Wiring

The 18650 holder ground is soldered to ground on the arduino. The positive lead goes to one pole of the switch. The other pole goes to the Vin of the arduino. The power supply for the led strip goes to the arduino Vin. The ground of the strip goes to the arduino Vin. The data line is connected to Pin(data).