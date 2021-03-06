#include <cstdint>
#include <cstddef>

#include "frame_buffer_config.hpp"

struct PixelColor {
    uint8_t r, g, b;
};

class PixelWriter {
    public:
        // クラス名と同名の関数はコンストラクタ． ~ があるものはデストラクタ
        // クラスそのものがメモリ上に乗るわけではないので，クラス型の変数を定義して初めて
        // クラスの定義に従ってインスタンスがメモリ上に生成される．
        // インスタンスをメモリ上に生成するために呼ばれるのがコンストラクタ．
        // インスタンスを破棄するために呼ばれるのがデストラクタ
        PixelWriter(const FrameBufferConfig& config) : config_{config} {
        }
        virtual ~PixelWriter() = default;
        // = 0: 純粋仮想関数．インタフェースのみで，実装はあとで与える．
        virtual void Write(int x, int y, const PixelColor& c) = 0;
    protected:
        uint8_t* PixelAt(int x, int y) {
            return config_.frame_buffer + 4 * (config_.pixels_per_scan_line * y + x);
        }

    private:
        const FrameBufferConfig& config_;
};

class RGBResv8BitPerColorPixelWriter : public PixelWriter {
    public:
        // `using ...` により Parent class のコンストラクタをそのまま child class のコンストラクタとして利用可能
        using PixelWriter::PixelWriter;

        virtual void Write(int x, int y, const PixelColor& c) override {
            auto p = PixelAt(x, y);
            p[0] = c.r;
            p[1] = c.g;
            p[2] = c.b;
        }
};

class BGRResv8BitPerColorPixelWriter : public PixelWriter {
    public:
        using PixelWriter::PixelWriter;

        virtual void Write(int x, int y, const PixelColor& c) override {
            auto p = PixelAt(x, y);
            p[0] = c.b;
            p[1] = c.g;
            p[2] = c.r;
        }
};

void* operator new(size_t size, void* buf) {
    return buf;
}

void operator delete(void* obj) noexcept {
}

char pixel_writer_buf[sizeof(RGBResv8BitPerColorPixelWriter)];
PixelWriter* pixel_writer;

extern "C" void KernelMain(const FrameBufferConfig& frame_buffer_config) {
    switch (frame_buffer_config.pixel_format) {
        case kPixelRGBResv8BitPerColor:
            pixel_writer = new(pixel_writer_buf)
                RGBResv8BitPerColorPixelWriter{frame_buffer_config};
            break;
        case kPixelBGRResv8BitPerColor:
            pixel_writer = new(pixel_writer_buf)
                BGRResv8BitPerColorPixelWriter{frame_buffer_config};
            break;
    }
    for (int x = 0; x < frame_buffer_config.horizontal_resolution; ++x) {
        for (int y = 0; y < frame_buffer_config.vertical_resolution; ++y) {
            pixel_writer->Write(x, y, {255, 255, 255});
        }
    }
    for (int x = 0; x < 200; ++x) {
        for (int y = 0; y < 100; ++y) {
            pixel_writer->Write(x, y, {0, 255, 0});
        }
    }
    while (1) __asm__("hlt");
}
