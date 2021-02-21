#pragma once

// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <stb_image.h>
#include <Platform/Generic/ImageLoadFormat.h>

namespace nuts {
    struct File {
        static std::string Read(std::filesystem::path filePath, std::ios_base::openmode fileMode = std::ios::binary) {
            std::ifstream mFile(filePath.string(), fileMode);

            if (!mFile.is_open()) {
                NUTS_LOG_WIN(auto stdOutHandle = ::GetStdHandle(STD_OUTPUT_HANDLE));
                NUTS_LOG_WIN(::SetConsoleTextAttribute(stdOutHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY));
                NUTS_LOG(std::cout << "Failed to open file: " << filePath.string() << "!\n");
                // TODO: Better manage exceptions
                throw std::exception {};
            }

            mFile.seekg(0, std::ios::end);
            auto fileSize = static_cast< std::size_t >(mFile.tellg());

            if (fileSize == 0) {
                NUTS_LOG_WIN(auto stdOutHandle = ::GetStdHandle(STD_OUTPUT_HANDLE));
                NUTS_LOG_WIN(::SetConsoleTextAttribute(stdOutHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY));
                NUTS_LOG(std::cout << "Opened an empty file: " << filePath.string() << "!\n");
                // TODO: Better manage exceptions
                throw std::exception {};
            }

            std::string fileData;
            fileData.reserve(fileSize);

            mFile.seekg(0, std::ios::beg);
            fileData.assign((std::istreambuf_iterator< char >(mFile)), std::istreambuf_iterator< char >());
            mFile.close();

            return std::move(fileData);
        }

        /// <summary>
        /// This function accepts a _ContainerType that defines
        /// _ContainerType::value_type* data(void) function to access the contained type
        /// and
        /// void resize(_ContainerType::size_type) to resize a contained data type of certain size
        /// and
        /// _Container_Type holds an integral data type
        /// </summary>
        /// <typeparam name="_ContainerType"></typeparam>
        /// <param name="filePath"></param>
        /// <param name="fileMode"></param>
        /// <returns></returns>
        template < class ContainerType >
        requires(has_data_function< ContainerType >&& std::is_default_constructible_v< ContainerType >&& has_resize_function< ContainerType >&&
                     std::integral< typename ContainerType::value_type >) static ContainerType
            Read(std::filesystem::path filePath, std::ios_base::openmode fileMode = std::ios::binary) {
            constexpr auto ContainerTypeSize = static_cast< std::size_t >(sizeof(ContainerType::value_type));

            auto mFile = std::ifstream { filePath.string(), fileMode };

            if (!mFile.is_open()) {
                NUTS_LOG_WIN(auto stdOutHandle = ::GetStdHandle(STD_OUTPUT_HANDLE));
                NUTS_LOG_WIN(::SetConsoleTextAttribute(stdOutHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY));
                NUTS_LOG(std::cout << "Failed to open file: " << filePath.string() << "!\n");
                // TODO: Better manage exceptions
                throw std::exception {};
            }

            mFile.seekg(0, std::ios::end);
            auto fileSize = static_cast< std::size_t >(mFile.tellg());

            if (fileSize == 0) {
                NUTS_LOG_WIN(auto stdOutHandle = ::GetStdHandle(STD_OUTPUT_HANDLE));
                NUTS_LOG_WIN(::SetConsoleTextAttribute(stdOutHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY));
                NUTS_LOG(std::cout << "Opened an empty file: " << filePath.string() << "!\n");
                // TODO: Better manage exceptions
                throw std::exception {};
            }

            ContainerType container {};
            container.resize(fileSize / ContainerTypeSize);

            mFile.seekg(0, std::ios::beg);
            mFile.read(reinterpret_cast< decltype(mFile)::char_type* >(container.data()), fileSize);
            mFile.close();

            return std::move(container);
        }

        template < class ContainerType >
        requires(has_data_function< ContainerType >&& std::is_default_constructible_v< ContainerType >&& has_resize_function< ContainerType >&&
                     std::integral< typename ContainerType::value_type >) static ContainerType
            ReadImage(std::filesystem::path filePath, ImageLoadFormat imgFmt = ImageLoadFormat::RGBA, int* outTexWidth = nullptr, int* outTexHeight = nullptr,
                      int* outTexChannel = nullptr) {
            constexpr auto ContainerTypeSize = static_cast< std::size_t >(sizeof(ContainerType::value_type));

            int  texWidth, texHeight, texChannels;
            auto pixels = stbi_load(filePath.string().c_str(), &texWidth, &texHeight, &texChannels, static_cast< int >(imgFmt));
            if (!pixels) {
                NUTS_LOG_WIN(auto stdOutHandle = ::GetStdHandle(STD_OUTPUT_HANDLE));
                NUTS_LOG_WIN(::SetConsoleTextAttribute(stdOutHandle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY));
                NUTS_LOG(std::cout << "Failed to open an image: " << filePath.string() << "!\n");
                // TODO: Better manage exceptions
                throw std::exception {};
            }

            if (outTexWidth) { *outTexWidth = texWidth; }
            if (outTexHeight) { *outTexHeight = texHeight; }
            if (outTexChannel) { *outTexChannel = texChannels; }

            ContainerType container {};
            if (imgFmt == ImageLoadFormat::Default) {
                container.resize((static_cast< unsigned long long >(texWidth) * texHeight * texChannels) / ContainerTypeSize);
            } else {
                container.resize((static_cast< unsigned long long >(texWidth) * texHeight * static_cast< int >(imgFmt)) / ContainerTypeSize);
            }

            std::memcpy(container.data(), pixels, container.sizeInBytes());
            stbi_image_free(pixels);

            return std::move(container);
        }
    };

} // namespace nuts