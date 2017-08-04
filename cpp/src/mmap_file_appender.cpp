#include "mmap_file_appender.h"
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
constexpr int kPageSizeMultipler = 20;
}
	
bool MMapFileAppender::Open(const std::string &filename) {
	page_size_ = sysconf(_SC_PAGESIZE) * kPageSizeMultipler;
	if ((fd_ = open(filename.c_str(), O_RDWR | O_APPEND | O_CREAT, S_IRWXU | S_IRGRP | S_IROTH)) < 0) {
		perror("Unable to open file");
		return false;
	}
	return FirstTimeExtendFileAndCreateMMap();
}

bool MMapFileAppender::Write(char *buffer, int size) {
	if (size == 0) {
		return true;
	}
	int more_space = std::max(write_index_ + size - page_size_, 0);
	// First write to the current page until there is no space
	int write_size = size - more_space;
	memcpy(addr_ + write_index_, buffer, write_size);
	write_index_ += write_size;
	if (more_space > 0) {
		// Only create new page if there are more data to be written.
		// Flush the memory
		if (msync(addr_, page_size_, MS_ASYNC) < 0) {
		  perror("Unable to persist data");
			return false;
		}
		if (munmap(addr_, page_size_) < 0) {
		  perror("Unable to cleanup memory map");
			return false;
		}
		// Remap the file.
		file_offset_ += page_size_;
		write_index_ = 0;
		if (!ExtendFileAndCreateMMap()) {
			return false;
		}
		return Write(buffer + write_size, more_space);
	}
	return true;
}
	
bool MMapFileAppender::Close() {
	if (fd_ < 0) {
		return true;
	}
	if (write_index_ < page_size_) {
		if (ftruncate(fd_, file_offset_ + write_index_) < 0) {
			perror("Unable to truncate file size");
			return false;
		}
	}
	// Need to flush the content in the last page.
	if (msync(addr_, write_index_, MS_ASYNC) < 0) {
	  perror("Unable to persist data");
		return false;
	}
	if (munmap(addr_, write_index_) < 0) {
	  perror("Unable to cleanup memory map");
		return false;
	}
	close(fd_);
	fd_ = -1;
	return false;
}

bool MMapFileAppender::FirstTimeExtendFileAndCreateMMap() {
	// File size may not be multiples of page_size_
	// We need to around up to page_size_ and set write_index_ at file size.
	struct stat st;
	if (fstat(fd_, &st) < 0) {
		perror("Unable to obtain file stats");
		return false;
	}
	int pages = st.st_size / page_size_;
	file_offset_ = pages * page_size_;
	write_index_ = st.st_size - file_offset_;
	
	return ExtendFileAndCreateMMap();
}

bool MMapFileAppender::ExtendFileAndCreateMMap() {
	if (ftruncate(fd_, file_offset_ + page_size_) < 0) {
		perror("Unable to extend file size");
		return false;
	}
	if ((addr_ = (char*)mmap(NULL, page_size_, PROT_WRITE, MAP_SHARED, fd_, file_offset_)) == MAP_FAILED) {
		perror("Unable to create memory map for the file");
		return false;
	}
	// Lock this page so that it won't be swapped out.
	if (mlock(addr_, page_size_) < 0){
	  perror("Unable to lock new page");
	  return false;
	}
	return true;
}

} // namespace mmap_cpp

