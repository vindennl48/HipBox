# HipBox v6.0.1

## What is it?

'HipBox' is an 'In-Ear Monitoring System' for a music band.  Each member has independent control over the volume of each band member in their headphones during a practice or show.  This software provides a smartphone GUI that can interact with sliders to adjust volume, and buttons to mute or solo specific band members.  This also allows for recording and uploading practices, jams, or shows to a google drive account.

## Why the new version?

The v6 is because we are completely ditching Ableton Live (DAW music software).. and Windows for that matter.  This is strictly a linux based distribution now.  There were many reasons for going this route, a couple being the complexity and overhead of v5.  This system is designed to be ran on a singleboard computer, specifically an 'odroid'.  In my tests, this was done in an Arch Linux distro running on an HP EliteBook 8440p. (the slowest computer I could get my hands on).

## Features

 - Rails interface pushed to port 80
    - Login screen for all band members
       - Each member has their own mixing interface which is saved for every reboot.
    - Sliders for all channels, ie. band members, click track, backtracks, headphone volume, etc..
    - Mute button for all channels except for headphone volume (defeats the purpose?)
    - Solo button per band member
       - This is to isolate yourself from the rest of the band.  When active, only YOU can hear what you are playing, and muted to everyone else.  A blue hue disabled solo button is shown to everyone else signifying that you are in 'solo mode'.
    - Record button which records all inputs to separate mp3 files, as well as a scratch-mix mp3 of selected inputs you would like to have premixed together.
 - SimpleDAW
    - This is a very simple audio engine run by OSC.  This can start and stop a click track and backtracks simultaneously.
 - Integrated Guitarix channels
    - If you have guitarists or bassist that are practicing in dorm-rooms, apartments, or anywhere that can not be audibly destroyed, then plugging straight in, you can be routed through Guitarix, an amp simulation plugin.  
    - This can be run with or without a GUI.  
    - Can save presets per user

## Dependencies

 - JackMinimix
   - https://github.com/vindennl48/jackminimix
     - You will need to build this on your own, there is custom code in this fork that is not apart of the release set.
     - Commit '54372dd' was used at the time of this release.
 - Jack_Capture
   - https://github.com/vindennl48/jack_capture
 - Guitarix-0.37.3
 - Python-3.7.2
 - Postgresql-11.1
 - Ruby-2.5.1
 - Rails-5.2.1
