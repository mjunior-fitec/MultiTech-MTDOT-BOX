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
    int read;
    int read_size;

    file = _dot->openUserFile(file_name, mDot::FM_RDONLY);
    if (file.fd < 0) {
        setErrorMessage("Failed to open file");
        return 1;
    }

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
