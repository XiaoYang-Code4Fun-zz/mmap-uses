#include "file_reader.h"
#include "vanilla_file_reader.h"
#include "mmap_file_reader.h"
#include "perf_test.h"
#include <chrono>
#include <iostream>
#include <locale>
#include <vector>

namespace {
constexpr long long kBufferSize = 10000;
constexpr long long kIteration = 10000;
}

void run(mmap_cpp::FileReader *fr, const std::string &filename) {
	char buffer[kBufferSize];
	if (!fr->Open(filename)) {
		return;
	}
	for (long long i = 0; i < kIteration; i ++) {
		if (fr->Read(buffer, kBufferSize) < 0) {
			return;
		}
	}
	fr->Close();
}

int main() {
	mmap_cpp::VanillaFileReader vfr;
	mmap_cpp::MMapFileReader mfr;

	PerfTestHelper timer;
	timer.Register();
	run(&vfr, "tmp_vfa");
	timer.Finish(kBufferSize * kIteration, "Vanilla File Reader");

	timer.Register();
	run(&mfr, "./tmp_mfa");
	timer.Finish(kBufferSize * kIteration, "MMap File Reader");
	return 0;
}
