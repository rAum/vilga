# vilga

**under development, nothing usable yet and it's highly experimental code**

## What is it?
I am prototyping a debug tool inspired by my idea I used to find extremely useful in my previous commercial projects when I was working on monocular SLAM working on mobile/desktop platform.

This prototype is for more abstract tool, basically an async logger for non-textual data like images, matrices, point clouds etc which streams data over network socket.

Example use case is to provide real time debug visualization without much hassle (think about problems with multithreaded rendering/gui..) or to record data for later analysis (e.g. find the first difference to find what breaks determinism).
