#include "file_appender.h"
#include "vanilla_file_appender.h"
#include "mmap_file_appender.h"
#include <chrono>
#include <iostream>
#include <locale>
#include <vector>

namespace {
constexpr int kBufferSize = 10000;
constexpr int kIteration = 100000;
}

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

std::vector<int> get_data_sizes() {
	std::vector<int> sizes;
	for (int i = 0; i < kIteration; i ++) {
		sizes.push_back(rand() % kBufferSize + 1);
	}
	return sizes;
}

long long get_total_size(const std::vector<int> &sizes) {
	long long total_size = 0;
	for (auto& size : sizes) {
	  total_size += size;
	}
	return total_size;
}

void run(mmap_cpp::FileAppender *fa, const std::string &filename, const std::vector<int> &sizes) {
	char buffer[kBufferSize];
	fa->Open(filename);
	for (int i = 0; i < kIteration; i ++) {
		fa->Write(buffer, sizes[i]);
	}
	fa->Close();
}

int main() {
	mmap_cpp::VanillaFileAppender vfa;
	mmap_cpp::MMapFileAppender mfa;
	std::vector<int> sizes = get_data_sizes();
	long long total_size = get_total_size(sizes);
	PerfTestHelper timer;
	timer.Register();
	run(&vfa, "tmp_vfa", sizes);
	timer.Finish(total_size, "Vanilla File Appender");
	timer.Register();
	run(&mfa, "tmp_mfa", sizes);
	timer.Finish(total_size, "MMap File Appender");
	return 0;
}