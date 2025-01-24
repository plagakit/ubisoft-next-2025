# Speedgolf! - Ubisoft Next 2025 Programming Submission

For Ubisoft Toronto's NEXT competition, the theme for 2025 was "Minigolf". My submission is titled *Speedgolf!* and is a golf-racing game where the player has to blaze through procedurally generated golf courses as fast as possible and see how many rounds they can make it through before their time runs out. It uses the NEXT API and a mini game-engine-like static library built on top of it that I wrote over the course of the competition to use multiple features:
- ECS style game architecture w/ generic memory pools for components
- 3D software renderer that supports both Painter's algorithm & depth-buffering
- Extensible GUI system inspired by ROBLOX's GUI
- Event-driven structure using Signals & type-safe callbacks
- Basic 2D physics
- Buncha other stuff

Even though the competition is done, the plan is to eventually support OpenGL and GLFW as another "backend" for Engine so that I can eventually use Emscripten to compile this to WebAssembly and host this on my website :D

![gameplay footage](speedgolf.gif)

## Building

1. Clone the repository and open `ubisoft-next-2025.sln` in Visual Studio.  
2. Set "Minigolf" as the Startup Project (right-click → "Set as Startup Project").  
3. Optional, for better graphics:  
   - Switch to **Release** configuration.  
   - Stay in **Debug** configuration, and undefine `USE_DEBUG_RASTER_DOWNSCALING` in `Engine/core/app_settings.h`.  
4. Press **Run**!

## Resources
There were a looooot of helpful resources that contributed to the development of this project. Many of them are scattered around the codebase in comments if they pertain to a specific topic, but most of the general ones are the following:

- [Game Engine Architecture](https://www.gameenginebook.com/) by Jason Gregory
- [ECS Back and Forth](https://skypjack.github.io/2019-02-14-ecs-baf-part-1/) by Michele Caini
- [TheCherno](https://www.youtube.com/@TheCherno)'s YouTube channel
- [Optimizing Software Occlussion Culling](https://fgiesen.wordpress.com/2013/02/17/optimizing-sw-occlusion-culling-index/) by Fabian "ryg" Giesen
- [ROBLOX](https://create.roblox.com/docs/ui) & [Godot](https://godotengine.org/) for inspiration
- https://easings.net/ by Andrey Sitnik and Ivan Solovev
- My past submissions & judge feedback from my 2023 submission :D
- Many, many more resources!
