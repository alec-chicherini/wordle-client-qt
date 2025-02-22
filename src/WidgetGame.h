#pragma once
#include <APIApplicationLogic.h>
#include <GameState.h>
#include <WidgetApplicationLogic.h>
#include <WidgetButtons.h>
#include <WidgetKeyboard.h>
#include <QObject>
#include <QVBoxLayout>
#include <QWidget>

class WidgetGame : public QWidget {
  Q_OBJECT

 public:
  WidgetGame(WidgetApplicationLogic& widget_logic,
             APIApplicationLogic& api_logic);

 private:
  GameState game_state_;
  WidgetApplicationLogic& widget_application_logic_;
  APIApplicationLogic& api_application_logic_;

 protected:
  virtual void keyPressEvent(QKeyEvent* event) override;
};