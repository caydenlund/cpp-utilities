#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

#include <cstdint>
#include <exception>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace util {

    class bmp {
    public:
        struct pix {
            uint8_t red;
            uint8_t green;
            uint8_t blue;
        };

        bmp(size_t width, size_t height);

        pix& operator()(size_t x_pix, size_t y_pix);

        void save(const std::string& filename) const;

        const size_t height;
        const size_t width;

    private:
        /**
         * @brief The literal characters `BM`.
         *
         */
        static const uint16_t _bm = 0x4D42;

        /**
         * @brief The file header size in bytes.
         *
         */
        static const uint32_t _file_header_size = 14;

        /**
         * @brief The image header size in bytes.
         *
         */
        static const uint32_t _image_header_size = 40;

        /**
         * @brief The number of bytes per pixel.
         *
         */
        static const uint16_t _bytes_per_pixel = 3;

        /**
         * @brief The number of bits per pixel.
         *
         */
        static const uint16_t _bits_per_pixel = 24;

        /**
         * @brief The alignment amount.
         *
         */
        static const uint8_t _alignment = 4;

#pragma pack(push, 1)
        /**
         * @brief The file header.
         * @details 14 bytes.
         *     2: Literal `"BM"`.
         *     4: File size in bytes.
         *     2: Zero.
         *     2: Zero.
         *     4: Offset to the pixel array.
         *
         * This data structure is packed.
         *
         */
        struct file_header {
            /**
             * @brief Class constructor.
             * @details Default-initializes all fields except `file_size`.
             *
             * @param file_size The total size of the file, in bytes.
             */
            file_header(uint32_t file_size) : file_size(file_size) {}

            /**
             * @brief Literal `"BM"` (2 bytes).
             *
             */
            uint16_t header_intro {_bm};

            /**
             * @brief File size in bytes (4 bytes).
             *
             */
            uint32_t file_size;

            /**
             * @brief Zero (2 bytes).
             *
             */
            uint16_t reserved_1 {0};

            /**
             * @brief Zero (2 bytes).
             *
             */
            uint16_t reserved_2 {0};

            /**
             * @brief Offset to the pixel array (4 bytes).
             *
             */
            uint32_t offset_to_pixel_array {_file_header_size + _image_header_size};
        };
        static_assert(sizeof(file_header) == _file_header_size);

        /**
         * @brief The image header.
         * @details 40 bytes.
         *     4: The image header size (exactly 40).
         *     4: The image width in pixels.
         *     4: The image height in pixels.
         *     2: The number of image planes (exactly 1).
         *     2: The number of bits per pixel (exactly 24).
         *     4: The compression method (exactly 0).
         *     4: The image size in bytes (set to 0; allowed because the image is uncompressed).
         *     4: The horizontal resolution in pixels per meter (set to 0, meaning no preference).
         *     4: The vertical resolution in pixels per meter (set to 0, meaning no preference).
         *     4: The number of used color map entries (set to 0).
         *     4: The number of significant colors (set to 0, meaning all colors are important).
         *
         * This data structure is packed.
         *
         */
        struct image_header {
            /**
             * @brief Constructor.
             * @details Default-initializes all fields but `image_width` and `image_height`.
             *
             * @param image_width The width of the image, in pixels.
             * @param image_height The height of the image, in pixels.
             */
            image_header(uint32_t image_width, uint32_t image_height)
                : image_width(image_width), image_height(image_height) {}

            /**
             * @brief The image header size (exactly 40) (4 bytes).
             *
             */
            uint32_t image_header_size {_image_header_size};

            /**
             * @brief The image width in pixels (4 bytes).
             *
             */
            uint32_t image_width;

            /**
             * @brief The image height in pixels (4 bytes).
             *
             */
            uint32_t image_height;

            /**
             * @brief The number of image planes (exactly 1) (2 bytes).
             *
             */
            uint16_t number_of_image_planes {1};

            /**
             * @brief The number of bits per pixel (exactly 24) (2 bytes).
             *
             */
            uint16_t bits_per_pixel {_bits_per_pixel};

            /**
             * @brief The compression method (exactly 0) (4 bytes).
             *
             */
            uint32_t compression_method {0};

            /**
             * @brief The image size in bytes (set to 0; allowed because the image is uncompressed) (4 bytes).
             *
             */
            uint32_t image_size {0};

            /**
             * @brief The horizontal resolution in pixels per meter (set to 0, meaning no preference) (4 bytes).
             *
             */
            uint32_t horizontal_resolution {0};

            /**
             * @brief The vertical resolution in pixels per meter (set to 0, meaning no preference) (4 bytes).
             *
             */
            uint32_t vertical_resolution {0};

            /**
             * @brief The number of used color map entries (set to 0) (4 bytes).
             *
             */
            uint32_t number_of_color_map_entries {0};

            /**
             * @brief The number of significant colors (set to 0, meaning all colors are important) (4 bytes).
             *
             */
            uint32_t number_of_important_colors {0};
        };
        static_assert(sizeof(image_header) == _image_header_size);
#pragma pack(pop)

        /**
         * @brief The file header data.
         *
         */
        const file_header _file_header;

        /**
         * @brief TheMi image header data.
         *
         */
        const image_header _image_header;

        /**
         * @brief The pixel data.
         * @details Saved as rows of columns of pixels.
         *     That is, `_pixels[0]` is the top row of the image and `_pixels[0][0]` is the top-left pixel.
         *
         */
        std::vector<std::vector<pix>> _pixels;
    };

    bmp::bmp(size_t width, size_t height)
        : width(width), height(height), _pixels(height, std::vector<pix>(width)), _file_header([&]() {
              const uint32_t header_size = _file_header_size + _image_header_size;

              const uint32_t row_size = (width * sizeof(pix));
              const uint32_t row_padding = (_alignment - (row_size % _alignment)) % _alignment;

              return header_size + height * row_size + (height - 1) * row_padding;
          }()),
          _image_header(width, height) {}

    bmp::pix& bmp::operator()(size_t x_pix, size_t y_pix) { return _pixels[y_pix][x_pix]; }

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-pro-type-reinterpret-cast"
    void bmp::save(const std::string& filename) const {
        std::ofstream output(filename, std::ios::binary);
        if (!output.is_open()) throw std::runtime_error("Error: could not open output file `" + filename + "`.");

        //  1. The file header (14 bytes).
        output.write(reinterpret_cast<const char*>(&_file_header), _file_header_size);

        //  2. The image header (40 bytes).
        output.write(reinterpret_cast<const char*>(&_image_header), _image_header_size);

        //  3. The color table (0 bytes).

        //  4. The pixel data.
        if (this->height > 0) {
            const uint32_t row_size = width * _bytes_per_pixel;
            std::vector<uint8_t> row(row_size);
            for (size_t row_num = 0; row_num < this->height; ++row_num) {
                const uint32_t row_ind = this->height - row_num - 1;
                for (size_t col_ind = 0; col_ind < this->width; ++col_ind) {
                    const auto& pixel = _pixels[row_ind][col_ind];
                    row[_bytes_per_pixel * col_ind] = static_cast<uint8_t>(pixel.blue);
                    row[_bytes_per_pixel * col_ind + 1] = static_cast<uint8_t>(pixel.green);
                    row[_bytes_per_pixel * col_ind + 2] = static_cast<uint8_t>(pixel.red);
                }
                output.write(reinterpret_cast<const char*>(row.data()), row_size);
            }
        }
    }
#pragma clang diagnostic pop

}  //  namespace util

#pragma clang diagnostic pop
