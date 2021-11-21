# Killswitch.vst3
A lightweight killswitch VST plugin for audio and midi. Essentially an on/off switch for any point in your chain.

<a href='https://ko-fi.com/mackenziehnc' target='_blank'><img height='35' style='border:0px;height:46px;' src='https://az743702.vo.msecnd.net/cdn/kofi3.png?v=0' border='0' alt='Buy Me a Coffee at ko-fi.com' />
 

# Behavior
When used on audio input, completely blocks signal from passing through.

When used on MIDI, behaves almost identically. However, MIDI is quirky. tl;dr It prevents "note on" messages from passing through, and silences any already playing notes. When killswitch is released, it passes a new "note on" message for any notes that would be playing if the killswitch wasn't active. At this time, non-note messages pass through unaffected.
 
[Watch a demo](https://www.youtube.com/watch?v=xBmSd2P3eGI)

# Midi is quirky
While normal audio sends new information every frame, midi only tells a note to start playing, and then later tells it when to stop. Also, there is no function to change a note's velocity once it has started playing. This means that in order to "resume" a note, the code has to generate a new note. The difference won't be noticeable on low-attack/high-sustain instruments, but on high-attack/low-sustain instruments, you'll hear the difference immediately.

For a more thorough explanation of midi, check out [the official MIDI Association's articles](https://www.midi.org/midi-articles/about-midi-part-3-midi-messages).

# Installation
## Windows
Windows users get preferential treatment. [Simply download the "MacKenzieH&C" folder from "Windows" here](https://drive.google.com/drive/folders/1TzjdWOwznmpu03R45NptfqAQHsbZXNCO?usp=sharing) and copy it into wherever you keep your VSTs (typically "C:\Program Files (x86)\Common Files\VST3").

## OSX
I'm sorry, but it won't be as easy for you. Due to Apple's licensing fees, I literally cannot afford to make this easy for you. If someone sends me the $99 to join the Apple Developer Program, I will happily make an installable distribution.
 
### Option 1: The Easy Way (Probably won't work.)
__1.__ [Simply download the "MacKenzieH&C" folder from "macOS" here](https://drive.google.com/drive/folders/1TzjdWOwznmpu03R45NptfqAQHsbZXNCO?usp=sharing) and put it into "\~/Library/Audio/Plug-Ins/VST3" (if you can't find that folder, open Finder, press CMD-Shift-G and go to "\~/Library/Audio/Plug-Ins/VST3")

### Option 2: The Hard Way (Should work, and will let you build any open-source JUCE programs)
#### Setup
__1.__ Install Xcode from the App Store

__2.__ [Download JUCE.](https://juce.com/get-juce)

__4.__ Move JUCE folder to your Home folder (open Finder, press CMD-Shift-G, "\~/").

__5.__ Open Projucer, from the Juce folder from earlier.

__6.__ If you put the JUCE folder in a custom location, in the bottom left, it will complain that your module location is wrong. Click this warning (or go to "Projucer>Global Paths..."), and set the JUCE path to the JUCE folder, the modules path to the modules folder inside the JUCE folder

__7.__ You should now be able to build any JUCE programs, which is a lot, since they require free users to make their code open-source.

#### Build a JUCE program

__8.__ Download and unzip this repository

__9.__ Open the .jucer file from the downloaded repository

__10.__ Last bit of setup, and you shouldn't need it for my files, but it might help you troubleshoot other JUCE code. A problem I ran into is that the macOS build location defaults to "$(HOME)/Library/Audio/Plug-Ins/VST3". Go to "Exporters" on the left side of Projucer, "XCode (macOS), and in both the "Debug" and "Release" versions, change "VST3 Binary Location" to "\~/Library/Audio/Plug-Ins/VST3"

__11.__ Click the XCode button near the top of the window (white button with a blue circle and a hammer). This will open Xcode. We're almost done.

__12.__ Finally, click the play button toward the top-left of the XCode window. This will build the VST/AU into "\~/Library/Audio/Plug-Ins/VST3".

__13.__ You should now be able to use the plug-in as normal.

## Linux
¯\\_(ツ)\_/¯ I'm so sorry.

Supposedly, [LinVST](https://github.com/osxmidi/LinVst) will let you use the Windows installation instructions. Otherwise, you will have to learn how to install Juce and an IDE, then build this project in the usual way.

