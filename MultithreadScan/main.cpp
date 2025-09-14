#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <cstdlib> // Needed for rand() and srand()
#include <ctime>   // Needed for time()

using namespace std;

static vector<int> container;

//Search method for threads
void search(int start, int end, int numberToSearch, int* ptr) {
	int timesFound = 0;
	for (int i = start; i < end; i++) {
		if (container.at(i) == numberToSearch) {
			timesFound++;
		}
	}
	*ptr = *ptr + timesFound;
}  // 

int main() {

	// ---  CONTROL PANEL -------------|
	int SIZE = 1000000;		//Size of the integer list
	const int Cores = 2;	//Core number you want to use - Avoid using higher number than your total CPU Cores
	int numberToSearch = 0; //The integer we are searching for
	// --------------------------------|



	// ---  LIST CREATION -------------|
	srand(time(0));
	for (int i = 0; i < SIZE; i++) {
		int num = rand() % SIZE;
		container.push_back(num);
	}//--------------------------------|



	// ---  Scanning prosess ----------|
	cout << "Scanning list using 1 thread/cpu core \n";
	int timesFound = 0;
	int* ptr = &timesFound;
	auto StartTime = std::chrono::high_resolution_clock::now();
	thread th(search, 0, SIZE, numberToSearch, ptr);
	th.join();
	auto finishTime = std::chrono::high_resolution_clock::now();
	float timePassed = std::chrono::duration<float, std::chrono::seconds::period>(finishTime - StartTime).count();
	std::cout << timePassed << "ms to scan all the list and found element " << timesFound << " times \n\n";
	//---------------------------------|


	// --- MultiThread Scan -----------|
	cout << "Now Scanning same list using " << Cores << " threads / cpu cores \n";
	timesFound = 0;
	thread threads[Cores];
	int start = 0;
	int end = 0;
	int range = SIZE / Cores;
	StartTime = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < Cores; i++) {
		start = end;
		if (i == Cores - 1) {
			end = SIZE;   // divisions are not perfect sometimes
		}
		else {
			end = end + range;
		}
		threads[i] = thread(search, start, end, numberToSearch, ptr);
	}
	for (int i = 0; i < Cores; i++) {
		threads[i].join();
	}
	finishTime = std::chrono::high_resolution_clock::now();
	float timePassed2 = std::chrono::duration<float, std::chrono::seconds::period>(finishTime - StartTime).count();
	std::cout << timePassed2 << "ms to scan using multithread and found element " << timesFound << " times \n";
	std::cout << "Approximately " << timePassed / timePassed2 << " times faster \n";
	//---------------------------------|

	//thread th3(&thirdThread::run);
	//thread th2(&threadex::run);
	//th3.join();
	//cout << " message from main thread \n";
	//th2.join();
	return 0;
}
