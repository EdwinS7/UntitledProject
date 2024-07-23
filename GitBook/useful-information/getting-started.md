---
description: Guide to writing lua scripts for Implicity
---

# 📲 Getting started

### Using lua scripts

1. Save the Lua script in a sub folder to unknown.exe. The file name should be test.lua
2. Go to the "Lua Scripting" tab and select the script you want to load and click "Load". If you want it to automatically be loaded whenever you load the cheat, tick "Load on startup"

### Text editor

The best text editors make it simple and easy to code without formatting issues corrupting it.

| Text editor                      | Link                                                    |
| -------------------------------- | ------------------------------------------------------- |
| :first\_place:Visual studio code | [code.visualstudio.com](https://code.visualstudio.com/) |
| :second\_place:Sublime Text      | [sublimetext.com](https://www.sublimetext.com/)         |
| :third\_place:Notepad++          | [notepad-plus-plus.org](https://notepad-plus-plus.org/) |

{% hint style="info" %}
**the API automatically includes the base, string, math, table, debug, package libraries!**
{% endhint %}

{% hint style="warning" %}
You should keep most of your scripts functions and variables as local to avoid collision with other scripts. Global variables should only be used to write libraries or scripts that interact with each other.
{% endhint %}

### :beginner:Beginner's guide

If you're new to Lua scripting, we'd recommend to take a look at the «[Lua in 5 minutes](https://learnxinyminutes.com/docs/lua/)» guide
