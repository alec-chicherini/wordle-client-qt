#pragma once
#include <QWidget>
#include <QObject>
#include <WidgetApplicationLogic.h>
#include <map>
class WidgetApplication : public QWidget
{
Q_OBJECT

public:
    WidgetApplication();

private:
    WidgetApplicationLogic widget_application_logic_;
    std::map<WidgetApplicationLogic::State, QWidget*> map_state_widget_;
};


