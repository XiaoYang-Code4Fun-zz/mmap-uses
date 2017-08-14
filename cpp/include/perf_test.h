#ifndef MMAP_CPP_INCLUDE_PERF_TEST_H_
#define MMAP_CPP_INCLUDE_PERF_TEST_H_

#include <chrono>
#include <iostream>
#include <locale>
#include <vector>

class PerfTestHelper {
public:
	void Register() {
		start = std::chrono::system_clock::now();
	}
	void Finish(long long size, const std::string& title) {
		end = std::chrono::system_clock::now();
		auto elapse = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
		std::cout.imbue(std::locale(""));
		std::cout << title << " run with throughput " <<
			size * 1000 / elapse.count() << " bytes/sec." << std::endl;
	}
private:
	std::chrono::time_point<std::chrono::system_clock> start, end;
};

#endif // MMAP_CPP_INCLUDE_PERF_TEST_H_
