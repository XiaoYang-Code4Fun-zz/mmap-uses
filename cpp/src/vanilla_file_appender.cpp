#include "vanilla_file_appender.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>

namespace mmap_cpp {
namespace {
constexpr int kFlushPeriod = 4096;
}

bool VanillaFileAppender::Open(const std::string &filename) {
	if (!(f_ = fopen(filename.c_str(), "ab"))) {
		perror("Unable to open file");
		return false;
	}
	return true;
}
	
bool VanillaFileAppender::Write(char *buffer, int size) {
	if (fwrite(buffer, 1, size, f_) != size) {
		perror("Unable to finish writing");
		return false;
	}
	// Periodically flush to reduce the risk of lossing
	// large amount of unflushed data.
	flush_counter_ += size;
	if (flush_counter_ > kFlushPeriod) {
		fflush(f_);
		flush_counter_ = 0;
	}
	return true;
}
	
bool VanillaFileAppender::Close() {
	if (!f_) {
		return true;
	}
	fclose(f_);
	f_ = nullptr;
	return true;
}

} // namespace mmap_cpp
