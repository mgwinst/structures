#include <cstddef>
#include <atomic>
#include <span>
#include <cstring>
#include <algorithm>
#include <print>
#include <memory>

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
        spsc_queue(std::size_t capacity)
        {
            if (capacity <= 0 || ((capacity & (capacity - 1)) != 0)) {
                throw std::invalid_argument("queue capacity must be power of 2");
            }
            capacity_ = capacity;
            data_ = std::make_unique<std::byte[]>(capacity);
        }

        spsc_queue(const spsc_queue&) = delete;
        spsc_queue& operator=(const spsc_queue&) = delete;
        spsc_queue(spsc_queue&&) = delete;
        spsc_queue& operator=(spsc_queue&&) = delete;
        ~spsc_queue() = default;

        auto write(std::span<std::byte> buffer) -> std::size_t
        {
            std::size_t write_pos = write_counter_.load(std::memory_order_acquire);
            std::size_t read_pos = read_counter_.load(std::memory_order_acquire);

            std::size_t bytes_in_queue = write_pos - read_pos;

            if (bytes_in_queue == capacity_)
                return 0;

            std::size_t bytes_to_write = std::min(buffer.size(), (capacity_ - bytes_in_queue));

            std::size_t index = write_pos % capacity_;
            std::size_t bytes_to_end = capacity_ - index;

            if (bytes_to_write <= bytes_to_end) {
                std::memcpy(data_.get() + index, buffer.data(), bytes_to_write);
            } else {
                std::memcpy(data_.get() + index, buffer.data(), bytes_to_end);
                std::memcpy(data_.get(), buffer.data() + bytes_to_end, bytes_to_write - bytes_to_end);
            }

            write_counter_.store(write_pos + bytes_to_write, std::memory_order_release);

            return bytes_to_write;
        }
        
        auto read(std::span<std::byte> buffer) -> std::size_t
        {
            std::size_t write_pos = write_counter_.load(std::memory_order_acquire);
            std::size_t read_pos = read_counter_.load(std::memory_order_acquire);

            std::size_t bytes_in_queue = write_pos - read_pos;

            if (bytes_in_queue == 0)
                return 0;

            std::size_t index = read_pos % capacity_;
            std::size_t bytes_to_read = std::min(buffer.size(), bytes_in_queue);
            std::size_t bytes_to_end = capacity_ - index;

            if (bytes_to_read < bytes_to_end) {
                std::memcpy(buffer.data(), data_.get() + index, bytes_to_read);
            } else {
                std::memcpy(buffer.data(), data_.get() + index, bytes_to_end);
                std::memcpy(buffer.data() + bytes_to_end, data_.get(), bytes_to_read - bytes_to_end);
            }

            read_counter_.store(read_pos + bytes_to_read, std::memory_order_release);

            return bytes_to_read;
        }

        private:
            alignas(cache_line_size) std::atomic<std::size_t> read_counter_{ 0 };
            alignas(cache_line_size) std::atomic<std::size_t> write_counter_{ 0 };
            std::unique_ptr<std::byte[]> data_;
            std::size_t capacity_;
    };
}

int main()
{
    lib::spsc_queue queue{ 1024 };
}
