#pragma once
#include <QWidget>
#include <QObject>
#include <WidgetApplicationLogic.h>
class WidgetMenu : public QWidget
{
    Q_OBJECT

public:
    WidgetMenu(WidgetApplicationLogic& logic);

private:
    WidgetApplicationLogic& widget_application_logic_;
};