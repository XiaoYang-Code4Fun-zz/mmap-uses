#include "vanilla_file_reader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>

namespace mmap_cpp {

bool VanillaFileReader::Open(const std::string &filename) {
	if (!(f_ = fopen(filename.c_str(), "rb"))) {
		perror("Unable to open file");
		return false;
	}
	return true;
}

int VanillaFileReader::Read(char *buffer, int size) {
	return fread(buffer, size, 1, f_);
}

bool VanillaFileReader::Close() {
	if (!f_) {
		return true;
	}
	fclose(f_);
	f_ = nullptr;
	return true;
}

} // namespace mmap_cpp
