#ifndef __LAYOUTCONFIG_H__
#define __LAYOUTCONFIG_H__

#include "Layout.h"

class LayoutConfig : public Layout {
    public:
        LayoutConfig(DOGS102* lcd);
        ~LayoutConfig();

        void display();
        void roll();

    private:
        Label _lMode;
        Field _fHeader;
        Field _fMsg1;
        Field _fMsg2;
        Field _fMsg3;
		Timer _timer;		
		uint8_t _screen;
		uint32_t _duration;
	    void update1of3();
	    void update2of3();
		void update3of3();
};

#endif
