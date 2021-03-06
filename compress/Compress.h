//
// Created by up duan on 15/5/15.
//

#ifndef RESTDATA_COMPRESS_H
#define RESTDATA_COMPRESS_H

#include <iosfwd>

class Compressor {
public:
    Compressor() = default;
    virtual ~Compressor() = default;
    virtual std::string Compress(std::string const& data) = 0;
    virtual std::string Uncompress(std::string const& data) = 0;
};

class GzipCompressor : public Compressor {
public:
    std::string Compress(std::string const& data) override;
    std::string Uncompress(std::string const& data) override;
};

class Bzip2Compressor : public Compressor {
public:
    std::string Compress(std::string const& data) override;
    std::string Uncompress(std::string const& data) override;
};

class LzmaCompressor : public Compressor {
public:
    std::string Compress(std::string const& data) override;
    std::string Uncompress(std::string const& data) override;
};

#endif //RESTDATA_COMPRESS_H
