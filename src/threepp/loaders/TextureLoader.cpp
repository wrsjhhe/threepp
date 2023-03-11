
#include "threepp/loaders/TextureLoader.hpp"

#include "threepp/utils/URLFetcher.hpp"

#include <iostream>
#include <regex>
#include <vector>

using namespace threepp;

namespace {

    bool checkIsJPEG(const std::string& path) {

        static std::regex reg(".*jpe?g", std::regex::icase);

        return std::regex_match(path, reg);
    }

}


[[deprecated("Function 'loadTexture' deprecated. Use 'load'")]] std::shared_ptr<Texture> TextureLoader::loadTexture(const std::filesystem::path& path, bool flipY) {

    std::cerr << "[TextureLoader] Function 'loadTexture' deprecated. Use 'load' instead" << std::endl;

    return load(path, flipY);
}

std::shared_ptr<Texture> TextureLoader::load(const std::filesystem::path& path, bool flipY) {

    if (useCache && cache_.count(path.string())) {
        auto cached = cache_[path.string()];
        if (!cached.expired()) {
            auto tex = cached.lock();
            return tex;
        } else {
            cache_.erase(path.string());
        }
    }

    if (!std::filesystem::exists(path)) {
        std::cerr << "[TextureLoader] No such file: '" << absolute(path).string() << "'!" << std::endl;
        return nullptr;
    }

    bool isJPEG = checkIsJPEG(path.string());

    auto image = imageLoader_.load(path, isJPEG ? 3 : 4, flipY);

    auto texture = Texture::create(image);
    texture->name = path.stem().string();

    texture->format = isJPEG ? RGBFormat : RGBAFormat;
    texture->needsUpdate();

    if (useCache) cache_[path.string()] = texture;

    return texture;
}

std::shared_ptr<Texture> TextureLoader::loadFromUrl(const std::string& url, bool flipY) {

    if (useCache && cache_.count(url)) {
        auto cached = cache_[url];
        if (!cached.expired()) {
            auto tex = cached.lock();
            return tex;
        } else {
            cache_.erase(url);
        }
    }

    bool isJPEG = checkIsJPEG(url);

    std::vector<unsigned char> stream;

    utils::UrlFetcher urlFetcher;
    bool res = urlFetcher.fetch(url, stream);

    if (res && !stream.empty()) {
        auto image = imageLoader_.load(stream);
        auto texture = Texture::create(image);

        texture->format = isJPEG ? RGBFormat : RGBAFormat;
        texture->needsUpdate();
        if (useCache) cache_[url] = texture;

        return texture;
    } else {

        std::cerr << "[TextureLoader] Failed loading texture from URL: " << url << std::endl;

        return nullptr;
    }

}

void TextureLoader::clearCache() {

    cache_.clear();
}
