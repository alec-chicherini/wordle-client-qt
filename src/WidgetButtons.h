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

class WidgetButtons : public QWidget {
  Q_OBJECT

 public:
  WidgetButtons(GameState& state);

 private:
  GameState& game_state_;
  std::array<std::array<QPushButton*, kColsNum>, kRowsNum> m_btns;
};
