#include <WidgetMenu.h>

#include <QDebug>
#include <QPushButton>
#include <QVBoxLayout>

#include <QtHelper.h>

WidgetMenu::WidgetMenu(WidgetApplicationLogic& logic)
    : widget_application_logic_(logic) {
  [[maybe_unused]] bool connected;
  QVBoxLayout* q_vbox_layout_menu = new QVBoxLayout(this);
  q_vbox_layout_menu->setAlignment(Qt::AlignCenter);

  QPushButton* btn_play = new QPushButton("Играть");
  q_vbox_layout_menu->addWidget(btn_play);
  connected = QObject::connect(
      btn_play, &QPushButton::clicked, &widget_application_logic_, [=, this]() {
        widget_application_logic_.GoTo(WidgetApplicationLogic::State::kPlay);
      });
  IS_CONENCTED_OK

  QPushButton* btn_statistic = new QPushButton("Статистика");
  q_vbox_layout_menu->addWidget(btn_statistic);
  connected = QObject::connect(btn_statistic, &QPushButton::clicked,
                               &widget_application_logic_, [=, this]() {
                                 widget_application_logic_.GoTo(
                                     WidgetApplicationLogic::State::kStatistic);
                               });
  IS_CONENCTED_OK

  QPushButton* btn_profile = new QPushButton("Профиль");
  q_vbox_layout_menu->addWidget(btn_profile);
  connected = QObject::connect(
      btn_profile, &QPushButton::clicked, &widget_application_logic_,
      [=, this]() {
        widget_application_logic_.GoTo(WidgetApplicationLogic::State::kProfile);
      });
  IS_CONENCTED_OK

  QPushButton* btn_log_out = new QPushButton("Выход");
  q_vbox_layout_menu->addWidget(btn_log_out);
  connected = QObject::connect(
      btn_log_out, &QPushButton::clicked, &widget_application_logic_,
      [=, this]() {
        widget_application_logic_.GoTo(WidgetApplicationLogic::State::kAuthorization);
      });
  IS_CONENCTED_OK
};