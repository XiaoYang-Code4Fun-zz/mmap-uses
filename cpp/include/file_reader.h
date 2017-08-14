#ifndef MMAP_CPP_INCLUDE_FILE_READER_H_
#define MMAP_CPP_INCLUDE_FILE_READER_H_

#include <string>

namespace mmap_cpp {

class FileReader {
public:
	virtual ~FileReader() {}
	virtual bool Open(const std::string &filename) = 0;
	virtual int Read(char *buffer, int size) = 0;
	virtual bool Close() = 0;
};

} // namespace mmap_cpp

#endif // MMAP_CPP_INCLUDE_FILE_READER_H_
