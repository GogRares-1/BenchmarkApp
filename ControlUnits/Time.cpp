
#include "Time.hpp"

using namespace std;

static std::chrono::high_resolution_clock::time_point startTime;

void Timing::start() {
	startTime = std::chrono::high_resolution_clock::now();
}

double Timing::stop() {
	auto endTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = endTime - startTime;

	return elapsed.count();
}