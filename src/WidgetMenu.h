#pragma once
#include <WidgetApplicationLogic.h>
#include <QObject>
#include <QWidget>
class WidgetMenu : public QWidget {
  Q_OBJECT

 public:
  WidgetMenu(WidgetApplicationLogic& logic);

 private:
  WidgetApplicationLogic& widget_application_logic_;
};