/* Copyright (c) <2016> <MultiTech Systems>, MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
 * and associated documentation files (the "Software"), to deal in the Software without restriction, 
 * including without limitation the rights to use, copy, modify, merge, publish, distribute, 
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is 
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or 
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING 
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/**
 ******************************************************************************
 * File Name          : Command.h
 * Date               : 18/04/2014 10:57:12
 * Description        : This file provides code for command line prompt
 ******************************************************************************
 *
 * COPYRIGHT(c) 2014 MultiTech Systems, Inc.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */

#include "mbed.h"
#include "mDot.h"
#include "MTSSerial.h"
#include "MTSText.h"
#include <cstdlib>
#include <string>
#include <vector>
#include "limits.h"

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __command_H
#define __command_H

#define KEY_LENGTH 16
#define EUI_LENGTH 8
#define PASSPHRASE_LENGTH 128

class Command {

    public:

        Command(mDot* dot);
        Command(mDot* dot, const char* name, const char* text, const char* desc);
        virtual ~Command() {};

        const char* name() const { return _name; };
        const char* text() const { return _text; };
        const char* desc() const { return _desc; };
        const char* help() const { return _help.c_str(); };

        virtual uint32_t action(std::vector<std::string> args) = 0;
        virtual bool verify(std::vector<std::string> args);
        const std::string usage() const;
        std::string& errorMessage();
        const bool queryable();

        static const char newline[];
        static void readByteArray(const std::string& input, std::vector<uint8_t>& out, size_t len);

        static bool isHexString(const std::string& str, size_t bytes);
        static bool isBaudRate(uint32_t baud);

    protected:

        void setErrorMessage(const char* message);
        void setErrorMessage(const std::string& message);
        std::string _help;
        std::string _usage;
        bool _queryable;
        mDot* _dot;

    private:

        const char* _name;
        const char* _text;
        const char* _desc;
        std::string _errorMessage;

};

#endif /*__ command_H */

/************************ (C) COPYRIGHT MultiTech Systems, Inc *****END OF FILE****/
