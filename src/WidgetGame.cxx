#include <QtHelper.h>
#include <WidgetGame.h>
#include <QApplication>
#include <QDebug>
#include <QKeyEvent>
#include <QMessageBox>
WidgetGame::WidgetGame(WidgetApplicationLogic& widget_logic,
                       APIApplicationLogic& api_logic)
    : game_state_(api_logic),
      widget_application_logic_(widget_logic),
      api_application_logic_(api_logic) {
  [[maybe_unused]] bool connected;
  WidgetKeyboard* widgetKeyboard = new WidgetKeyboard(game_state_);
  WidgetButtons* widgetButtons = new WidgetButtons(game_state_);

  QVBoxLayout* qVBoxLayout = new QVBoxLayout(this);
  qVBoxLayout->addWidget(widgetButtons);
  qVBoxLayout->addWidget(widgetKeyboard);

  connected = QObject::connect(&game_state_, &GameState::SignalMsgBox, this,
                               [=, this](QString msg) {
                                 QMessageBox msgBox(this);
                                 msgBox.setText(msg);
                                 msgBox.exec();
                               });
  IS_CONENCTED_OK
  connected = QObject::connect(&game_state_, &GameState::SignalQuitOrRestart,
                               this, [=, this]() {
                                 QMessageBox msgBox(this);
                                 msgBox.setText(QString("Играть ещё?"));
                                 msgBox.addButton(QMessageBox::Yes);
                                 msgBox.addButton(QMessageBox::No);
                                 int result = msgBox.exec();
                                 if (result == QMessageBox::No) {
                                   qApp->exit(1);
                                 } else {
                                   game_state_.Reset();
                                 }
                               });
  IS_CONENCTED_OK
  connected = QObject::connect(
      &widget_application_logic_, &WidgetApplicationLogic::StateChanged,
      &game_state_,
      [=, this](WidgetApplicationLogic::State state_previous,
                WidgetApplicationLogic::State state_now) {
        if (state_previous == WidgetApplicationLogic::State::kAuthorization)
          game_state_.Reset();
      });
  IS_CONENCTED_OK
};

void WidgetGame::keyPressEvent(QKeyEvent* event) {
  quint32 code = event->nativeScanCode();

  QString charToSend;
  bool needToSend = true;
  if (code == 36) {
    charToSend = "Enter";
  } else if (code == 22) {
    charToSend = "<-";
  } else if (code >= 24 && code <= 35) {
    QString row1("ЙЦУКЕНГШЩЗХЪ");
    charToSend = QString(row1[code - 24]);
  } else if (code >= 38 && code <= 48) {
    QString row2("ФЫВАПРОЛДЖЭ");
    charToSend = QString(row2[code - 38]);
  } else if (code >= 52 && code <= 60) {
    QString row3("ЯЧСМИТЬБЮ");
    charToSend = QString(row3[code - 52]);
  } else {
    needToSend = false;
  }

  if (needToSend) {
    game_state_.InputChar(charToSend);
  }
};
