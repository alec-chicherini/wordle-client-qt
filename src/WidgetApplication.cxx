#include <WidgetApplication.h>
#include <WidgetAuthorization.h>
#include <WidgetGame.h>
#include <WidgetMenu.h>
#include <WidgetTopMenu.h>

#include <QApplication>
#include <QDebug>
#include <QMessageBox>
#include <QStackedWidget>

#include <QtHelper.h>

WidgetApplication::WidgetApplication() {
  [[maybe_unused]] bool connected;
  WidgetTopMenu* widget_top_menu = new WidgetTopMenu(widget_application_logic_);
  QStackedWidget* q_stacked_widget = new QStackedWidget();
  q_stacked_widget->layout()->setAlignment(Qt::AlignCenter);
  WidgetGame* widget_game =
      new WidgetGame(widget_application_logic_, api_application_logic_);
  WidgetAuthorization* widget_authorization = new WidgetAuthorization(
      widget_application_logic_, api_application_logic_);
  WidgetMenu* widget_menu = new WidgetMenu(widget_application_logic_);
  q_stacked_widget->addWidget(widget_authorization);
  q_stacked_widget->addWidget(widget_menu);
  q_stacked_widget->addWidget(widget_game);
  map_state_widget_[WidgetApplicationLogic::State::kAuthorization] =
      widget_authorization;
  map_state_widget_[WidgetApplicationLogic::State::kMenu] = widget_menu;
  map_state_widget_[WidgetApplicationLogic::State::kPlay] = widget_game;

  connected = QObject::connect(
      &widget_application_logic_, &WidgetApplicationLogic::StateChanged,
      q_stacked_widget,
      [=, this](WidgetApplicationLogic::State state_previous,
                WidgetApplicationLogic::State state_now) {
        q_stacked_widget->setCurrentWidget(map_state_widget_[state_now]);
      });
  IS_CONENCTED_OK

  QVBoxLayout* q_vbox_layout = new QVBoxLayout(this);
  q_vbox_layout->addWidget(widget_top_menu);
  q_vbox_layout->addWidget(q_stacked_widget);
  q_vbox_layout->setAlignment(Qt::AlignCenter);

  emit widget_application_logic_.StateChanged(
      WidgetApplicationLogic::State::kNone,
      WidgetApplicationLogic::State::kAuthorization);
};