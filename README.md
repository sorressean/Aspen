Aspen
=====

## Synopsis

Aspen is a light-weight, easy-to-use barebones mud engine written in c++, with [Angelscript](http://angelcode.com/angelscript/) as its' scripting engine.

## purpose
Aspen was created to address many issues:
* Current mud engines are large and usually ship ready-to-play, which may not be what the developer wants. This requires hours, if not days and weeks spent pulling out systems and revamping code to survive and compile without those systems.
* Many engines ship with restrictive licenses, which prevent the developer from accepting donations.
* Many engines are still using incredibly old code and are written in C. While this is not a huge problem in and of itself, we decided to go all the way and use c++ so that basic data structures and other utilities are handled for you.
* Many engines do not allow the addition of modular code. This means that in order to add your own logic and systems, you must hack and slash them in. While this cannot be avoided altogether, we strive to allow for Aspen to be as modular as possible: you add modules and do not need to touch the core unless you require changes to the logic.

## History
I began work on aspen in late 2009, after trying out a few different engines for what I wanted. I ran into all of the issues outlined above, in one form or another before I finally decided to begin work on Aspen. For a while, we were hosted on Googlecode before I decided to convert this into my own mud and work on it closed source. I've explained my reasons and motivations for open sourcing the project [here](http://tysdomain.com/blog/?p=192).

## Is Aspen ready-to-use?
The short answer is probably not. The even longer answer is no; Aspen is currently in development, and as such is not ready for primetime. We still welcome anyone who wishes to use it; if you would like to contribute to the engine, we would greatly appreciate any contributions. Our todo list can     be found [here](https://github.com/sorressean/aspen/wiki/todo). You are more than welcome to work on any individual items, as well as any other section of your choosing.

## Community
We do not currently have a mailing list in place for discussions; you can find us on IRC: irc.freenode.net, #aspenmud.

## Support the project
This project is driven by one developer; I have a lot of other constraints on my time, as much as I'd like to dedicate full-time to this project. If you are interested in this project, please feel free to either donate or get in touch with me, I'd love to know how it is being used. If you wish to donate, you may do so [here](http://tysdomain.com/page/donate).
