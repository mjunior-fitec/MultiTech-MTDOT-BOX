#include "Command.h"
#include <algorithm>

const char Command::newline[] = "\r\n";

Command::Command(mDot* dot) : _dot(dot)
{
    _usage = "NONE";
    _queryable = false;
}

Command::Command(mDot* dot, const char* name, const char* text, const char* desc) :
    _dot(dot), _name(name), _text(text), _desc(desc)
{
    _usage = "NONE";
    _queryable = false;
}

std::string &Command::errorMessage()
{
    return _errorMessage;
}

void Command::setErrorMessage(const char* message)
{
    _errorMessage.assign(message);
}

void Command::setErrorMessage(const std::string& message)
{
    _errorMessage.assign(message);
}

const std::string Command::usage() const
{
    std::string usage(_text);
    usage.append(": ");
    usage.append(_usage);
    return usage;
}

const bool Command::queryable()
{
    return _queryable;
}

void Command::readByteArray(const std::string& input, std::vector<uint8_t>& out, size_t len)
{
    // if input length is greater than expected byte output
    // there must be a delimiter included
    if (input.length() > len * 2)
    {
        std::vector < std::string > bytes;
        if (input.find(" ") != std::string::npos)
            bytes = mts::Text::split(input, " ");
        else if (input.find(":") != std::string::npos)
            bytes = mts::Text::split(input, ":");
        else if (input.find("-") != std::string::npos)
            bytes = mts::Text::split(input, "-");
        else if (input.find(".") != std::string::npos)
            bytes = mts::Text::split(input, ".");

        if (bytes.size() != len) {
            return;
        }

        uint8_t temp;
        // Read in the key components...
        for (size_t i = 0; i < len; i++)
        {
            sscanf(bytes[i].c_str(), "%02x", &temp);
            out.push_back(temp);
        }
    }
    else
    {
        // no delims
        uint8_t temp;

        // Read in the key components...
        for (size_t i = 0; i < len; i++)
        {
            if (i * 2 < input.size())
            {
                sscanf(input.substr(i * 2).c_str(), "%02x", &temp);
                out.push_back(temp);
            }
        }
    }
}

bool Command::isHexString(const std::string& str, size_t bytes) {
    size_t numDelims = bytes - 1;
    size_t minSize = bytes * 2;
    size_t maxSize = minSize + numDelims;

    if (str.size() == minSize) {
        return str.find_first_not_of("0123456789abcdefABCDEF") == std::string::npos;
    }
    else if (str.size() == maxSize) {
        if (str.find_first_of(":-.") == std::string::npos) {
            // no delim found
            return false;
        }
        if (str.find(":") != std::string::npos && std::count(str.begin(), str.end(), ':') != numDelims) {
            return false;
        }
        if (str.find(".") != std::string::npos && std::count(str.begin(), str.end(), '.') != numDelims) {
            return false;
        }
        if (str.find("-") != std::string::npos && std::count(str.begin(), str.end(), '-') != numDelims) {
            return false;
        }

        return str.find_first_not_of("0123456789abcdefABCDEF:-.") == std::string::npos;
    }   

    return false;
}

bool Command::verify(std::vector<std::string> args) {
    if (args.size() == 1)
        return true;

    setErrorMessage("Invalid arguments");
    return false;
}

