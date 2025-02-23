#pragma once
#include <QDebug>
#include <QObject>

class WidgetApplicationLogic : public QObject {
  Q_OBJECT

 public:
  WidgetApplicationLogic();
  enum class State {
    kNone = 0,
    kAuthorization,
    kMenu,
    kPlay,
    kStatistic,
    kProfile
  };

 public slots:
  void GoTo(State state);

 signals:
  void StateChanged(State state_previous, State state_now);

 private:
  State current_state_;
};
