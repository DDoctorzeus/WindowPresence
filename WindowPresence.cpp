// WindowPresence.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <cstdlib>
#include <windows.h>
#include <winuser.h>
#include <conio.h>
#include <thread>

using namespace std;

//NOTE: Not crypto-random so predictive, but tries to make input as realistic as possible (can probably be improved

//SET THIS TO THE DESIRED WINDOW NAME
const char WINDOWNAME[] = "Untitled - Notepad";
const bool BRINGFOREGROUND = false;


int main()
{
	const short WINKEYCODEADJUST = ('A' - 0x41);
	bool keepAlive = true;

	//Use startup time for random seed
	srand(time(nullptr));

	while (keepAlive)
	{
		HWND window = NULL;

		printf("Beginning Cycle..\n");

		//Loop looking for window
		while (window == NULL)
		{
			//Sleep while null
			if ((window = FindWindowA(NULL, WINDOWNAME)) == NULL)
			{
				printf("Waiting for window to appear..\n");
				this_thread::sleep_for(chrono::seconds(1));
			}
		}

		//THIS LINE WAS FOR TESTING (GRABBING THE TEXT WINDOW INSIDE NOTEPAD)
		//window = FindWindowEx(window, NULL, L"Edit", NULL);

		//Get the window info
		RECT windowInfo;
		if (GetWindowRect(window, &windowInfo))
		{
			//Random how many mouse moves there are (between 2 and 12)
			short simInputCount = (rand() % (12 + 1 - 2) + 2);

			for (int i = 0; i < simInputCount; i++)
			{
				//bit random if we want to decide on a mouse move or a keyboard input
				bool mouseMove = (rand() % 2);

				//Bring window to foreground if asked
				if (BRINGFOREGROUND && !SetForegroundWindow(window))
					printf("Error bringing window to foreground, continuing..\n");

				//If we are moving the mouse
				if (mouseMove)
				{
					//Get random positions to move to based off window size
					short moveX = (rand() % (windowInfo.right + 1 - windowInfo.left) + windowInfo.left);
					short moveY = (rand() % (windowInfo.bottom + 1 - windowInfo.top) + windowInfo.top);

					printf("Simulating mouse move at: %i,%i\n", moveX, moveY);
					LRESULT inputResult = SendMessage(window, WM_MOUSEMOVE, MK_LBUTTON, MAKELONG(moveX, moveY));
				}
				else
				{
					//Get random numeric key to press (betweem A TO Z and then adjust for keycodes hex value in keycodes (https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes))
					char randomKey = (rand() % ('Z' + 1 - 'A') + 'A') - WINKEYCODEADJUST;

					//Simulate the press
					printf("Simulating key press: %c\n", (randomKey+WINKEYCODEADJUST));
					LRESULT inputResult = SendMessage(window, WM_CHAR, randomKey, NULL);
				}
			}
		}
		else
			printf("Could not get window info so moving on...\n");

		//Randomize the wait time (between 1-120 seconds) and sleep for that length
		short sleepTime = (rand() % (120 + 1));
		printf("Seelping for %i seconds till next cycle, ZZZZ.\n", sleepTime);
		this_thread::sleep_for(chrono::seconds(sleepTime));
	}
}