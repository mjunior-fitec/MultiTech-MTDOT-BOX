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

#include "CmdGetSurveyDataFile.h"
#include "FileName.h"
#include "MTSLog.h"

CmdGetSurveyDataFile::CmdGetSurveyDataFile(mDot* dot, mts::MTSSerial& serial) :
        Command(dot, "Get Survey Data File", "AT+GSDF", "Outputs the survey data file contents to the command port"), _serial(serial)
{
    _help = std::string(text()) + ": " + std::string(desc());
}

uint32_t CmdGetSurveyDataFile::action(std::vector<std::string> args)
{
    mDot::mdot_file file;
    int buf_size = 512;
    char buf[buf_size];
    int read = 0;
    int read_size = 0;

    file = _dot->openUserFile(file_name, mDot::FM_RDONLY);
    if (file.fd < 0) {
        setErrorMessage("Failed to open file");
        return 1;
    }

    // print header line with column labels
    _serial.writef("ID,Status,Lock,Lat,Long,Alt,Time,Gateways,Margin,RSSIdown,SNRdown,DataRate,Power\r\n");

    while (read < file.size) {
        read_size = (file.size - read) > buf_size ? buf_size : file.size - read;
        int size = _dot->readUserFile(file, (void*)buf, read_size);
        if (size < 0) {
            setErrorMessage("Failed to read file");
            _dot->closeUserFile(file);
            return 1;
        }

        for (int i = 0; i < size; i++) {
            if (buf[i] == '\n')
                _serial.writef("\r\n");
            else
                _serial.writef("%c", buf[i]);
        }

        read += size;
    }

    _dot->closeUserFile(file);
    
    return 0;
}
