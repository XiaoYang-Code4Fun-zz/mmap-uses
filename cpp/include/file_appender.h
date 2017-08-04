#ifndef MMAP_CPP_INCLUDE_FILE_APPENDER_H_
#define MMAP_CPP_INCLUDE_FILE_APPENDER_H_

#include <string>

namespace mmap_cpp {

class FileAppender {
public:
	virtual ~FileAppender() {}
	virtual bool Open(const std::string &filename) = 0;
	virtual bool Write(char *buffer, int size) = 0;
	virtual bool Close() = 0;
};

} // namespace mmap_cpp

#endif // MMAP_CPP_INCLUDE_FILE_APPENDER_H_

