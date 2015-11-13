#ifndef __LAYOUTSCROLLSELECT_H__
#define __LAYOUTSCROLLSELECT_H__

#include "Layout.h"
#include <vector>

typedef std::vector<std::string> Items;

class LayoutScrollSelect : public Layout {
    public:
        LayoutScrollSelect(DOGS102* lcd, Items items, std::string info1 = "", std::string info2 = "");
        ~LayoutScrollSelect();

        void display();

        void scroll();
        std::string select();

    private:
        void increment(size_t& index);
        void decrement(size_t& index);

        Label _lSw1;
        Label _lSw2;
        Label _lInfo1;
        Label _lInfo2;
        Label _lCursor;

        Field _fItem1;
        Field _fItem2;
        Field _fItem3;
        Field _fItem4;
        Field _fItem5;

        Items _items;
        size_t _selected;
        size_t _size;
};

#endif
