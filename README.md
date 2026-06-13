# RaniEngineCmake
made by Rani Nagels for the course Programming 4 at Digital Arts and Entertainment, Howest.
https://raninagels.github.io/RaniEngineCmake/

# Bomberman 

[![CMake](https://github.com/RaniNagels/RaniEngineCmake/actions/workflows/cmake.yml/badge.svg?branch=main)](https://github.com/RaniNagels/RaniEngineCmake/actions/workflows/cmake.yml)
[![Emscripten](https://github.com/RaniNagels/RaniEngineCmake/actions/workflows/emscripten.yml/badge.svg)](https://github.com/RaniNagels/RaniEngineCmake/actions/workflows/emscripten.yml)
[![pages-build-deployment](https://github.com/RaniNagels/RaniEngineCmake/actions/workflows/pages/pages-build-deployment/badge.svg)](https://github.com/RaniNagels/RaniEngineCmake/actions/workflows/pages/pages-build-deployment)


## Introduction

Bomberman is a maze-based video game developed by Hudson Soft and released in 1985 on NES platforms.
"Bomberman is a robot engaged in the production of bombs. Like his fellow robots, he had been put to work in an underground compound by evil forces. Bomberman found it to be an unbearably dreary existence. One day, he heard an encouraging rumor. According to the rumor, any robot that could escape the underground compound and make it to the surface could become human. Bomberman leaped at the opportunity, but escape proved to be no small task. Alerted to Bomberman's betrayal, large numbers of the enemy set out in pursuit. Bomberman can rely only on bombs of his own production for his defense. Will he ever make it up to the surface? Once there, will he really become human?"

## Stats

* uses an aspect ration of 4:3 for NES

## Controls

* D-Pad: Move Bomberman.
* A Button: Lays down a Bomb.
* B Button: Detonate Remote Bomb.
* Start Button: Pause the game.
* Select Button: Toggle menu options.

## GamePlay

The player must navigate Bomberman through a maze by destroying Soft Blocks and enemies with his bombs. Hidden in every stage is one Power-Up that will either increase his blast radius, speed, or the number of bombs he can lay at one time, among other things. Each stage also features 1 secret item that can be uncovered under special conditions. To clear a stage, the player must find the exit that is hidden under one of the Soft Blocks and defeat all the enemies in order to open it. If a bomb blast touches the exit after it is exposed, more enemies will be generated out of it. There are 50 levels in total, and a password system can be used to continue the game. 

## Enemies

* Balloom -> (p:100, s:2, sm:1, a:01) moves slow and random, it takes 1 hit to be destroyed. Associated with the Bomb-Up power-up
* Oneal   -> (p:200, s:3, sm:2, a:02) moves quickly and random, it takes 1 hit to be destroyed. Associated with the Fire-up power-up
* Dall    -> (p:400, s:3, sm:1, a:03) moves quickly and random, it takes 1 hit to be destroyed. Associated with the Speed-up power-up
* Minvo   -> (p:800, s:4, sm:2, a:03) moves fast and will persue bomberman if nearby, it takes 1 hit to be destroyed. Associated with the Soft Block Pass power-up

not required:
* Doria
* Ovape
* Pass
* Pontan

# Engine
## GameObject
The GameObject is heavily component based.
Each type of component can only exist ones. so one sprite component and/or one text component even though the text component inherits from the spritecomponent.
Upon Creating a new GameObject a TransformComponent is automatically created in the constructor of the GameObject. and the GameObject has a GetTransform() Method for easy and fast access.
To Create Controller Input Bindings, the referenced gameobject must have a ControllerComponent specifing the controller index it will listen to.

## NamingConventions
I use a lot of Vulkan terminology:
* CreateInfo -> when I am loading resources
* Setting(s) -> mainly used to set engine settings
* Descriptor -> used to feed into components

* Manager -> Contains and owns a lot of data (like ResourceManager)
* System -> Focus on implementation/functionality not data (like InputSystem), can still contain/own some data

## Engine Structure
### includes
<> => indicates public headers
"" => indicates private headers
relative paths are avoided in includes, unless the private header is in the same folder as the current private file
"" does not necessarily mean that the path is relative, but it does mean that this header is private!
<> only refers to public headers, and will use absolute paths exclusivly.

### CollisionSystem and PhysicsSystem
a gameObject can have a collision component which will automatically subscribe it to the collision system
the collision system will check for collisions between gameobjects with a collision component and will call the OnCollisionEnter function of the gameobjects when a collision is detected.

when a gameobject has a rigidbody component, it will automatically subscribe to the physics system.
the physics system will use the collision component bounds to detect any phisical collisions.
it will only take the bounds that have not been marked as trigger, and will ignore any collision component that is marked as trigger.

### InputSystem
the input system has a list of input bindings, which are created by the user.
inputbindings can contain multiple inputActions, and can be triggered by any of the inputs in the binding.
inputbindings also take commands and events that will be executed and Broadcasted when the binding is triggered.

### Ids
gameobjects, scenes, events... are all identified by an Id, which is a simple wrapper around an unsigned int.
gameobjects do not need to have a unique id, it functions more like a tag for convienince reasons.
Resources to be followed. They are still identified by a string (for now)
it allows for easy lookup access. for example you could request all gameobjects with a certain id.

### ResourceManager
resources are identified by a string, and stored in a map with the string as the key and the resource as the value.
the game has no knowledge of the resources, it only knows the string identifier, and the resource manager will handle the loading and unloading of the resources.
the game can only access the resource manager through an Interface and has very limited access to it.