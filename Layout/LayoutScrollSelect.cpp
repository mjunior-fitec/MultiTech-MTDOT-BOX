#include "LayoutScrollSelect.h"
#include "MTSLog.h"

LayoutScrollSelect::LayoutScrollSelect(DOGS102* lcd, Items items, std::string info1, std::string info2)
  : Layout(lcd),
    _lSw1(0, 7, "Scroll"),
    _lSw2(11, 7, "Select"),
    _lInfo1(0, 0, info1),
    _lInfo2(0, 1, info2),
    _lCursor(0, 4, "=>"),
    _fItem1(3, 2, 14),
    _fItem2(3, 3, 14),
    _fItem3(3, 4, 14),
    _fItem4(3, 5, 14),
    _fItem5(3, 6, 14),
    _items(items)
{
    _size = _items.size();
    _selected = 0;
}

LayoutScrollSelect::~LayoutScrollSelect() {}

void LayoutScrollSelect::display() {
    clear();
    startUpdate();

    writeLabel(_lSw1);
    writeLabel(_lSw2);
    writeLabel(_lInfo1);
    writeLabel(_lInfo2);
    writeLabel(_lCursor);

    switch (_size) {
        case 0:
            // special case - no items
            //    (empty)
            //    (empty)
            // => (empty)
            //    (empty)
            //    (empty)
            break;

        case 1:
            // special case - 1 item
            //    (empty)
            //    (empty)
            // => item1
            //    (empty)
            //    (empty)
            writeField(_fItem3, _items[0]);
            break;

        case 2:
            // special case - 2 items
            //    (empty)
            //    (empty)
            // => item1
            //    item2
            //    (empty)
            writeField(_fItem3, _items[0]);
            writeField(_fItem4, _items[1]);
            break;

        case 3:
            // special case - 3 items
            //    (empty)
            //    item3
            // => item1
            //    item2
            //    (empty)
            writeField(_fItem2, _items[2]);
            writeField(_fItem3, _items[0]);
            writeField(_fItem4, _items[1]);
            break;

        case 4:
            // special case - 4 items
            //    item3
            //    item4
            // => item1
            //    item2
            //    (empty)
            writeField(_fItem1, _items[2]);
            writeField(_fItem2, _items[3]);
            writeField(_fItem3, _items[0]);
            writeField(_fItem4, _items[1]);
            break;

        default:
            // this is the generic case - should handle lists of 5+ items correctly
            //    item4         item6           item9
            //    item5         item7           item10
            // => item1      => item1        => item1
            //    item2         item2           item2
            //    item3         item3           item3
            writeField(_fItem1, _items[_size - 2]);
            writeField(_fItem2, _items[_size - 1]);
            writeField(_fItem3, _items[0]);
            writeField(_fItem4, _items[1]);
            writeField(_fItem5, _items[2]);
            break;
    }

    endUpdate();
}

void LayoutScrollSelect::scroll() {
    size_t index;

    switch (_size) {
        case 0:
        case 1:
            // nothing to scroll
            break;

        case 2:
            // special case - 2 items
            //    (empty)  ->     (empty)
            //    (empty)  ->     (empty)
            // => item1    ->  => item2
            //    item2    ->     item1
            //    (empty)  ->     (empty)

            index = _selected;
            // keep selected item up to date
            increment(_selected);

            startUpdate();
            // previously selected item moves down to field4
            writeField(_fItem4, _items[index]);
            increment(index);
            // other item moves up to field3
            writeField(_fItem3, _items[index]);
            endUpdate();
            break;

        case 3:
            // special case - 3 items
            //    (empty)  ->     (empty)
            //    item3    ->     item1  
            // => item1    ->  => item2
            //    item2    ->     item3
            //    (empty)  ->     (empty)
            
            index = _selected;
            // keep selected item up to date
            increment(_selected);

            startUpdate();
            // previously selected item moves up to field2
            writeField(_fItem2, _items[index]);
            increment(index);
            // new selected item moves up to field3
            writeField(_fItem3, _items[index]);
            increment(index);
            // item from field2 moves down to field4
            writeField(_fItem4, _items[index]);
            endUpdate();
            break;

        case 4:
            // special case - 4 items
            //    item3    ->     item4  
            //    item4    ->     item1  
            // => item1    ->  => item2
            //    item2    ->     item3
            //    (empty)  ->     (empty)
            
            index = _selected;
            // keep selected item up to date
            increment(_selected);

            startUpdate();
            // previously selected item moves up to field2
            writeField(_fItem2, _items[index]);
            increment(index);
            // new selected item moves up to field3
            writeField(_fItem3, _items[index]);
            increment(index);
            // item from field1 moves down to field4
            writeField(_fItem4, _items[index]);
            increment(index);
            // item from field2 moves up to field1
            writeField(_fItem1, _items[index]);
            endUpdate();
            break;

        default:
            // this is the generic case - should handle lists of 5+ items correctly
            //    item4    ->     item5           item6    ->     item7
            //    item5    ->     item1           item7    ->     item1
            // => item1    ->  => item2        => item1    ->  => item2
            //    item2    ->     item3           item2    ->     item3
            //    item3    ->     item4           item3    ->     item4
            
            index = _selected;
            // keep selected item up to date
            increment(_selected);

            startUpdate();
            decrement(index);
            // item from field2 moves up to field1
            writeField(_fItem1, _items[index]);
            increment(index);
            // previously selected item moves up to field2
            writeField(_fItem2, _items[index]);
            increment(index);
            // new selected item moves up to field3
            writeField(_fItem3, _items[index]);
            increment(index);
            // item from field5 moves up to field4
            writeField(_fItem4, _items[index]);
            increment(index);
            // next item (from field1 or off screen) moves up to field5
            writeField(_fItem5, _items[index]);
            endUpdate();
            break;
      }
}

std::string LayoutScrollSelect::select() {
    std::string selected = "";
    if (_size > 0)
        selected = _items[_selected];

    return selected;
}

void LayoutScrollSelect::increment(size_t& index) {
    if (_size > 1) {
        index++;
        index %= _size;
    }
}

void LayoutScrollSelect::decrement(size_t& index) {
    if (_size > 1) {
        if (index == 0)
            index = _size - 1;
        else
            index--;
    }
}
