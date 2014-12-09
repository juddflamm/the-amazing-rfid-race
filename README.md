The Amazing RFID Race
=====================

[Read my blog post about this...](http://megagalactic.io/the-amazing-rfid-race/)

Mashup of Pinocc.io Arduino hardware, mesh networking, RFID card readers, AngularJS, and Firebase to produce a relay race with an RFID card as the batton.

#Pinocc.io (Arduino w/ Mesh Networking)
The pinoccio-arduino-sketch folder contains an arduino sketch specifically for the Pinocc.io scout devices.  You will need to setup the latest Arduino IDE and import the Pinocc.io arduino library, before attempting to compile and upload this sketch to a Pinocc.io scout.

This sketch will enable the Pinocc.io to read RFID cards using the inexpensive MFRC522 RFID reader / writer board.

The sketch configures a 2 second RGB LED signal that the card was read, as well as triggers a 3 second timeout before reading another RFID card.  Without this the board would read the same card multiple times per second.

Additionally, upon reading an RFID card, the sketch will instruct the Pinocc.io to fire an event to the Pinocc.io API / HQ, via the ScoutScript command: hq.report(eventName, eventVals...)

#Web Application
The webapp folder contains a single index.html file that listens to the stream of events from the Pinocc.io arduino relayed by the Pinocc.io API.

It interprets the RFID card reads as checkpoints in the race, and saves the race data to Firebase for persistence, and realtime syncing with any other client listening to the Firebase database.

The index.html may be hosted any way you prefer.
