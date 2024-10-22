# Interactive Japanese Environment using OpenGL

This project is an interactive 3D environment that simulates a dynamic Japanese setting using OpenGL. The environment is designed to provide an immersive experience where users can navigate and interact with traditional elements such as cherry blossoms, a mojo house, and various environmental effects.

## Table of Contents
- [Features](#features)
- [User Interaction Instructions](#user-interaction-instructions)
- [Technical Implementation](#technical-implementation)
- [Resources Used](#resources-used)
- [Challenges Faced](#challenges-faced)
- [Contributors](#contributors)

## Features

- **3D Environment Navigation**: Explore the Japanese setting using the keyboard to move around in a boat.
- **Background Changes**: Press the "B" key to change the background (day, night, weather conditions).
- **Lighting Control**: Use the up/down arrow keys to adjust the ambient lighting for different atmospheres.
- **Theme Switching**: Press the "T" key to switch between various environment themes.
- **Ghost Mode**: Press "G" to enter ghost mode, removing physical constraints for free exploration.
- **3D Models and Animation**: Each element, such as the cherry blossoms and mojo house, is represented as a 3D model with added animations, such as moving water and floating blossoms.

## User Interaction Instructions

- **Navigation**:
  - `W` to move forward.
  - `S` to move backward.
  - `A` to move left.
  - `D` to move right.
- **Lighting Control**: Adjust ambient lighting with the up and down arrow keys.
- **Theme Switching**: Press the "T" key to change environmental themes.
- **Ghost Mode**: Activate ghost mode with the "G" key.
- **Exit**: Press `Esc` to exit the program.

## Technical Implementation

- **Dome Modeling**: A 360-degree dome with textures is used to create an immersive background.
- **Lighting**: The Phong lighting model was used to calculate ambient, diffuse, and specular lighting for dynamic and realistic lighting effects.
- **3D Models**: Traditional elements like cherry blossoms and the mojo house were created using mesh representations and texture mapping.
- **Animations**: Subtle animations such as water movement and floating blossoms were created using sine and cosine functions.
- **Shaders**: Implemented multiple shaders including:
  - Bulb Shader for realistic lighting effects.
  - Phong Shader for dynamic lighting calculations.
  - Dialog Shader for rendering instructions.

## Resources Used

- **3D Models**: Free, pre-made models from [TurboSquid](https://www.turbosquid.com), [CGTrader](https://www.cgtrader.com), and [Sketchfab](https://sketchfab.com) were customized for this project.
- **External Libraries**: OpenGL libraries similar to those used in class.
- **Tutorials and Blogs**:
  - [LearnOpenGL](https://learnopengl.com/)
  - [OpenGL Step by Step](https://ogldev.org/)

## Challenges Faced

- **Environment Mapping**: Initially attempted to use cubemaps but switched to dome mapping for better quality.
- **External Libraries**: Faced compatibility issues when integrating external libraries, which led to a decision to reduce their use.
- **Performance Optimization**: Balancing visual quality with performance was a challenge, requiring optimizations in rendering complex models and textures.
