<p align="center">
	<h1 align="center">Amaretto</h2>
	<p align="center">A multi platform C++ terminal I/O library</p>
</p>
<p align="center">
	<a href="./LICENSE">
		<img alt="License" src="https://img.shields.io/badge/license-GPL-blue?color=7aca00"/>
	</a>
	<a href="https://github.com/LordOfTrident/amaretto/issues">
		<img alt="Issues" src="https://img.shields.io/github/issues/LordOfTrident/amaretto?color=0088ff"/>
	</a>
	<a href="https://github.com/LordOfTrident/amaretto/pulls">
		<img alt="GitHub pull requests" src="https://img.shields.io/github/issues-pr/LordOfTrident/amaretto?color=0088ff"/>
	</a>
	<br><br><br>
</p>

## Table of contents
* [Introduction](#introduction)
* [Features](#features)
* [Bugs](#bugs)
* [Documentation](#documentation)
* [Dependencies](#dependencies)
* [Make](#make)

## Introduction
A [C++](https://en.wikipedia.org/wiki/C%2B%2B) [terminal](https://en.wikipedia.org/wiki/Terminal_emulator) [input/output](https://en.wikipedia.org/wiki/Input/output)
library for [Linux](https://en.wikipedia.org/wiki/Linux) and [Windows](https://en.wikipedia.org/wiki/Microsoft_Windows).

## Features
- [X] Basic colored output
- [X] Keyboard input
- [X] Window title changing
- [X] Cursor positioning, hiding/showing
- [x] Rectangle drawing, filling functions
- [X] Line drawing functions
- [X] String outputting functions
- [X] Pattern drawing functions
- [X] Border drawing functions
- [X] Color-only/character-only drawing functions
- [ ] Special characters outputting
- [ ] Mouse events

## Bugs
If you find any bugs, please create an issue and report them.

## Documentation
Coming soon.

## Dependencies
### Linux
- [ncurses](https://en.wikipedia.org/wiki/Ncurses)

## Make
Run `make all` to see all the make rules.
<br><br>
- Use `make static` to compile a static library and `make shared` to compile a dynamic library
- Dont forget to `make clean` when you want to recompile the library
- Use `make install` (on Linux) to install the library
- Use `make tests` to compile the test programs
