# TikTok Live Games
Originally this was intended to just be a Twitch Plays type of deal with various games and emulators. 
Instead I took apart my kind of working GB advance SP and connected it to an ESP-32. This combined with the TikTok live server will
allow TikTok live commenters to pick the actions for the gameboy. 

This is not a good example of clean code or an example to follow just a fun hacked together project. 

The 2 different firmwares are for one with serial commands and another with just HTTP requests but it returns a pretty Gameboy with working buttons when you request `http://espIP/`

Shout out to https://codepen.io/zerratar for the HTML/CSS Gameboy I used for testing
