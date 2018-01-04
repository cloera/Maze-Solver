#include <stdio.h>
#include <assert.h>

#include "Timer.h"
#include "STMazeSolverDFS.h"
#include "STMazeSolverBFS.h"
#include "DFSTop.h"
#include "DFSBottom.h"
#include "StudentSolveBuilder.h"

#define INPUT_NAME_SIZE 64

// change to "1" for the final submission
#define FINAL_SUBMIT 1


int main(int argc, char *argv[]) 
{

#if !FINAL_SUBMIT
	char inFileName[INPUT_NAME_SIZE] = "Maze10Kx10K.data";
#else
	char inFileName[INPUT_NAME_SIZE];
	memset(inFileName,0x0,INPUT_NAME_SIZE);
	strcpy_s( inFileName, INPUT_NAME_SIZE, argv[1]);
#endif

	printf("Maze: start(%s) ------------\n",inFileName);

	// Initialize Timer
	timer::initTimer();

	// Create timers 
	timer ST_DFS_Timer;
	timer ST_BFS_Timer;
	timer MT_Student_Timer;
	 
	// Create a maze
	Maze *pMaze;
	
	pMaze = new Maze();

	// Load it
	pMaze->Load(inFileName);

	// -- STMazeSolverBFS -----------------------------------------------------------
	printf("\n Maze: STMazeSolverBFS\n");

		// Create and load Maze
		// Only changes allowed converting to atomic int, if desired
			pMaze = new Maze();
			pMaze->Load(inFileName);

		// Solve it
		ST_BFS_Timer.tic(); 

			STMazeSolverBFS stSolverBFS(pMaze);
			std::list<Direction> *pSolutionBFS = stSolverBFS.Solve();
		
		ST_BFS_Timer.toc();

		// Verify solution - do not delete, it is not timed
		pMaze->checkSolution( pSolutionBFS );

		// release memory
		delete pSolutionBFS;
		delete pMaze;

	// -- STMazeSolverDFS -----------------------------------------------------------
	printf("\n Maze: STMazeSolverDFS\n");

		// Create and load Maze
		// Only changes allowed converting to atomic int, if desired
			pMaze = new Maze();
			pMaze->Load(inFileName);

		// Solve it
		ST_DFS_Timer.tic(); 

			STMazeSolverDFS stSolverDFS(pMaze);
			std::list<Direction> *pSolutionDFS = stSolverDFS.Solve();
		
		ST_DFS_Timer.toc();

		// Verify solution - do not delete, it is not timed
		pMaze->checkSolution( pSolutionDFS );

		// release memory
		delete pSolutionDFS;
		delete pMaze;

	// -- Multi-Threaded Student Solver ----------------------------------------------
	printf("\n Maze: MTStudentSolver\n");

		// Create and load Maze
		// Only changes allowed converting to atomic int, if desired
			pMaze = new Maze();
			pMaze->Load(inFileName);

		// Solve it
		MT_Student_Timer.tic(); 

			DFSTop *top = new DFSTop(pMaze);
			DFSBottom *bot = new DFSBottom(pMaze);
			StudentSolveBuilder *studentSolver = new StudentSolveBuilder(pMaze, top, bot);
			std::list<Direction> *pSolutionStudent = studentSolver->Solve();
		
		MT_Student_Timer.toc();

		// Verify solution - do not delete, it is not timed
		pMaze->checkSolution( pSolutionStudent );

		// release memory
		delete pSolutionStudent;
		delete studentSolver;
		delete bot;
		delete top;
		delete pMaze;

	// Stats --------------------------------------------------------------------------

		float ST_DFSTime = ST_DFS_Timer.timeInSeconds();
		float ST_BFSTime = ST_BFS_Timer.timeInSeconds();
		float MT_StudentTime = MT_Student_Timer.timeInSeconds();

		printf("\n");
		printf("Results (%s):\n\n",inFileName);
		printf("   BFS      : %f s\n",ST_BFSTime);
		printf("   DFS      : %f s\n",ST_DFSTime);
		printf("   Student  : %f s\n",MT_StudentTime);

	printf("\nMaze: end() --------------\n\n"); 

#if !FINAL_SUBMIT
	system("PAUSE");
#endif

}

