#ifndef MMAP_CPP_MMAP_FILE_READER_H_
#define MMAP_CPP_MMAP_FILE_READER_H_

#include "file_reader.h"
#include <stdint.h>
#include <string>

namespace mmap_cpp {
namespace {
constexpr int kDefaultPageSize = 4096;
}

class MMapFileReader : public FileReader {
public:
	~MMapFileReader() override { Close(); }
	MMapFileReader()
		: fd_(-1), page_index_(-1),
	    read_index_(0), page_size_(kDefaultPageSize), file_size_(0),
	    addr_(nullptr) {}

	bool Open(const std::string &filename) override;
	int Read(char *buffer, int size) override;
	bool Close() override;
private:
	bool LoadNextPage();
	int Read(char *buffer, int offset, int size);
	int fd_;
	int page_index_;
	int read_index_;
	int64_t page_size_;
	int64_t file_size_;
	char *addr_;
};
} // namespace mmap_cpp

#endif // MMAP_CPP_MMAP_FILE_READER_H_
