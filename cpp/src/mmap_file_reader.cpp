#include "mmap_file_reader.h"
#include <stdio.h>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <algorithm>
#include <iostream>

namespace mmap_cpp {
namespace {
constexpr int kPageSizeMultipler = 200;
}

bool MMapFileReader::Open(const std::string &filename) {
	page_size_ = sysconf(_SC_PAGESIZE) * kPageSizeMultipler;
	if ((fd_ = open(filename.c_str(), O_RDONLY, S_IREAD)) < 0) {
		perror("Unable to open file");
		return false;
	}
	struct stat s;
	if (fstat(fd_, &s) < 0) {
		perror("Unable to get file size");
		return false;
	}
	file_size_ = s.st_size;
	return LoadNextPage();
}

int MMapFileReader::Read(char *buffer, int size) {
	int size_to_read = (int)std::min((int64_t)size, file_size_ - read_index_);
	return Read(buffer, 0, size_to_read);
}

bool MMapFileReader::Close() {
	if (!addr_) {
		if (munmap(addr_, page_size_) < 0) {
		  perror("Unable to cleanup memory map");
			return false;
		}
	}
	if (fd_ >= 0) {
		close(fd_);
	}
	fd_ = -1;
	return false;
}

int MMapFileReader::Read(char *buffer, int offset, int size) {
	int64_t nex_page_start = (page_index_ + 1) * page_size_;
	if (read_index_ + size > nex_page_start) {
		// Need to load next page.
		int available_space = (int)(nex_page_start - read_index_);
		memcpy(buffer + offset, addr_, available_space);
		read_index_ += available_space;
		if (!LoadNextPage()) {
			perror("Unable to continue reading");
			return -1;
		}
		Read(buffer, offset + available_space, size - available_space);
	} else {
		memcpy(buffer + offset, addr_, size);
		read_index_ += size;
	}
	return size;
}

bool MMapFileReader::LoadNextPage() {
	page_index_ ++;
	if (page_index_ > 0) {
		// Clean up previous page.
		if (munmap(addr_, page_size_) < 0) {
			perror("Unable to cleanup memory map");
			return false;
		}
	}
	if ((addr_ = (char*)mmap(NULL, page_size_, PROT_READ, MAP_PRIVATE, fd_, read_index_)) == MAP_FAILED) {
		perror("Unable to create memory map for the file");
		return false;
	}
	return true;
}

} // namespace mmap_cpp

