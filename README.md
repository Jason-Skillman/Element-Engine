# Element-Engine
An OpenGL game engine written in C++ for games.

## Getting started
Requierments:
- Windows
- Visual Studio 2019 or later
- Python https://www.microsoft.com/en-us/p/python-39/9p7qfqmjrfp7

Steps:
1. Run `scripts/Setup.bat` to install and setup required packages.
1. Run `scripts/Premake.bat` to setup the Visual Studio project files.
1. Open the Visual Studio project `Element-Engine.sln`.

### Command line arguments
Optional command line arguments can be passed into the application to open a scene on startup.
1. Right click the `ElementEditor` project in Visual Studio and click on `Properties`.
1. Navigate to `Debugging | Command arguments`.
1. Input address to a scene file. Eg. `"D:\Workspaces\Git\Element-Engine\ElementEditor\Assets\Scenes\RGBSquares.scene"`

## Sources

### 3rd party libraries
List of all third party libraries and packages used in this project and where to find them. This project uses all forked links instead of the original link.
- `spdlog` https://github.com/gabime/spdlog
- `premake` https://github.com/premake/premake-core
- `GLFW` https://github.com/glfw/glfw
  - Fork: https://github.com/TheCherno/GLFW
- `Glad` https://glad.dav1d.de/
- `ImGui` https://github.com/ocornut/imgui
  - Fork: https://github.com/TheCherno/imgui
- `glm` https://github.com/g-truc/glm
- `stb` https://github.com/nothings/stb
- `entt` https://github.com/skypjack/entt
- `yaml-cpp` https://github.com/jbeder/yaml-cpp
  - Fork: https://github.com/TheCherno/yaml-cpp
- `ImGuizmo` https://github.com/CedricGuillemet/ImGuizmo 
  - Fork: https://github.com/TheCherno/ImGuizmo
- `VulkanSDK` https://vulkan.lunarg.com/sdk/home
- `Box2D` https://github.com/erincatto/box2d
  - Fork: https://github.com/Jason-Skillman/box2d

### Original source
This project is based off of `The Cherno`'s Game Engine series.

YouTube: https://youtube.com/playlist?list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT

GitHub: https://github.com/TheCherno/Hazel
