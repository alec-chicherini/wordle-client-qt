#pragma once
#include <QPushButton>
#include <QGridLayout>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDebug>
#include <QObject>

#include <vector>

#include <GameState.h>

class WidgetButtons : public QWidget
{
Q_OBJECT

public:
    WidgetButtons(GameState& state);

private:
    GameState& m_state;
    std::array<std::array<QPushButton*,COLS_NUM>,ROWS_NUM> m_btns;
};
