# Self-Evaluation

## Name(s): 

Out of 20 points. Use output.txt created using 
`./create-output.sh > output.txt 2>&1` for guidance.

Complete all questions with "Q:"

Q: Does the program compile and run to completion: Yes

- If the program does not compile or gives a segmentation error when run, 
the maximum possible grade is 50%. No need to continue with the rest of self-evaluation

Q: All public functions have been implemented: 0

- -1 for each public function not implemented
- Constructors:  Graph()
- Destructor: ~Graph()
- Helpers: contains, verticesSize, edgesSize, vertexDegree, getEdgesAsString
- Modifiers: add, connect, disconnect, readFile
- Algorithms: dfs, bfs, dijkstra, mstPrim

Q: Bonus function - mstKruskal: 0

- +1 if Minimum Spanning Tree using Kruskal's algorithm has been implemented

Q: -1 for each compilation warning, min -3: 0

- Check under *1. Compiles without warnings*
- If the warning message is addressed in README.md, including how the programmer tried to address it, no deductions

Q: -1 for each clang-tidy warning, min -3: 0

- Check under *3. clang-tidy warnings*
- If the warning message is addressed in README.md, including how the programmer tried to address it, no deductions

Q: -1 for each clang-format warning, min -3: 0

- Check under *4. clang-format does not find any formatting issues*

Q: Runs and produces correct output: 0

- Try running "./a.out". assert statement should test the code

Q: -2 for any detected memory leak: 0

- Check under *5. No memory leaks using g++*
- Check under *6. No memory leaks using valgrind*

Q: Are all functions in .h and .cpp file documented (min -3): 0

- You need at least 1-line of comments
- -1 for each function not documented

Q: Total points: 20 / 20