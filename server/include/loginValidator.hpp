#pragma once

#include <string>

class LoginValidatorInterface
{
public:
    virtual bool validate(const std::string& login, const std::string& password) const = 0;
    virtual ~LoginValidatorInterface() = default;

};

class ExampleLoginValidator : public LoginValidatorInterface
{
public:
    bool validate(const std::string& login, const std::string& password) const override{
        return true;
    }
};
