# AudioPlugin-SVF
Simple Multi-Mode State Variable Filter using TPT, built directly from the StoneyDSP AudioPlugin template repository and the juce State Variable TPT Filter module (with a few mods)...

![Build-01-06-2022](https://github.com/StoneyDSP/AudioPlugin-SVF/blob/234151b0e6595737b33ef5a8f1afb08eb9214194/Res/SVF-AutoGUI-1-0-1.png)

This plugin is left quite bare-bones as it is intended to show how to integrate the AudioPlugin template repository with the off-the-shelf juce modules and ways of working (as best I can).

One should be able to compare the AudioPlugin template project against this source code, and easily spot how we've instantiated a dsp module and interacted with it via the APVTS.

# Built-in features:
+ variable oversampling up to 16x
+ variable processing precision (floats or doubles)
+ dry/wet mix with optional latency compensation
+ master output gain for "wet" signal
+ local context bypass switch
+ master bypass switch
+ resizeable GUI
+ de-coupled processing and true-bypass of audio processors in a lock-free atomic structure

# Before we begin...

Coffee! That's how I get things done!! If you'd like to see me get more things done, please kindly consider <a href="https://www.patreon.com/bePatron?u=8549187" data-patreon-widget-type="become-patron-button">buying me a coffee</a> or two ;)

<p align="center">
 <a href= "https://paypal.me/StoneyDSPAudio?country.x=ES&locale.x=en_US"><img src="https://www.paypalobjects.com/en_US/i/btn/btn_donate_SM.gif"/></a>
</p>

<p align="center">
 <a href= "https://twitter.com/Stoney_DSP/"><img src="https://github.com/StoneyDSP/StoneyDSP/blob/a075caeedffe23b2733ee38b12f9800f62aab9c2/Assets/twitter.png"/></a>
 <a href= "https://www.instagram.com/stoney.d.s.p/"><img src="https://github.com/StoneyDSP/StoneyDSP/blob/2253d684ba99e6c072353a94b49315162c381406/Assets/instagram.png"/></a>
 <a href= "https://www.facebook.com/StoneyDSP.Audio/"><img src="https://github.com/StoneyDSP/StoneyDSP/blob/9608562b09ee2708affd0c31117fc25a235672d9/Assets/facebook.png"/></a>
</p>

# Manual - v1.0.0b

+ Frequency - Sets the centre frequency of the equalizer filter.
+ Resonance - Increases the amount of "emphasis" of the corner frequency
+ Type* - Chooses the type of filter to use. See below for more.
+ Oversampling - Increasing the oversampling will improve performance at high frequencies - at the cost of more CPU!
+ Mix - Blend between the filter affect (100%) and the dry signal (0%).
+ Bypass - Toggles the entire plugin on or off.

For further information, please continue reading at https://github.com/StoneyDSP/AudioPlugin

# Before you go...

Coffee! That's how I get things done!! If you'd like to see me get more things done, please kindly consider <a href="https://www.patreon.com/bePatron?u=8549187" data-patreon-widget-type="become-patron-button">buying me a coffee</a> or two ;)

<p align="center">
 <a href= "https://paypal.me/StoneyDSPAudio?country.x=ES&locale.x=en_US"><img src="https://www.paypalobjects.com/en_US/i/btn/btn_donate_SM.gif"/></a>
</p>

<p align="center">
 <a href= "https://twitter.com/Stoney_DSP/"><img src="https://github.com/StoneyDSP/StoneyDSP/blob/a075caeedffe23b2733ee38b12f9800f62aab9c2/Assets/twitter.png"/></a>
 <a href= "https://www.instagram.com/stoney.d.s.p/"><img src="https://github.com/StoneyDSP/StoneyDSP/blob/2253d684ba99e6c072353a94b49315162c381406/Assets/instagram.png"/></a>
 <a href= "https://www.facebook.com/StoneyDSP.Audio/"><img src="https://github.com/StoneyDSP/StoneyDSP/blob/9608562b09ee2708affd0c31117fc25a235672d9/Assets/facebook.png"/></a>
</p>
