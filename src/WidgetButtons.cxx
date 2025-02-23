#include <QtHelper.h>
#include <WidgetButtons.h>

WidgetButtons::WidgetButtons(GameState& state) : game_state_(state) {
  [[maybe_unused]] bool connected;
  auto createBtnGameField = []() {
    QPushButton* btn;
    btn = new QPushButton(" ");
    btn->setFixedSize(60, 60);
    btn->setFocusPolicy(Qt::NoFocus);
    return btn;
  };

  QGridLayout* qGridLayout = new QGridLayout();
  qGridLayout->setSpacing(2);
  qGridLayout->setContentsMargins(0, 0, 0, 0);
  qGridLayout->setAlignment(Qt::AlignCenter);

  for (int i = 0; i < kRowsNum; i++) {
    for (int j = 0; j < kColsNum; j++) {
      QPushButton* widgetField = createBtnGameField();
      m_btns[i][j] = widgetField;
      qGridLayout->addWidget(widgetField, i, j);
      connected = QObject::connect(
          &game_state_, &GameState::SignalUpdate, this, [=, this](int row) {
            QString rowString = game_state_.GetRow(row);
            for (int k = 0; k < kColsNum; k++) {
              if (k < rowString.length()) {
                m_btns[row][k]->setText(QString(rowString[k]));
              } else {
                m_btns[row][k]->setText("");
              }
            }
          });
      IS_CONENCTED_OK

      connected =
          QObject::connect(&game_state_, &GameState::SignalUpdateRowColors,
                           this, [=, this](int row, QVector<QColor> colors) {
                             QString rowString = game_state_.GetRow(row);
                             for (int k = 0; k < kColsNum; k++) {
                               auto btn = m_btns[row][k];
                               QPalette pal = btn->palette();
                               pal.setColor(QPalette::Button, colors[k]);
                               btn->setAutoFillBackground(true);
                               btn->setPalette(pal);
                               btn->update();
                             }
                           });
      IS_CONENCTED_OK

      connected = QObject::connect(
          &game_state_, &GameState::SignalReset, this, [widgetField]() {
            QPalette pal = widgetField->palette();
            pal.setColor(QPalette::Button, QColor(239, 239, 239, 255));
            widgetField->setAutoFillBackground(true);
            widgetField->setPalette(pal);
            widgetField->update();
          });
      IS_CONENCTED_OK
    }
  }

  this->setLayout(qGridLayout);
};
