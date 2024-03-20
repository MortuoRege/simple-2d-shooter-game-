## <p align="center">**OVERVIEW**</p>

this is a simple 2D shooter game made in C++ using SFML graphics library and ECS(Entity, Components, Systems) design pattern <br>

**objective**<br>
the objective of the game is to get as many points as possible without dying

## <p align="center">**INSTALLATION**</p>
clone repository to your local machine using<br> git clone https://github.com/MortuoRege/simple-2d-shooter-game-.git<br> Open the project in Visual Studio Build the project, run the executable file generated,
for Linux users, you have to generate a CMake file 
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


