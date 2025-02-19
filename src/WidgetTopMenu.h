#pragma once
#include <QWidget>
#include <QObject>
#include <WidgetApplicationLogic.h>
class WidgetTopMenu : public QWidget
{
    Q_OBJECT

public:
    WidgetTopMenu(WidgetApplicationLogic& logic);

private:
    WidgetApplicationLogic& widget_application_logic_;
};