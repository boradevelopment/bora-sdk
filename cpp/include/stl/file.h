#include <streambuf>
#include <vector>
#include <string>
#include <istream>
#include <macros>

namespace bora {

    namespace private_file { // private namespace so it doesn't appear in intellisense.
    IMPORT_ATTR("bora::fs", "openFile")
    int32_t bora_open_file(const char* path_ptr, const char* mode);
    IMPORT_ATTR("bora::fs", "seekFile")
    int32_t bora_seek_file(int32_t file_handle, int64_t offset, int32_t pos);
    IMPORT_ATTR("bora::fs", "readFile")
    int32_t bora_read_file(int32_t file_handle, char* buf_ptr, uint64_t size, uint64_t nmemb);
    IMPORT_ATTR("bora::fs", "closeFile")
    int32_t bora_close_file(int32_t file_handle);
    }

    class FileStreamBuf : public std::streambuf {
    public:
        explicit FileStreamBuf(const std::string& path) {
            fileHandle_ = private_file::bora_open_file(path.data(), "");
            
            // 2. Set up a small internal read buffer
            if (fileHandle_ >= 0) {
                buffer_.resize(512);
                setg(buffer_.data(), buffer_.data(), buffer_.data());
            }
        }

        virtual ~FileStreamBuf() {
            if (fileHandle_ >= 0) {
                private_file::bora_close_file(fileHandle_);
            }
        }

        int getHandle() const {
            return fileHandle_;
        }

    protected:
        // Called when the stream buffer runs out of characters and needs more data
        int_type underflow() override {
        if (fileHandle_ < 0) return traits_type::eof();

        if (gptr() < egptr()) {
            return traits_type::to_int_type(*gptr());
        }

        // Using size = 1 byte, nmemb = buffer_.size()
        int bytesRead = private_file::bora_read_file(fileHandle_, buffer_.data(), buffer_.size(), 1);
        if (bytesRead <= 0) {
            return traits_type::eof();
        }

        setg(buffer_.data(), buffer_.data(), buffer_.data() + bytesRead);
        return traits_type::to_int_type(*gptr());
        }

        pos_type seekoff(off_type off, std::ios_base::seekdir dir, std::ios_base::openmode which = std::ios_base::in) override {
        if (fileHandle_ < 0 || !(which & std::ios_base::in)) {
            return pos_type(off_type(-1));
        }

        int whence = SEEK_SET;
        if (dir == std::ios_base::cur) whence = SEEK_CUR;
        else if (dir == std::ios_base::end) whence = SEEK_END;

        if (dir == std::ios_base::cur) {
            off -= (egptr() - gptr());
        }

        int32_t result = private_file::bora_seek_file(fileHandle_, off, whence);
        if (result < 0) return pos_type(off_type(-1));

        setg(buffer_.data(), buffer_.data() + buffer_.size(), buffer_.data() + buffer_.size());

        return pos_type(result);
    }

    private:
        int32_t fileHandle_ = -1;
        std::vector<char> buffer_;
    };

    // A drop-in custom file class that behaves like an ifstream
    class File : public std::istream {
    public:
        explicit File(const std::string& path) 
            : std::istream(&buf_), buf_(path) {}

        bool is_open() const {
            return buf_.getHandle() > 0; 
        }
    private:
        FileStreamBuf buf_;
    };
    }
