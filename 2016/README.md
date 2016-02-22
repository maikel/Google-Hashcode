## Installation and Usage

You need C++11 and CMake. To compile just type

    $ cmake .
    $ make

The standard settings have assertions enabled and such it is to slow for the big files.
Change in the `CMakeLists.txt` the line with

    SET(CMAKE_BUILD_TYPE Debug)

to

    SET(CMAKE_BUILD_TYPE Release)

to turn off assertions.

The project will build a library called `lib/libhashcode-2016.a` and some binaries in the `bin/` folder.
The binary `bin/simulation` performs a simulation and needs a file name as argument.
It will attempt to read an input file described in the problem description (see deliver.pdf).
It also expects to get a command list via standard input also in the format of the problem description.

The files `example.in` and `example.out` are from the given PDF and can be used to test a simluation.
Use it like

    $ cat example.out | ./bin/simulation example.in
  
The output should be something like this:

```
Reading input file 'example.in' ... done
Map Dimension: (100, 100)
Number of Drones: 3
Maximum Load Capacity of Drones: 500
Max Steps of Simulation: 50
Number of Prodcuts: 3
Number of Warehouses: 2
Number of Orders: 3
time step t = 0
Drone 0 recieves a new command 0 L 0 0 1
Drone 1 recieves a new command 1 L 1 2 1
Drone 1 has to fly from grid cell (0,0) to the grid cell (5,5) and will need 8 turns to do so.
Drone 0 performs its assinged command: 0 L 0 0 1
time step t = 1

[ ... ]

time step t = 49
Score: 194
```

To create a command list from an problem use a Strategy like simple_strategy.

    ./bin/simple_strategy example.in

And the output should be something like

```
12
0 L 0 0 1
0 D 0 0 1
0 L 1 2 1
0 D 0 2 1
1 L 0 0 1
1 D 1 0 1
1 L 0 0 1
1 D 1 0 1
1 L 0 0 1
1 D 1 0 1
2 L 1 2 1
2 D 2 2 1
```

To use a strategy generator and a simulation together you can pipe them like. This will result here to

    $ ./bin/simple_strategy example.in | ./bin/simulate example.in 

```
Reading input file 'example.in' ... done
Map Dimension: (100, 100)
Number of Drones: 3
Maximum Load Capacity of Drones: 500
Max Steps of Simulation: 50
Number of Prodcuts: 3
Number of Warehouses: 2
Number of Orders: 3
time step t = 0
Drone 0 recieves a new command 0 L 0 0 1
Drone 1 recieves a new command 1 L 0 0 1
Drone 2 recieves a new command 2 L 1 2 1
Drone 2 has to fly from grid cell (0,0) to the grid cell (5,5) and will need 8 turns to do so.
Drone 0 performs its assinged command: 0 L 0 0 1
Drone 1 performs its assinged command: 1 L 0 0 1
time step t = 1

[ ... ]

time step t = 49
Score: 186
```
