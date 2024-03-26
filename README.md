## <p align="center">**OVERVIEW**</p>

this is a simple 2D shooter game made in C++ using SFML graphics library and ECS(Entity, Components, Systems) design pattern <br>

**objective**<br>
the objective of the game is to get as many points as possible without dying

## <p align="center">**INSTALLATION**</p>
clone the repository to your local machine<br>
```bash
git clone https://github.com/MortuoRege/simple-2d-shooter-game-.git
```
Open the project in Visual Studio and build it. If you're using a different IDE or compiler, ensure it supports C++ and SFML.<br>
### **Linux Users:**
Before building the project, you need to install SFML. Use the following commands based on your distribution:<br>
For Arch Linux users:<br>
```bash
sudo pacman -S sfml
```
For Ubuntu users: <br>
```bash
sudo apt install libsfml-dev
```
Navigate to the cloned directory and compile the project:<br>
```bash
g++ -I include -L lib -o Game Game_engine/*.cpp -lsfml-graphics -lsfml-window -lsfml-system
```
This command compiles the project using g++, linking the necessary SFML libraries.<br>
Finally, run the executable file:
```bash
./Game
```
Ensure you're in the correct directory containing the compiled executable.

## <p align="center">**CONTROLS**</p>
W,A,S,D: Move character<br>
P: pause the game<br>
left mouse click: shoot bullets<br>
Spacebar: shoot special weapon<br>
## <p align="center">**GAMEPLAY**</p>

The gameplay features a player character and enemies that spawn randomly at regular intervals, typically every 100 frames. 

- The player earns 100 points for each enemy killed.
- If an enemy touches the player, the player character dies and respawns in the center of the screen. Upon respawn, the player's score resets to 0.
- Additionally, the player has access to a special weapon with a unique ability, such as a gravity bullet that travels at the center and attracts every enemy to it, so that the player can kill them easily.


