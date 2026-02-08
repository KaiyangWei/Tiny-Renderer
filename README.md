# Tiny Software Renderer (C++)

This is my tiny software renderer project, finished in about four days.  
I am new to graphics programming, but I tried to understand every step by writing comments and linking code to linear algebra.  
I hope every readers can give me some suggestions, thank you.

## Features
- Model/View/Projection pipeline
- Rasterization + Z-buffer
- Perspective-correct interpolation (`1/w`)
- Texture sampling (TGA)
- Lambert lighting

## Build & Run
- Open the `.sln` in Visual Studio
- Build `x64/Debug` or `x64/Release`
- Run and get `output.tga`

## Notes (My Background)
- I am a sophomore math major.
- I used to struggle with C and pointers, so I wrote many comments to keep track of memory and data flow.
- This project is my way to rebuild confidence and understand low-level graphics.
- I focused on vector/matrix math and avoided using high-level graphics libraries.

## AI Usage
- I used AI as an assistant for about **20%¨C30%** of the work.
- Mainly for OBJ parsing details and some explanations, but the core pipeline is written by myself.

## Current Question
- My main confusion is still about how `1/w` works in perspective-correct interpolation.

## Assets
- `african_head.obj`
- `african_head_diffuse.tga`

> Please ensure assets are placed in the working directory.  
> Thanks to every reader for your time and suggestions.

## Repository Layout
- `README.md`, `.gitignore`, `.sln` are in the repo root.
- Source code is in `Tiny Renderer/` (like `main.cpp`, `geometry.h`, `our_gl.*`, `model.*`).
- Assets can be placed in `assets/` (or just the working directory for now).
- Output file: `output.tga` (can be ignored if needed).