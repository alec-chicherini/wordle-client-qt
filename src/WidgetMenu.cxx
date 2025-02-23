#include <WidgetMenu.h>

#include <QtHelper.h>
#include <QDebug>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>

WidgetMenu::WidgetMenu(WidgetApplicationLogic& logic)
    : widget_application_logic_(logic) {
  [[maybe_unused]] bool connected;

  QHBoxLayout* qHBoxLayoutAuthorizationTop = new QHBoxLayout(this);
  qHBoxLayoutAuthorizationTop->setAlignment(Qt::AlignCenter);
  QWidget* widget_aligned_center = new QWidget;
  widget_aligned_center->setMaximumWidth(330);
  widget_aligned_center->setSizePolicy(QSizePolicy::Expanding,
                                       QSizePolicy::Preferred);
  qHBoxLayoutAuthorizationTop->addWidget(widget_aligned_center);

  QVBoxLayout* q_vbox_layout_menu = new QVBoxLayout(widget_aligned_center);

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
  connected =
      QObject::connect(btn_log_out, &QPushButton::clicked,
                       &widget_application_logic_, [=, this]() {
                         widget_application_logic_.GoTo(
                             WidgetApplicationLogic::State::kAuthorization);
                       });
  IS_CONENCTED_OK
};