#include "pch.h"
#include <windows.h>
#include "Maze.h"

int main()
{
	forge::Time::Initialize();

	Maze maze;

	while( true )
	{
		forge::Time::Update();
		forge::StopWatch sw;

		maze.Update( forge::Time::GetDeltaTime() );
		
		SetConsoleTitle( std::to_wstring( 1.0f / sw.GetDuration() ).c_str() );
	}
}