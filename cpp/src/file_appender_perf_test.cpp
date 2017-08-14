#include "file_appender.h"
#include "vanilla_file_appender.h"
#include "mmap_file_appender.h"
#include "perf_test.h"
#include <chrono>
#include <iostream>
#include <locale>
#include <vector>

namespace {
constexpr int kBufferSize = 10000;
constexpr int kIteration = 10000;
}

void run(mmap_cpp::FileAppender *fa, const std::string &filename) {
	char buffer[kBufferSize];
	fa->Open(filename);
	for (int i = 0; i < kIteration; i ++) {
		fa->Write(buffer, kBufferSize);
	}
	fa->Close();
}

int main() {
	mmap_cpp::VanillaFileAppender vfa;
	mmap_cpp::MMapFileAppender mfa;

	PerfTestHelper timer;
	timer.Register();
	run(&vfa, "tmp_vfa");
	timer.Finish(kBufferSize * kIteration, "Vanilla File Appender");

	timer.Register();
	run(&mfa, "tmp_mfa");
	timer.Finish(kBufferSize * kIteration, "MMap File Appender");
	return 0;
}
