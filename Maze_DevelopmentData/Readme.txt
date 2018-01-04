I found that I need to use:

Maze5x5 - for single stepping code and debugging
Maze500x500 - was more useful as a mini stress test
      I placed counters printing to help me see if I had infinite loops
      Example:  printf("%d\r",count++);
Maze1Kx1K or greater performed too poorly in Debug mode
Suggest only using Release mode

Microsoft IDE - boggs the whole application greatly
For final submission and performance tuning - use "Start Without Debugging" option - <Cntrl> F5

-Good luck