#ifndef MMAP_CPP_MMAP_FILE_APPENDER_H_
#define MMAP_CPP_MMAP_FILE_APPENDER_H_

#include "file_appender.h"
#include <string>

namespace mmap_cpp {
namespace {
constexpr int kDefaultPageSize = 4096;
}
	
class MMapFileAppender : public FileAppender {
public:
	~MMapFileAppender() override { Close(); }
	MMapFileAppender()
		: fd_(-1), file_offset_(0), 
	    write_index_(0), page_size_(kDefaultPageSize),
	    addr_(nullptr) {}
			
	bool Open(const std::string &filename) override;
	bool Write(char *buffer, int size) override;
	bool Close() override;
private:
	bool FirstTimeExtendFileAndCreateMMap();
	bool ExtendFileAndCreateMMap();
	int fd_;
	int file_offset_;
	int write_index_;
	int page_size_;
	char *addr_;
};
} // namespace mmap_cpp

#endif // MMAP_CPP_MMAP_FILE_APPENDER_H_
