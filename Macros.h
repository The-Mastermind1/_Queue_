#pragma once
#include<exception>
#include<string>
#define _NODISCARD [[nodiscard]]

#define _CONSTEXPR20 constexpr
#define _PANAGIOTIS_BEGIN namespace panagiotis{
#define _PANAGIOTIS_END }

_PANAGIOTIS_BEGIN

class pop_from_an_empty_queue_:public std::exception 
{
private:
    std::string errorMessage; // To store the error message
public:
    //Constructor to initialize the error message
    explicit pop_from_an_empty_queue_(const std::string& message)
        : errorMessage(message) {
    }

    // Override the what() method
    const char* what() const noexcept override {
        return errorMessage.c_str();
    }
};

class tried_to_access_an_empty_queue_ :public std::exception
{
private:
    std::string errorMessage; // To store the error message
public:
    //Constructor to initialize the error message
    explicit tried_to_access_an_empty_queue_(const std::string& message)
        : errorMessage(message) {
    }

    // Override the what() method
    const char* what() const noexcept override {
        return errorMessage.c_str();
    }
};


_PANAGIOTIS_END
