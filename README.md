# Killswitch.vst3
A lightweight killswitch VST plugin for audio and midi. Essentially an on/off switch for any point in your chain.

## Table of Contents
- [Behavior](#behavior)
- [MIDI is quirky](#midi-is-quirky)
- [Installation](#installation)
  * [Windows](#windows)
  * [OSX](#osx)
  * [Linux](#linux)

<small><i><a href='http://ecotrust-canada.github.io/markdown-toc/'>Table of contents generated with markdown-toc</a></i></small>


<a href='https://ko-fi.com/mackenziehnc' target='_blank'><img height='35' style='border:0px;height:46px;' src='https://az743702.vo.msecnd.net/cdn/kofi3.png?v=0' border='0' alt='Buy Me a Coffee at ko-fi.com' />
 

# Behavior
## Audio
When placed after something that generates audio, completely blocks signal from passing through. Sounds exactly like you expect if you're going for that Buckethead sound.

## MIDI
When placed after something that generates MIDI, behaves almost identically. It prevents "note on" messages from passing through, and sends a "note off" message to any already-playing notes. 
 
If "Resume withheld MIDI notes" is selected, it passes a new "note on" message for any notes that would otherwise be playing once the Killswitch is released. However, MIDI is quirky, so it might not sound exactly like you're expecting.
 
It also has a setting to block non-note messages.
 
[Watch a demo](https://www.youtube.com/watch?v=xBmSd2P3eGI)

# MIDI is quirky
While normal audio sends new information every frame, MIDI only tells a note to start playing, and then later tells it when to stop. Also, there is no function to change a note's velocity once it has started playing. This means that in order to "resume" a note, the code has to generate a new note. The difference won't be noticeable on low-attack/high-sustain instruments, but on attacky synths that decay quickly (like a harp, for example), you'll hear the difference immediately.

For a more thorough explanation of MIDI, check out [the official MIDI Association's articles](https://www.midi.org/midi-articles/about-midi-part-3-midi-messages).
 
It is possible to add a "decay" setting, so that the note will not return as loudly, but that would have to be set per-instrument, since my code has no way to predict what would "sound right". If there is interest, I will add this setting. However, if that's the sound you're looking for, I would suggest simply placing the plugin after your synthesizer.

# Installation
## Windows
COMING SOON

## OSX
COMING SOON

## Linux
¯\\_(ツ)\_/¯ I'm so sorry.

Supposedly, [LinVST](https://github.com/osxmidi/LinVst) will let you use the Windows installation instructions. Otherwise, you will have to learn how to install Juce and an IDE, then build this project in the usual way.

