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

#include "ModeData.h"
#include "MTSText.h"

ModeData::ModeData(DOGS102* lcd, ButtonHandler* _buttons, mDot* _dot, LoRaHandler* lora, GPSPARSER* gps, SensorHandler* sensors)
    :Mode(lcd,_buttons,_dot,lora,gps,sensors),
     _data(lcd),
     _help(lcd),
     _buf_size(ONELINEMAX)
{}

ModeData::~ModeData() {}

bool ModeData::checkFile()
{
    bool exists = false;
    //get all files and see if file exists
    vector<mDot::mdot_file> files = _dot->listUserFiles();
    for (vector<mDot::mdot_file>::iterator it = files.begin(); it != files.end(); it++) {
        if (strcmp(file_name,it->name)==0) {
            exists = true;
            break;
        }
    }
    //if file doesnt exist exit to main menu
    if(!exists) {
        _data.noData();
        osDelay(4000);
        return true;
    }
    _file = _dot->openUserFile(file_name, mDot::FM_RDONLY);
    //if nothing is in file exit to main menu
    if (_file.fd < 0) {
        _data.errorData();
        osDelay(4000);
        _dot->closeUserFile(_file);
        return true;
    }
    return false;
}

bool ModeData::start()
{
    if(checkFile())
        return true;
    _help.display();
    osDelay(3000);
    readFile();
}

void ModeData::displayData()
{
    std::vector<std::string> data = mts::Text::split(_str, ',');
    _line.id = data.at(0);
    _line.status = data.at(1);
    _line.lock = data.at(2);
    _line.lat = data.at(3);
    _line.lng = data.at(4);
    _line.alt = data.at(5);
    _line.time = data.at(6);
    _line.gateways = data.at(7);
    _line.margin = data.at(8);
    _line.rssiD = data.at(9);
    _line.snrD = data.at(10);
    _line.dataRate = data.at(11);
    _line.power = data.at(12);
    _data.updateAll(_line);
}

//get the current line out of the buffer into str
void ModeData::getLine()
{
    _prev = 0;
    _indexUpdate = 0;
    _str = "";
    _dot->readUserFile(_file, (void*)_buf, _buf_size);
    //only gets called when going back
    if(_last) {
        //-3 puts it back to one before new line
        _indexUpdate = _buf_size - 3;
        //check from back of buffer for new line
        while(_buf[_indexUpdate]!='\n'&&_indexUpdate>=0) _indexUpdate--;
        _indexUpdate++;
    }
    //go from indexUpdate to new line to ge the line
    //prev keeps track of how long line read was
    while(_buf[_indexUpdate]!='\n') {
        _str += _buf[_indexUpdate];
        _indexUpdate++;
        _prev++;
    }
    //push index past newline
    _index += _indexUpdate+1;
    displayData();
}

void ModeData::back()
{
    if(_index>=(_buf_size+_prev)) {
        _index -= (_prev+_buf_size);
    } else {
        //special case for beginning of file
        if(_index>0) _buf_size = _index-1;
        _buf_size -= _prev;
        _index = 0;
    }
    _last = true;
    _dot->seekUserFile(_file,_index, SEEK_SET);
    getLine();
}

void ModeData::forward()
{
    _last = false;
    if(_index<_file.size) {
        _buf_size = ONELINEMAX;
        _dot->seekUserFile(_file, _index, SEEK_SET);
        getLine();
    }
}

//update switch labels
void ModeData::configSw()
{
    if(_index-(_prev+1)<=0) _data.updateSw2("");
    else _data.updateSw2("Back");
    if(_index<_file.size) _data.updateSw1("Next");
    else _data.updateSw1("");
}


bool ModeData::readFile()
{
    _index = 0;
    _last = false;
    _prev = 0;
    _indexUpdate = 0;
    //called to start on page one
    forward();
    configSw();
    ButtonHandler::ButtonEvent be;

    while (true) {

        be = _buttons->getButtonEvent();

        switch(be) {
            case ButtonHandler::sw1_press:
                if(_index!=_file.size) {
                    forward();
                    configSw();}
                break;

            case ButtonHandler::sw2_press:
                if(_index-(_prev+1)>0) {
                    back();
                    configSw();}
                break;

            case ButtonHandler::sw1_hold:
                _dot->closeUserFile(_file);
                return true;

            default:
                break;
        }
    }
}


