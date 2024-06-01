# Lightweight Game Engine
This project is designed to be a minimalistic and highly efficient game engine with a comprehensive Lua API for scripting your games. As it is in the early stages of development, expect rapid changes and improvements.

### Table of Contents
* **[Introduction](#Introduction)**<br>
* **[Features](#Features)**<br>
* **[Installation](#Installation)**<br>
* **[Building](#Building)**<br>
* **[Usage](#Usage)**<br>
* **[Documentation](#Documentation)**<br>
* **[License](#License)**<br>

# Introduction
Our Game Engine is a project aimed at providing developers with a simple yet powerful game development tool. The engine is designed to be extremely lightweight, making it suitable for various types of games, from simple 2D games to more complex high-definition 3d projects.

# Features
* **Lightweight and Efficient:** Designed to minimize overhead and maximize performance.
* **Comprehensive Lua API:** Full Lua scripting support for game logic and customization.
* **Modular Design:** Easy to extend and customize with additional modules.
* **Upcoming Features:**
* **Map Editor:** A user-friendly interface for creating and editing game maps.
* **Script Editor UI:** An integrated environment for writing and debugging Lua scripts.

# Installation
`git clone https://github.com/yourusername/lightweight-game-engine.git`

# Building
* **Only supported by MSVC at the moment!**

# Usage
Once you have installed the engine and successfully built it, you can start creating your game. Here’s a quick example of how to set up a basic game using the [Lua API](https://edwn.gitbook.io/documentation/):
```lua
Graphics.SetClearColor(Color.new(0, 0, 0, 255)) --Demo made by @Splatzy

local DefaultFont = Renderer.CreateFont( "Segoe UI", 16, 400, Vector2.new( 0, 0 ), false)

local ASCII = [[
    ,'\
    _.----.        ____         ,'  _\   ___    ___     ____
_,-'       `.     |    |  /`.   \,-'    |   \  /   |   |    \  |`.
\      __    \    '-.  | /   `.  ___    |    \/    |   '-.   \ |  |
 \.    \ \   |  __  |  |/    ,','_  `.  |          | __  |    \|  |
   \    \/   /,' _`.|      ,' / / / /   |          ,' _`.|     |  |
    \     ,-'/  /   \    ,'   | \/ / ,`.|         /  /   \  |     |
     \    \ |   \_/  |   `-.  \    `'  /|  |    ||   \_/  | |\    |
      \    \ \      /       `-.`.___,-' |  |\  /| \      /  | |   |
       \    \ `.__,'|  |`-._    `|      |__| \/ |  `.__,'|  | |   |
        \_.-'       |__|    `-._ |              '-.|     '-.| |   |
                                `'                            '-._|
]]
    
function Interface ()   
	Renderer.Text(DefaultFont, ASCII, Vector2.new(5, 5), Color.new(204, 53, 53,255))
end

AddCallback("OnInterfacePaint", Interface)
```

# Documentation
The documentation is currently under construction, it may have some undocumented features.
[Documentation/Wiki](https://edwn.gitbook.io/documentation/)