To compile these files run: "make all" in your terminal.

Serial implementation:
    For the serial implementation input should be as follows:
    ./serial_conway <number of rows> <number of columns> <number of timesteps>
    The number of rows and columns will determine the size of the 2D grid and the number of timesteps/generations of life cycles of the cells
    e.g. "./serial_conway 3 3 5", which will correspond to 3 rows 3 columns and 5 life cycles.

Parallel implementation:
    For the serial implementation input should be as follows:
    "mpirun -np <number of processes> ./parallel_conway <number of rows> <number of columns> <number of timesteps>"
    The number of rows and columns will determine the size of the 2D grid and the number of timesteps/generations of life cycles of the cells
    e.g. "mpirun -np 4 ./parallel_conway 3 3 5", which will correspond to 3 rows 3 columns and 5 life cycles using 4 processes.

Output:
    The code will then display all the generations generated at each iteration.

Sample I/O:
    - input: "./serial_conway 3 3 5"
    - output: 
        First Generation: 
        1 0 1 
        1 1 1 
        0 0 1 
        Generation 1: 
        1 0 1 
        1 0 1 
        0 0 1 
        Generation 2: 
        0 0 0 
        0 0 1 
        0 1 0 
        Generation 3: 
        0 0 0 
        0 0 0 
        0 0 0 
        Generation 4: 
        0 0 0 
        0 0 0 
        0 0 0 
        Generation 5: 
        0 0 0 
        0 0 0 
        0 0 0