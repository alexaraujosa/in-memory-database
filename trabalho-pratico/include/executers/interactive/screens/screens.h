#ifndef TP_EXECUTERS_INTERACTIVE_SCREENS_H
#define TP_EXECUTERS_INTERACTIVE_SCREENS_H

#include "executers/interactive/interactive_common.h"

typedef void(*ScreenDrawFunction)(GM_Term, FrameStore);

ScreenDrawFunction manage_screen(ScreenId id, GM_Term term, FrameStore store);

void destroy_screens(GM_Term term, FrameStore store);

Keypress_Code keypress_screen(ScreenId id, GM_Term term, FrameStore store, GM_Key key);

#endif