To use this program, first ensure all of the files are in the same directory and have been compiled. To do this, simply type "make" in the command line while in the same location as all of the files.
Then, to run the program, simply enter ./simulation [input_file.txt] [random_number_generator_seed]
simulation is the name of the file that contains the main logic, and thus has the main file. A sample input filed called "sample.txt" has been provided. replace [input_file.txt] with the name of the input file you want to use and replace [random_number_generator_seed] with an integer to control the random environment.
When it editing the input file, make sure that all parameters that start with prob or proportion are floats and everything else is an integer. 
To advance each "frame" or tick, simply press enter while in the command terminal.
The Animator class was provided to us, and handles everything the client sees on screen. 
The VehicleBase class was also provided, however was modified in the process of getting the program to the state that it is currently in. 
The Ingest class was created to take in all of the date while withstanding any modification that has been done to the text document within the scope described in this project. It then assigns these points of data to variables that are accessible via getter methods.

The design of his project was quite turbulent, but we think we were able to develope a concrete solution to the issues presented. 
Firstly, we developed a way to take in the data given to us in the input file. That lead to the development of the ingest class.
The idea behind the ingest class was simply to develope some easily accessible getter methods to be able to quickly use those numbers in our main logic and in our randomizer class.
The way the Ingest class works is by using the <fsteam> class built in to c++. It uses fstream to open the file, and check to make sure that the file was opened correctly.
Then, file is parsed, using the name of the variable in the text document to determine what number to assign to which variable in the code. The Ingest class is programmed to withstand variations in the order of the variables and white spaces being placed between the variable name and the number.
After all of the variables recieve their assignments, they Ingest class has getter methods for all of them in order for quick access to the specific number when needed.
At one point, both a Randomizer class and a Section class were created in order to faciliate generation and movement of the vehicles respectively.
However, due to bugs or due to a lack of time to solve those bugs, those classes were scrapped. Most of the features present in the Randomizer class were ported directly into the main file itself. 
The logic for those calculations uses the range of numbers between zero and one (in the real number space) as spaces to model percentages. For example, a right turn with a chance of 30% and a left turn with a 30% chance would be represented by the random number hitting between 0 (inclusive) and 0.30 (exclusive),
while the left turn would be represented by the random number hitting in the range between 0.30 (inclusive) and 0.60 (exclusive). This technique is used for spawning a type of vehicle, 

The main part of the simulation is made up of a few key parts, the spawner, the move logic, and the light logic. Starting with the former, the spawner uses the aformentioned calculations to determine where the next vehicle will be spawned, what its type will be, and what it is doing at the intersection.
It assigns these values to variables present within the VehicleBase itself, which are then accessible via getter methods. Once, a the correct vehicle has been constructed with the correct specifications, it gets placed into one of four maps. These maps, along with the dummy maps the coexist along side of them are the backbone of this logic.
The maps are used to keep track of each vehicle on each of the four lanes, and dummy maps are used to advance the positions without messing up the original maps. These dummy maps can then be reset without the program losing their references to the existing vehicles. We then use those references in the main maps to draw the vehicles.
A decision was made to use this method over a method that involved a section class, as difficulites arose when trying to determine how to program the vehicles on the road to perform a right turn. After much debate, despite the section class having been already written, the program was switched to this map based design to cater to the challlenges presented when turning.
To move the Vehicles, each map is parsed through using the references maintained by the maps. The vehicles, whose head is stored as the key of the VehicleBase in the map, have their locations pulled and area infront of them is checked for a few different things.
If it is determined that the section that would be occupied if the vehicles moved is a spawn or despawn tile, the approporiate logic is used to handle the erasing or adding of one "body" of the vehicle. A modified VehicleBase helps with this, as each vehicle knows how long it is from the head once it is fully spawned.
Collision are checked by using the maps to compare different vehicle references and where they are located. Plus, four sectionOccupied arrays help keep track of which sections are free.
The lights are fairly simple. By using a "clock" (counting every tick of the sim), the program uses variables to keep track of the time remaining and subtracts one for every tick. When these variables hit 0, the lights are switched and timers used for telling vehicles if it could move are reset. 
 
We decided on this approach because we believe having the maps made it easier keep track of the vehicles on the simulation. It also means that we have references to the vehicles when they get erased when the VehicleBase vectors are reset.
The decision to use a sectionOccupied array was born out of the ashes of the unused section class. Since we had experience with this way of keeping track of what places were occupied, it was easiest. The decision move the ingest logic to another class was done in order to make the code a little cleaner. If we had more time, the Randomizer class would be revised and implemented in a way that would allow it to not take up so much space in the main.
Most of the logic in the Ingest class is done for ease of use. We thought it would be easier to just have a getter method for each of the parameters instead of using another map. Again, if we had more time the Ingest class could be cleaned up to be more efficient. 
The light logic was the simplest way we thought that we could implement the functionality, henceforth we implemented the way that we did.
The modifications to VehicleBase were done in order to make it easier for us to spawn in, as we then know how large the vehicle needs to be while spawning. It makes it easier for the implementation of the maps.
Dummy maps were also made because it was the best way we could think of to move everything. It allows us to keep track of positions and vehicles we need to in order to move cars every tick.
As a final thought, the use of maps was chosen out of a number of different implementations because we thought it would the easiest to logically compute how we need to move everything in our brains. If given more time, we think a different implementation using a stack or a working section class or a comibination of the two could be a better way to implement the solution.

For testing, we both played around with the parameters inputted from the input file and manually spawned vehicles. We played around with the size of the intersections, making them small and large. This is where we found the problem where under short lengths certain vehicles would not work correctly 100% of the time.
We made the simulation last 1000s of ticks, we made the program spawn only one type of vehicles and/or only in one lane. The length of lights were tested from 0 to 50. This is how we discovered the issue with lanes not moving if the light does not change.
For the ingest, the sample file shows how the data was messed with in order to make sure the Ingest class works correctly. Mainly, the whitespace and orders were changed. 
Tests were run to make sure that the copy constructor and move constructor work as intended. Copies of vehicles were made and a copy using std::move to make sure the move constructor and assigmnet work.
The frequency of spawning was increased to spawn a vehcile every tick in order to make sure that program checks that there is room to spawn the vehicle.
Many different seeds were used in order to ensure that the code worked in many different situations. 

Overall, while this was a long program to code that took a lot of time, we are pretty satisfied with our program and have learned alot!

Known problems:
1). At the end of the program's execution, a double free error is thrown. We think this is caused by the maps being deleted twice, once when it goes out of scope and again when the program ends.
2). For halfsize = 1,2 and 3: trucks do not work correctly and for halfsize = 1 suvs do not work correctly some of the time. We think that it's because we hard code the cases where we spawn it, trucks must go in
by 3 sections and suv must take 2 sections in order for it to go properly into the lanes and go into the formal loop.
3). If the yellow light timer is set to 0, the lights do not change. This causes the vehicles in all lanes to approach the intersection, but not go through it. 
