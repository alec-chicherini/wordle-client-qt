#include <WidgetApplicationLogic.h>

WidgetApplicationLogic::WidgetApplicationLogic()
{
    
};

void WidgetApplicationLogic::GoTo(State state)
{
    emit StateChanged(state);
    current_state_ = state;
};
