#pragma once
#include <QWidget>
#include <QVBoxLayout>
#include <QObject>
#include <GameState.h>
#include <WidgetKeyboard.h>
#include <WidgetButtons.h>

class WidgetGame : public QWidget
{
Q_OBJECT

public:
    WidgetGame();

private:
    GameState m_state;

protected:
    virtual void keyPressEvent(QKeyEvent* event) override;
};


