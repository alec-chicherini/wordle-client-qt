#pragma once
#include <QDebug>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QObject>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

#include <vector>

#include <GameState.h>

class WidgetKeyboard : public QWidget {
  Q_OBJECT

 public:
  WidgetKeyboard(GameState& state);

 private:
  GameState& game_state_;
};
