#include <WidgetTopMenu.h>

#include <QDebug>
#include <QFrame>
#include <QHBoxLayout>
#include <QPushButton>

#include <QtHelper.h>
WidgetTopMenu::WidgetTopMenu(WidgetApplicationLogic& logic)
    : widget_application_logic_(logic) {
  [[maybe_unused]] bool connected;
  QHBoxLayout* q_hbox_layout_menu = new QHBoxLayout(this);
  q_hbox_layout_menu->setAlignment(Qt::AlignCenter);
  auto GetVLine = [] {
    QFrame* line = new QFrame();
    line->setFrameShape(QFrame::VLine);
    line->setFrameShadow(QFrame::Sunken);
    line->setLineWidth(2);
    return line;
  };

  QPushButton* btn_play = new QPushButton("Играть");
  connected = QObject::connect(
      btn_play, &QPushButton::clicked, &widget_application_logic_, [=, this]() {
        widget_application_logic_.GoTo(WidgetApplicationLogic::State::kPlay);
      });
  IS_CONENCTED_OK
  q_hbox_layout_menu->addWidget(btn_play);

  q_hbox_layout_menu->addWidget(GetVLine());

  QPushButton* btn_menu = new QPushButton("Меню");
  connected = QObject::connect(
      btn_menu, &QPushButton::clicked, &widget_application_logic_, [=, this]() {
        widget_application_logic_.GoTo(WidgetApplicationLogic::State::kMenu);
      });
  IS_CONENCTED_OK
  q_hbox_layout_menu->addWidget(btn_menu);

  q_hbox_layout_menu->addWidget(GetVLine());

  QPushButton* btn_log_out = new QPushButton("Выход");
  connected =
      QObject::connect(btn_log_out, &QPushButton::clicked,
                       &widget_application_logic_, [=, this]() {
                         widget_application_logic_.GoTo(
                             WidgetApplicationLogic::State::kAuthorization);
                       });
  IS_CONENCTED_OK
  q_hbox_layout_menu->addWidget(btn_log_out);

  connected = QObject::connect(
      &widget_application_logic_, &WidgetApplicationLogic::StateChanged, this,
      [=, this](WidgetApplicationLogic::State state_previous,
                WidgetApplicationLogic::State state_now) {
        if (state_now == WidgetApplicationLogic::State::kAuthorization) {
          setVisible(false);
        } else {
          setVisible(true);
        }
      });
  IS_CONENCTED_OK
};