#pragma once

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

class chat_message
{
public:
    enum
    {
        header_length = 1
    };
    enum
    {
        max_body_length = 512
    };

    chat_message() : body_length_(0) {}

    const char* data() const { return data_; }

    char* data() { return data_; }

    std::size_t length() const { return header_length + body_length_; }

    const char* body() const { return data_ + header_length; }

    char* body() { return data_ + header_length; }

    std::size_t body_length() const { return body_length_; }

    void body_length(std::size_t new_length)
    {
        body_length_ = new_length;
        if (body_length_ > max_body_length) body_length_ = max_body_length;
    }

    bool decode_header()
    {
        char header[header_length + 1] = "";
        strncat_s(header, data_, header_length);
        body_length_ = static_cast<unsigned>(std::atoi(header));
        if (body_length_ > max_body_length)
        {
            body_length_ = 0;
            return false;
        }
        return true;
    }

    void encode_header()
    {
        char header[header_length + 1] = "";
        std::sprintf(header, "%0d", static_cast<int>(body_length_));
        std::cout << header << std::endl;
        std::cout << "HEADER1" << std::endl;
        std::memcpy(data_, header, header_length);
        std::cout << "HEADER2" << std::endl;
    }

private:
    char data_[header_length + max_body_length];
    std::size_t body_length_;
};
