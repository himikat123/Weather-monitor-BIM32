# JSON_Decoder

This is a fork of altrunner's variant of Daniel Eichhorn's library. altrunner converted the library to use char arrays instead of the String class. The update has the potential to reduce the parser memory footprint during decoding and I like the error reporting added in altrunner's variant.

This fork here has been renamed JSON_Decoder to avoid conflicts in the Arduino IDE with Daniel's original "json-streaming-parser" library:

https://github.com/squix78/json-streaming-parser

Thus the two libraries can happily co-exist. The example sketch has been corrected to work with an update that added error reporting in altrunners fork.

This version of the library will also throw away characters in the GET response until the first '{' is found, which marks the start of a JSON object. This means you do not need to discard the response header yourself.

The SpaceStation example has been added to show how to fetch and decode the JSON messages from a website. This is about as simple as I could make it and the example includes comments that explain what the various bits of the example code do.

The streaming parser is very efficient on memory use compared to the Arduino JSON library because space only needs to be reserved temporarily to store the elements from the message that you need, for example the Arduino library required 55kbytes of RAM to decode a 28kbyte JSON message which made it unusable on an ESP8266.  The price paid for this memory efficiency is that you must write the code to pull out the values you need. The SpaceStation example includes links so you can see the JSON message that is being parsed in your web browser.

The following Weather forecast libraries use this JSON_Decoder version of the streaming parser:

https://github.com/Bodmer/ApixuWeather

https://github.com/Bodmer/DarkSkyWeather

This library is based on all the hard work by Daniel Eichhorn: https://github.com/squix78

The original ReadMe for his source library is below.

# json-streaming-parser
Arduino library for parsing potentially huge json streams on devices with scarce memory.

This library is a port of Salsify's PHP based json streaming parser (https://github.com/salsify/jsonstreamingparser).

## Why yet another JSON parser?

When working with small (connected) devices you might quickly get to the point where you need to process potentially huge JSON object received from a REST interface.
All libraries for processing JSON objects on the Arduino platform had some deficiencies which led me to create one on my own.
Not really being an expert in writing parsers I miserably failed the first time. 

This is the second attempt and I took Salsify's PHP based parser and ported it to C++. 
This library has been tested on the Arduino/ESP8266 target platform but should work in theory on all platforms available for the Arduino environment

## Why a streaming parser?

Generally speaking when parsing data you have two options to make sense of this data: 
you either create a document object model (DOM) which contains the whole information of that document and lets you retrieve the
nodes with random access. A DOM parser needs the whole document to start parsing and only lets you access the data
after it has finished the process.
The other option you have when parsing a document is to process it char by char (or byte by byte) while it becomes
available to your client code. Compared to the DOM parser this has two advantages: a) you can react
the data as soon as meaningful parts are available and b) you can drop information as soon as the parser has processed
it. This reduces the memory consumption a lot, especially if you retrieve huge documents when only being interested
by a small subset of it. But this efficiency comes at a price: your code will have to do more "magic" than with a
DOM parser, the business logic becomes part of the parser.

## How to install

Until the library becomes available in the Arduino IDE library manager you'll have to do a bit more work by hand.
1) Download this library: https://github.com/squix78/json-streaming-parser/archive/master.zip
2) Rename master.zip to json-streaming-parser.zip
3) Open the zip file in the Arduino IDE from menu Sketch > Include Library > Add ZIP Library...

## How to use

This is a streaming parser, which means that you feed a stream of chars into the parser and you take out from that
stream whatever you are interested in. In order to do that you will create a subclass of JsonListener class and
implement methods which will be notified in case of certain events in the feed occure. Available events are:

 * startDocument()
 * key(const char *key)
 * value(const char *value)
 * endArray()
 * endObject()
 * endDocument()
 * startArray()
 * startObject()
 * error(const char *message)

In your implementation of these methods you will have to write problem specific code to find the parts of the document that you are interested in. Please see the example to understand what that means. In the example the ExampleListener implements the event methods declared in the JsonListener interface and prints to the serial console when they are called.

## License

This code is available under the MIT license, which basically means that you can use, modify the distribute the code as long as you give credits to me (and Salsify) and add a reference back to this repository. Please read https://github.com/squix78/json-streaming-parser/blob/master/LICENSE for more detail...

## Credits

First of all I'd like to thank Salsify for making their PHP parser available to the public. You find their repository here: https://github.com/salsify/jsonstreamingparser

Then I'd like to thank my employer Netcetera (https://github.com/netceteragroup) to let us hackers go twice a year to the CodeCamp and work on software projects like this one.

And last but not least I'd like to thank my wife that she led me spend three days away from the family hacking in the wonderful mountains of Berne.
