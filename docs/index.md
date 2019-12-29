# PhotoPlatformer

PhotoPlatformer is a 2D sidescrolling platformer where you are a marmalade toast that can make copies of itself to reach to inaccesible places.

<iframe width="560" height="315" src="https://www.youtube.com/embed/OwbZcCwomj4" frameborder="0" allow="autoplay; encrypted-media" allowfullscreen></iframe>

[Watch the game trailer](https://www.youtube.com/watch?v=OwbZcCwomj4&feature=youtu.be)

[Download the game](https://github.com/RoperoIvan/PhotoPlatformer/releases)

[See the repository code](https://github.com/RoperoIvan/PhotoPlatformer)

## Getting started

This is a little game made by two students, Dídac Llop Serna & Ivan Ropero Garcia, of the CITM UPC center for the development assignment. 

## Core subsystems

On the top of everything there is the main module called j1App.cpp . This module manages all the other modules calling in a loop its respective Awake, PreUpdate, Update, Postupdte, Cleanup that they share through a base class j1Module.h . Do not forget that there is a Main.cpp that has a switch of the different states said before.

Then the game is divided in 4 main blocks:

- The j1Scene where everything is printed managed by a module called j1Scene.
- The Modules that manage different aspects of the game such as: audio, input, collisions, renderer, console, etc…
- The j1Entities such as: coins, enemies and player; managed by a module called EntityManager.
- The j1Gui where there are some classes that represent diferent elements of gui such as: images, buttons, labels, sliders, checkbox, input boxes. This is managed by a module called j1Gui.

## Innovations

- Animations loaded from Tiled, from a tsx
- Player copies handling feature
- Camera culling with the function IsOnCamera
- Colliders loaded and defined from Tiled
- Checkpoints also loaded and defined from Tiled

## Members

<img src="https://github.com/RoperoIvan/PhotoPlatformer/blob/master/docs/WebsiteImages/group_photo.png?raw=true"/>

### Dídac Llop

<img src="https://github.com/RoperoIvan/PhotoPlatformer/blob/master/docs/WebsiteImages/dudec.png?raw=true"/>

- [Profile](https://github.com/didaclis)

#### Work done

- Info in XML
- Copies of the player feature
- Entities handling
- Entity class base
- Collisions
- Movement
- Camera Culling
- Camera
- Death
- CheckPoints
- Level making
- Pathfinding module
- Movement walking and flying enemy

### Ivan Ropero

<img src="https://github.com/RoperoIvan/PhotoPlatformer/blob/master/docs/WebsiteImages/ebon.png?raw=true"/>

- [Profile](https://github.com/RoperoIvan)

#### Work done

- Sounds and music
- Parallax
- Save and Load
- Debug keys
- Art
- Changing levels
- Sprites and animations from Tiled
- Level making
- DT implementation
- Enemy class
- Brofiler
- UML
- All gui
- Coins
- HUD


### Installation

- Download the latest version of the game here: [link](https://github.com/RoperoIvan/PhotoPlatformer/releases)
- Extract the zip
- Run the file that says PhotoPlatformer.exe

## Controls

- W to jump
- D to go the right
- A to go the left
- SPACEBAR to make a copy
- E to erase the copies

### Debug controls

- F1 go to the first level
- F2 go to the second level
- F3 start from the beginning of the current level
- F5 save the current state of the game
- F6 load the latest saved game
- F8 view debug UI
- F9 view the colliders of the game
- F10 activate god mode( when you do it the player begins to fall, just jump and you will begin to fly without falling again when you are standing still)
- F11 Enable/Disable FPS cap to 30

## Art, sounds, music and fonts credits

[Main theme](https://opengameart.org/content/menu-music)

[Player](https://lhteam.itch.io/zombie-toast)

[Level 1](https://rottingpixels.itch.io/platformer-dungeon-tileset)

[Level 2](https://beyonderboy.itch.io/simple-platform-tileset-16x16)

[GUI Font](https://www.dafont.com/wolfs-bane.font)

[GUI atlas](https://www.gameart2d.com/free-fantasy-game-gui.html)

[Main Menu Theme](https://www.bensound.com/)

## License

MIT License

Copyright (c) 2019 Dídac Llop Serna & Ivan Ropero Garcia

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
