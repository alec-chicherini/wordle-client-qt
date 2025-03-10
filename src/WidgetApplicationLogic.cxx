#include <WidgetApplicationLogic.h>

WidgetApplicationLogic::WidgetApplicationLogic() {

};

void WidgetApplicationLogic::GoTo(State state) {
  if (state == WidgetApplicationLogic::State::kProfile ||
      state == WidgetApplicationLogic::State::kStatistic) {
    return;
  }
  emit StateChanged(current_state_, state);
  current_state_ = state;
};
