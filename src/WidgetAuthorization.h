#pragma once
#include <QWidget>
#include <QObject>
#include <WidgetApplicationLogic.h>
class WidgetAuthorization : public QWidget
{
    Q_OBJECT

public:
    WidgetAuthorization(WidgetApplicationLogic& logic);

private:
    WidgetApplicationLogic& widget_application_logic_;
};