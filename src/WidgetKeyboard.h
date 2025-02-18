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

class WidgetKeyboard : public QWidget
{
Q_OBJECT

public:
    WidgetKeyboard(GameState& state);

private:
    GameState& m_state;
};


