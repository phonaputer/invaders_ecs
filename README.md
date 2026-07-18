# Invaders ECS

_No LLMs were used in the generation of the source code, documentation, or assets (images, audio) in this repository. However, during implementation LLMs (specifically, Google search "AI Mode") were used to search through C++ and library docs, as well as find relevant information for specific error messages._

_This means any crap code is entirely my own fault._ 😉

---

This is a knockoff of the classic Taito game _Space Invaders_ written in C++ using [SDL3](https://wiki.libsdl.org/SDL3/FrontPage), [SDL3 Mixer](https://wiki.libsdl.org/SDL3_mixer/FrontPage), and [EnTT](https://github.com/skypjack/entt).
Unit tests use [GoogleTest](https://google.github.io/googletest/).

It was written to learn about game development patterns. _Space Invaders_ was chosen as it is complex enough to try some patterns but simple enough to write in a short time.

[It can be played on Itch.io here.](https://phonaputer.itch.io/invaders-ecs)

### Motivation

When I heard about the "ECS" / "Entity Component System" pattern used in many modern games, I was immediately curious to try it out.
Writing this "Space Invaders-esque" game was an excuse to do so.

While I have limited interest in creating video games for the sake of creating cool games, I am quite interested in how games are coded.
This is because the patterns & challenges inherent in game development are very different from what I work with at my day job in the web industry.

Put a bit vaguely, in web we write stateless applications which offload state into separately deployed databases -
and our performance problems arise from a large volume of concurrent users.
Whereas in games there is a massive amount of state stored within the application's memory -
and performance problems arise due to hardware limitations on a single machine.

So my primary motivation for writing this code was to get a newbie-level feel for what problems game developers have and what patterns they have created to solve them.

### Impressions

In comparison with [the initial, object-oriented version](https://github.com/phonaputer/personal_space_invaders) of this project, ECS felt much more natural to me.

I suspect this is due to separation of data and logic being standard in my industry. Since ECS also keeps data and logic separate, it feels way more similar to the kind of coding I do at work.
With ECS it also feels way easier to write unit tests since it's easy to set up preconditions in the "database" (ECS) and then run the logic on them.
Perhaps this is an indictment of my OOP programming in the first version of this project, but for me using the ECS felt cleaner.

Thinking a bit on the tradeoffs - ECS is a heavyweight pattern since it requires restructuring your game data & logic around it.
I suspect this would be even more pronounced if you were trying to get the performance benefits ECS purports to provide (I wasn't).
So if this style of codebase isn't your thing, ECS would be a pain.

Actually, before I settled on EnTT I also tried [Flecs](https://github.com/SanderMertens/flecs) which felt even more invasive since it wants to handle scheduling of your systems & provides pretty complex querying functions.
This reminded me a bit of Spring or Hibernate in the Java ecosystem where the libraries do so much for you that it can feel like you're programming Spring/Hibernate instead of Java. Flecs was too invasive even for me (I'm trying to learn a generalized pattern here, not a library) so I ended up going with the less full-featured EnTT.

At any rate, if I were to find out that I'm becoming a full-time game developer tomorrow and I'm not allowed to use existing engines for some reason, I would definitely go the ECS route rather than the "OOP" route.
Let's hope I don't become a full-time game dev though, since the world has enough shovelware games already! 😆

### Build & Run

This project uses [CMake](https://cmake.org/).
Presets for debug & release builds may be found in `CMakePresets.json`.

After building with CMake, unit tests may be run with the following command:

```
ctest --output-on-failure
```

And (on Linux) the game may be run with the following command:

```
./game
```
