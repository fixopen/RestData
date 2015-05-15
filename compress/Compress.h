//
// Created by up duan on 15/5/15.
//

#ifndef RESTDATA_COMPRESS_H
#define RESTDATA_COMPRESS_H


#include <iosfwd>

class Compressor {
public:
    Compressor() = default;
    virtual ~Compressor();
    virtual std::string Compress(std::string const& data) = 0;
    virtual std::string Uncompress(std::string const& data) = 0;
};

class GzipCompressor : public Compressor {
public:
    virtual std::string Compress(std::string const& data) override;
    virtual std::string Uncompress(std::string const& data) override;
};

#endif //RESTDATA_COMPRESS_H
