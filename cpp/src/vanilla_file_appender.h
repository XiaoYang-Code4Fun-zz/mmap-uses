#ifndef MMAP_CPP_VANILLA_FILE_APPENDER_H_
#define MMAP_CPP_VANILLA_FILE_APPENDER_H_

#include "file_appender.h"

namespace mmap_cpp {
	
class VanillaFileAppender : public FileAppender {
public:
	VanillaFileAppender() : f_(nullptr), flush_counter_(0) {}
	~VanillaFileAppender() { Close(); }
	
	bool Open(const std::string &filename) override;
	bool Write(char *buffer, int size) override;
	bool Close() override;
private:
	FILE *f_;
	int flush_counter_;
};

} // namespace mmap_cpp

#endif // MMAP_CPP_VANILLA_FILE_APPENDER_H_

