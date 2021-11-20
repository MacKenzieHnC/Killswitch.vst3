# Killswitch.vst3
A lightweight killswitch plugin for audio and midi. Essentially an on/off switch for any point in your chain.

<a href='https://ko-fi.com/mackenziehnc' target='_blank'><img height='35' style='border:0px;height:46px;' src='https://az743702.vo.msecnd.net/cdn/kofi3.png?v=0' border='0' alt='Buy Me a Coffee at ko-fi.com' />
 

# Behavior
When used on audio input, completely blocks signal from passing through.

When used on midi, behaves almost identically. However, [Midi is quirky.](#midi-is-quirky) tl;dr It prevents "note on" messages from passing through, and silences any already playing notes. When killswitch is released, it passes a new "note on" message for any notes that would be playing if the killswitch wasn't active. At this time, non-note messages pass through unaffected.

# Installation
## Windows
Windows users get preferential treatment. Simply download the .vst3 file from the "Distributions" folder and copy it into wherever you keep your VSTs (typically "C:\Program Files (x86)\Common Files\VST3").

## OSX
I'm sorry, but it won't be as easy for you. Due to Apple's licensing fees, I literally cannot afford to make this easy for you. If someone sends me the $99 to join the Apple Developer Program, I will happily make an installable distribution.

__1.__ [Download and install Xcode from the App Store](https://apps.apple.com/us/app/xcode/id497799835?ls=1&mt=12).

__2.__ [download the free version of JUCE](https://juce.com/get-juce).

__3.__ Unzip the folder

__4.__ Download and unzip this repo and open the .jucer file in Projucer (either double-click the .jucer file, or if that doesn't work, open Projucer and go to "File > Open"). 

__5.__ Go to "File>Open in IDE...". This should open Xcode.

__6.__ In Xcode, select "Product > Run"

## Linux
¯\\_(ツ)\_/¯ I'm so sorry.

Supposedly, [LinVST](https://github.com/osxmidi/LinVst) will let you use the Windows installation instructions. Otherwise, you will have to learn how to install Juce and an IDE, then build this project in the usual way.

# Midi is quirky
While normal audio sends new information every frame, midi only tells a note to start playing, and then later tells it when to stop. Also, there is no function to change a note's velocity once it has started playing. This means that in order to "resume" a note, the code has to generate a new note. The difference won't be noticeable on low-attack/high-sustain instruments, but on high-attack/low-sustain instruments, you'll hear the difference immediately.

For a more thorough explanation of midi, check out [the official MIDI Association's articles](https://www.midi.org/midi-articles/about-midi-part-3-midi-messages).
