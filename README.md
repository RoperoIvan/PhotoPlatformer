# PhotoPlatformer

PhotoPlatformer is a 2D sidescrolling platformer where you are a mermelade toast that can make copies of itself to reach to inaccesible places.

## Getting started

This is a little game made by two students, Dídac Llop Serna & Ivan Ropero Garcia, of the CITM UPC center for the development assignment. 

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
- F9 view the colliders of the game
- F10 activate god mode( when you do it the player begins to fall, just jump and you will begin to fly without falling again when you are standing still)

## Tools used

- Visual Studio 2017/19
- Tiled
- SDL
- Pugi

## Art, sounds and music

[Main theme](https://opengameart.org/content/menu-music)

[Player](https://lhteam.itch.io/zombie-toast)

[Level 1](https://rottingpixels.itch.io/platformer-dungeon-tileset)

[Level 2](https://beyonderboy.itch.io/simple-platform-tileset-16x16)

## Authors

[Dídac Llop Serna](https://github.com/didaclis)

[Ivan Ropero Garcia](https://github.com/RoperoIvan)

[Repository](https://github.com/RoperoIvan/PhotoPlatformer)

## Innovations

### First assignment

- Animations loaded from Tiled, from a tsx
- Player copies handling feature
- Camera culling with the function IsOnCamera
- Colliders loaded and defined from Tiled
- Checkpoints also loaded and defined from Tiled

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
