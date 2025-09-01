#include <cstddef>
#include <atomic>
#include <span>
#include <cstring>
#include <algorithm>
#include <print>

#ifdef __cpp_lib_hardware_interference_size
    inline static constexpr std::size_t cache_line_size = std::hardware_destructive_interference_size;
#else
    inline static constexpr std::size_t cache_line_size{64};
#endif

namespace lib
{
    class spsc_queue
    {
    public:
        spsc_queue(std::size_t capacity) : capacity_{ capacity }
        {
            data_ = new std::byte[capacity];
        }

        ~spsc_queue()
        {
            delete[] data_;
        }

        spsc_queue(const spsc_queue&) = delete;
        spsc_queue& operator=(const spsc_queue&) = delete;
        spsc_queue(spsc_queue&&) = delete;
        spsc_queue& operator=(spsc_queue&&) = delete;

        std::size_t write(std::span<std::byte> buffer)
        {
            std::size_t write_pos = write_counter_.load(std::memory_order_acquire);
            std::size_t read_pos = read_counter_.load(std::memory_order_acquire);
            
            if (write_pos - read_pos + buffer.size() > capacity_)
                return 0;
            
            std::size_t index = write_pos % capacity_;
            std::size_t bytes_to_end = capacity_ - index;
            if (buffer.size() < bytes_to_end) {
                std::memcpy(data_ + index, buffer.data(), buffer.size());
            } else {
                std::memcpy(data_ + index, buffer.data(), bytes_to_end);
                std::memcpy(data_, buffer.data() + bytes_to_end, buffer.size() - bytes_to_end);
            }

            write_counter_.store(write_pos + buffer.size(), std::memory_order_release);

            return buffer.size();
        }
        
        int64_t read(std::span<std::byte> buffer)
        {
            std::size_t write_pos = write_counter_.load(std::memory_order_acquire);
            std::size_t read_pos = read_counter_.load(std::memory_order_acquire);

            std::size_t readable_bytes = write_pos - read_pos;

            if (readable_bytes == 0)
                return false;

            std::size_t bytes_to_read = std::min(readable_bytes, buffer.size());
            std::size_t index = read_pos;
            std::size_t bytes_to_end = capacity_ - index;

            if (bytes_to_read <= bytes_to_end) {
                std::memcpy(buffer.data(), data_ + index, bytes_to_read);
            } else {
                std::memcpy(buffer.data(), data_ + index, bytes_to_end);
                std::memcpy(buffer.data() + bytes_to_end, data_, bytes_to_read - bytes_to_end);
            }
            
            read_counter_.store(read_counter_ + bytes_to_read, std::memory_order_release);

            return bytes_to_read;
        }

        private:
            alignas(cache_line_size) std::atomic<int> read_counter_;
            alignas(cache_line_size) std::atomic<int> write_counter_;
            alignas(cache_line_size) std::byte* data_;
            std::size_t capacity_;
    };
}

int main()
{

}
