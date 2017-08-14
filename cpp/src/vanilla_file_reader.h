#ifndef MMAP_CPP_VANILLA_FILE_READER_H_
#define MMAP_CPP_VANILLA_FILE_READER_H_

#include "file_reader.h"

namespace mmap_cpp {

class VanillaFileReader : public FileReader {
public:
	VanillaFileReader() : f_(nullptr) {}
	~VanillaFileReader() { Close(); }

	bool Open(const std::string &filename) override;
	int Read(char *buffer, int size) override;
	bool Close() override;
private:
	FILE *f_;
};

} // namespace mmap_cpp

#endif // MMAP_CPP_VANILLA_FILE_READER_H_

