# Intro
The Prey-Predator relations are those that describe a natural behavior between distinct groups in a biological system. One popular mathematical representation of it is the Lotka-Volterra model which is the ODE system. This project is a similar driven model with its own assumptions and logic.
# World
The simulation takes place at the left-hand side of the window in a toroidal square grid of cells. Each cell can contain a rabbit, wolf, sun, or nothing. There are two different time periods: days and nights. During the day the sun shines on the world and leaves some pattern of sun units on the map. During the night the sun is gone.
Rabbits feed on the sun units. If a rabbit sees an adjacent cell with more sun units than his current cell, he will move there. If there are only empty cells around the rabbit, he will move randomly. If the rabbit already sits on the cell with some sun units, but there are no better cells around it, he can still move spontaneously by means of his variance.
Wolves feed on the rabbits. If the wolf sees an adjacent cell that has a rabbit, he will move there. Wolves can not move to a cell where the sun shines, moreover, when the day starts and the wolf happens to be in a cell where the sun shines, he dies. If there are no rabbits around, the wolf moves randomly.
# Model logic
Rabbits and wolves have the same maximum amount of health points, which descend linearly with time. Animals give birth if there is a free space around them and their health is larger than the birth threshold. Both have the same birth threshold around 92% of max hp.
Rabbits can only feed on the sun. If a rabbit sits on the sun pot it gains the amount of sun units added to his health each frame. Rabbits can die from sun scarcity or by being eaten by wolves.
Wolves can only feed on rabbits. When a wolf eats a rabbit it gains a fixed amount of health points.
When the day starts, some amount of parent sun seeds are randomly scattered across the map. Each seed then fills the area 3x3 around itself with some amount of sun units bounded by maxshine and also spawns a child seed with some chance to a neighboring cell. The process continues for all seeds.
# Parameters
###### The right-hand side of the window displays the current parameters.
- Time is the amount of ms elapsed since the beginning of a simulation
- Next Day/Night is the moment when the next day/night starts
- Day/Night duration
###### This section also has an intuitive clock that changes colours depending on the day/night. Its arrow always winds exactly once in the entire day/night duration.
- Mapsize is the amount of cells in the width & height of the grid
- Expected is the mathematically expected value of sun units
- Actual is the total value of sun units that are present on the map during the day
- Degree is the mark of sun intensity colour. Colours of the cells that contain sun are picked from a gradient of 40 colours. Basically, it means that the hottest cell has 40*degree units (raspberry red colour) and each change of colours is degree amount of sun units
- Rabbits current population
- Wolves current population
###### To the left there is a small bar plot comparing the expected value to the actual value.
- Speed is the speed of the animation in ms, it means that the world updates each speed amount of ms. The lower the amount, the faster it goes
- Seeds is the amount of parent seeds from which the sun starts spreading;
- Chance is the chance that the seed will also spawn a child
- Maxshine is the maximum value of sun units that can be added at once to a cell
###### There is a gif of a running rabbit that shall give the user a reference to how fast the simulation goes.
# Keys
###### The model is flexible and the parameters can be adjusted on the go. Press I to see the instructions for keys to use. 
- S W R hides/shows the appearance of sun/wolves/rabbits
- O hides/shows the phase plot
- P hides/shows the time plot
- Enter/leave adjust the mode by pressing Left Shift (a yellow box appears upon entering)
- Restart the model by Right Shift (while being in adjust mode)
###### Plots serve as an output to analyze the model behavior.
# Run
Download the files and open the repository containing makefile in terminal.
Simply run make to compile files and create an executable. 
```
make
```
After this the executable file Prey-Predator appears in your repository. You can either run it from terminal or by clicking on it.
```
./Prey-Predator
```
See the video for an example [Prey-Predator](https://youtu.be/lL30megBv20)