Gloomhaven Helper RFID
=

Connect an RFID reader to your Gloomhaven Helper app, and scan tags to interface with Gloomhaven Helper app.

Gloomhaven Helper app is about reducing the number of components on the board when playing Gloomhaven Board Game. The problem with the app is that it takes the focus off the board, and onto a screen (if you play a boardgame, you kind of want to avoid screens, otherwise you could just play a video game).

This project is about using the board game to interface with the app by placing rfid taggs onto tokens, cards, standees, minis etc. Then you can scan the tag and send an action to Gloomhaven Helper app.

Examples:

    - Scan your ability card and automatically set your initiative to that cards initiative (visible ass ?? in the app until round begins)
    - Scan miniature/standee/card and automatically add new enemy.
    - Scan Element tokens to put the element to strong.
    - Scan Summon token to summon with preset HP, attack, move, range

Video of scanning a tag/card and changing initiative
-

<video style="width:100%" controls>
  <source src="https://seafile.robingronberg.se/seafhttp/files/1bea010c-1881-4e7d-bfbe-e064c62a860f/ghh_rfid.mp4" type="video/mp4">
  <source src="https://seafile.robingronberg.se/seafhttp/files/c277eec2-a163-4c1a-bc97-cdbea153e3cf/ghh_rfid.webm" type="video/webm">
Your browser does not support the video tag.
</video>

The app has access to the entire game state, so any action is possible. Each tag can be programmed to perform a specific action.

To implement this, an LR rfid, 125kHz (rmd6300/rmd630) reader is used with an ESP8266 Wifi enabled microcontroller (tested with Wemos Mini lite).

It would be nice if NFC tags (HF 13.56MHz rfid tags) could be used. This would require another rfid reader and other tags. These tags are smaller (requires smaller antenna), can write data (EM4100 only holds 4 static bytes) and is compatible with most nfc enabled phones. If you want to contribute send a pull request.

Development setup:
-

Use platformio. If you have problems setting up your develop environment, building the software or similar, open an issue, and we can improve the docs from there.

Components needed:
-
    - Wemos mini lite (basically an ESP8266 with a few Accessible GPIO:s)
    - RMD6300 125kHz EM4100 rfid scanner
    - 3pcs 10kOhm-30kOhm resistors (rmd6300 uses 5V, we want 3.3V for ESP8266)
    - 1pcs led
    - wire
    - Alot of EM4100 compatible RFID tags

TODO/fix:
-
    - When tag is scanned, use the id of the tag and perform action based on id
    - Add support for different app versions (only 7.6 is tested and works right now)
    - Programming of tags (web interface?)
    - Bug: Sometimes, if the device is restarted and then a previous client is connected (hosting the Gloomhaven Helper Server) connects, the device does not attempt to connect.
    - Document Schematics
    - Write build guide
    - Design a case
    - Print the case

Done:
-
    - Communicate with RFID reader (rmd6300)
    - Connect to tcp server when a client connects (hard coded to port 58888)
    - Read game state from Gloomhaven Helper tcp server
    - Write game state to Gloomhaven Helper tcp server when a card is scanned.

License: MIT

Contributers

    - Robin Grönberg
