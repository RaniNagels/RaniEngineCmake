# Bomberman 

[![Build Status](https://github.com/avadae/minigin/actions/workflows/cmake.yml/badge.svg)](https://github.com/avadae/cmake/actions)
[![Build Status](https://github.com/avadae/minigin/actions/workflows/emscripten.yml/badge.svg)](https://github.com/avadae/emscripten/actions)
[![GitHub Release](https://img.shields.io/github/v/release/avadae/minigin?logo=github&sort=semver)](https://github.com/avadae/minigin/releases/latest)

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
