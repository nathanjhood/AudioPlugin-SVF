# AudioPlugin
JUCE Audio Plugin template (under construction)

# Built-in features:
+ variable oversampling up to 16x
+ variable processing precision (floats or doubles)
+ dry/wet mix with optional latency compensation
+ master output gain for "wet" signal
+ local context bypass switch
+ master bypass switch
+ defeatable GUI (requires window close/re-open)
+ de-coupled processing and true-bypass of audio processors in a lock-free atomic structure

# Before we begin...

Coffee! That's how I get things done!! If you'd like to see me get more things done, please kindly consider <a href="https://www.patreon.com/bePatron?u=8549187" data-patreon-widget-type="become-patron-button">buying me a coffee</a> or two ;)

<p align="center">
 <a href= "https://paypal.me/StoneyDSPAudio?country.x=ES&locale.x=en_US"><img src="https://www.paypalobjects.com/en_US/i/btn/btn_donate_SM.gif"/></a>
</p>

<p align="center">
 <a href= "https://twitter.com/Stoney_DSP/"><img src="https://github.com/StoneyDSP/StoneyDSP/blob/a075caeedffe23b2733ee38b12f9800f62aab9c2/Assets/twitter.png"/></a>
 <a href= "https://www.instagram.com/stoney_dsp/"><img src="https://github.com/StoneyDSP/StoneyDSP/blob/2253d684ba99e6c072353a94b49315162c381406/Assets/instagram.png"/></a>
 <a href= "https://www.facebook.com/StoneyDSP.Audio/"><img src="https://github.com/StoneyDSP/StoneyDSP/blob/9608562b09ee2708affd0c31117fc25a235672d9/Assets/facebook.png"/></a>
</p>

# Manual - v1.0.0b

+ **IO** - Toggles the individual processContext on or off (useful in duplicates for independant bypass of several chained processors).
+ **Oversampling** - Increasing the oversampling will improve performance at high frequencies - at the cost of more CPU!
+ **Mix** - Blend between the effected (100%) and the dry signal (0%). Optional latency compensation.
+ **Precision** - Switch between Float precision (High Quality) and Double precision (beyond High Quality) in the audio path.
+ **Bypass** - Toggles the entire plugin on or off.
+ **GUI** - Toggles between a generic, automated GUI arrangement, or the custom one defined in the Editor files (requires window close/re-open)

For further information, please continue reading. 

# Architecture of an AudioPlugin

The plugin template consists of four containers, each dedicated to it's own role:

+ **Processor** - contains all the interactions between DAW/host, and plugin
+ **Parameters** - contains all the control information to be accessed by the Editor and sent to the Wrapper
+ **Editor** - contains all graphical elements for user interaction
+ **Wrapper** - contains all the functions and audio processors of the plugin

Each of the above four containers is divided across two files;

+ **.h (header) file** - defines the functions of the container and exposes them as an interface
+ **.cpp (source) file** - defines the implementations of said functions of the container

All four containers are connected by a single **"Audio Processor Value Tree State" ("APVTS")** object. This **"APVTS"** object is created and held in the Processor container, which then passes it forward to each of the other containers, before it is passed back to the Processor for further work.

The Parameters container comes first - our plugin's internal parameters are created and defined in this container; including their various types (float value, boolean, multiple choice, etc), their min and max values, skew factors, and other details. These internal parameter objects (such as *"volumeFader"*) are then passed into the APVTS, along with a relevant reference name ID (such as *"volumeID"*).

The Editor container is where our graphical elements, including knobs and sliders, are created and drawn to the screen. This container then takes in the APVTS, looks up the list of parameters we have filled it with, and attaches our graphical elements to the desired internal parameters, using the previously-allocated reference name ID (such as *"volumeID"*); thus, exposing our plugin's internal parameters to user control via the GUI. 

Finally, the Wrapper container creates and holds whatever audio DSP effects and processors we are implementing in our plugin. Each audio effect/processor's internal parameters (such as *"setVolume(type newVolume)"*) are exposed to the APVTS inside this container; corresponding parameters are then connected to each other (again by reference such as *"volumeID"*) by a shared memory allocation (such as *"volumePtr"*), which is strictly written to by the parameter and read by the audio, and *never* the other way around.

The Processor container provides some powerful connectivity between the APVTS and the outside world (the DAW/host). Here, the APVTS is given read/write access to memory (for presets, undo/redo, project re-loading, etc) using some XML functions. This means that every single object connected to the APVTS (usually meaning, parameter) can read from and write to it's own unique little memory address, which is a unique location on an object called a "value tree". This means that our audio processor now has a memory of it's own internal "state" - or, rather, it has an **"Audio Processor Value Tree state"**.

More details soon. Please see my [Biquads repo](https://github.com/StoneyDSP/Biquads) for an example implementation meanwhile :)

Don't forget you can always shoot me a PM - you can also look me up on most of the usual DSP/Audio software sites under @StoneyDSP!

# To be done...
+ basic GUI for the built-in parameters
+ fix precision switching
+ fix variable number of channels/buses with oversampling
+ provide more examples...

# ...not sure where to look?

If perhaps you're new to the juce framework, or maybe even coding audio plugins in general, the above info might not be quite enough to guide you to get started on building a working audio plugin. Further guidance can be found all over the internet - in particular, the juce website and forum itself, as well as many popular youTube channels - but perhaps a further nudge wouldn't hurt :)

You might start by exploring the "PluginWrapper.h/.cpp" files, as this is where all of the DSP and such is happening to the audio (check what's happening in "processSample"). This is the container where we call for a DSP module, attach parameters to it, and send our audio in/out of it. There is already a dry/wet mixer, and simple gain object, instantiated(in the .h or "header" file) and placed in the signal path (in the .cpp or "source" file). Can you also see where the parameters are being updated, and by what?

Inside the "PluginParameter.h/.cpp" files, is a thankfully much less complex scene, and perhaps quite easy to grasp at a glance, compared to the Wrapper; these files are where we create and set ranges for our parameters. If we again look at the useage of the "header" and "source" files, we will see a similar pattern to what was in the Wrapper; that is, our "objects" (parameters) are "instantiated" (created) in the header file, and defined in the source file. This seperation and relationship is an important practice in "good coding" - though not *always* necessary.

So what's the difference between the Parameter container, and the "PluginEditor.h/.cpp" files? Things look almost mistakeably similar at first, don't they? Actually, this container is strictly working with your display thread, to draw user-controllable elements (and coloured backgrounds, etc) and place them around within the plugin window. One of the main (visual) similarities to the Parameter container, is the list of our parameters in the "constructor", each following by a "cast" function and a command to "getParameter" from something called "apvts"...

The "PluginProcessor.h/.cpp" files are something like the brain, or the motherboard, of our plugin. There are fewer user-controllable elements, and those on offer have a deeper relationship between the plugin, and the DAW or host in which the plugin is currently operating. This template has largely pre-configured the Processor files to allow for the widest set of expressions downstream (such as variable channel counts and processing precisions), but of course can be tinkered with as much as is necessary. It's thoroughly worth exploring the concept of "polymorphism" at play here, by looking for the call "public juce::AudioProcessor" and viewing it's definition(s). You can then trace how a similar method has been implemented in *all* of our containers, allowing us to query very low-level information, such as channel count, block size, or total latency, at *any* point in our plugin's architecture - just simply type "audioProcessor(dot)." and a list of functions will appear at your dispoal. This is a very powerful tool to have on hand when building, and further allows us to leave the Processor container alone, to act as a passive "translator" between the DAW/host and the plugin (*and* the user!).

One of the deepest features of the Processor is the previously-described "Audio Processor Value Tree State". In my template, I have renamed this object to simply "APVTS" (or lower "apvts") for readability. Now that you've observed how an "object" is often "instantiated" in a header file, then "implemented" in source file, can you see where we're creating this APVTS object, and then what we're doing with it?

If so, try having another read of the previous brief tutorial posted above :) 

...and stay tuned for more!

- Nathan (StoneyDSP) May 2022

# Before you go...

Coffee! That's how I get things done!! If you'd like to see me get more things done, please kindly consider <a href="https://www.patreon.com/bePatron?u=8549187" data-patreon-widget-type="become-patron-button">buying me a coffee</a> or two ;)

<p align="center">
 <a href= "https://paypal.me/StoneyDSPAudio?country.x=ES&locale.x=en_US"><img src="https://www.paypalobjects.com/en_US/i/btn/btn_donate_SM.gif"/></a>
</p>

<p align="center">
 <a href= "https://twitter.com/Stoney_DSP/"><img src="https://github.com/StoneyDSP/StoneyDSP/blob/a075caeedffe23b2733ee38b12f9800f62aab9c2/Assets/twitter.png"/></a>
 <a href= "https://www.instagram.com/stoney_dsp/"><img src="https://github.com/StoneyDSP/StoneyDSP/blob/2253d684ba99e6c072353a94b49315162c381406/Assets/instagram.png"/></a>
 <a href= "https://www.facebook.com/StoneyDSP.Audio/"><img src="https://github.com/StoneyDSP/StoneyDSP/blob/9608562b09ee2708affd0c31117fc25a235672d9/Assets/facebook.png"/></a>
</p>
