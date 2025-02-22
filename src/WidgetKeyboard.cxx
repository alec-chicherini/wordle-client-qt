#include <QtHelper.h>
#include <WidgetKeyboard.h>
#include <QApplication>
#include <QDebug>
#include <QKeyEvent>
WidgetKeyboard::WidgetKeyboard(GameState& state) : game_state_(state) {
  [[maybe_unused]] bool connected;
  constexpr int KEY_SIZE = 50;
  using strQSize_T = std::pair<QString, QSize>;
  auto createKeyboardRow = [&](const std::vector<strQSize_T>& btnAttribute) {
    QWidget* gameKeyboardRow = new QWidget();
    gameKeyboardRow->setContentsMargins(0, 0, 0, 0);

    QHBoxLayout* qHBoxLayout = new QHBoxLayout(gameKeyboardRow);
    qHBoxLayout->setSpacing(2);
    for (const auto& attr : btnAttribute) {
      auto btn = new QPushButton(attr.first);
      btn->setMinimumWidth(attr.second.width());
      btn->setFixedHeight(attr.second.height());
      qHBoxLayout->addWidget(btn);
      connected =
          QObject::connect(btn, &QPushButton::pressed, &game_state_,
                           [=, this]() { game_state_.InputChar(btn->text()); });
      IS_CONENCTED_OK

      connected =
          QObject::connect(&game_state_, &GameState::SignalUpdateRowColors,
                           this, [=, this](int row, QVector<QColor> colors) {
                             QString rowString = game_state_.GetRow(row);
                             for (int k = 0; k < kColsNum; k++) {
                               if (btn->text() == rowString[k]) {
                                 QPalette pal = btn->palette();
                                 pal.setColor(QPalette::Button, colors[k]);
                                 btn->setAutoFillBackground(true);
                                 btn->setPalette(pal);
                                 btn->update();
                               }
                             }
                           });
      IS_CONENCTED_OK

      connected = QObject::connect(
          &game_state_, &GameState::SignalReset, this, [btn]() {
            QPalette pal = btn->palette();
            pal.setColor(QPalette::Button, QColor(239, 239, 239, 255));
            btn->setAutoFillBackground(true);
            btn->setPalette(pal);
            btn->update();
          });
      IS_CONENCTED_OK
    }
    return gameKeyboardRow;
  };

  auto key = [](const std::string& name, QSize size = QSize()) {
    if (size == QSize()) size = QSize(KEY_SIZE, KEY_SIZE);
    return strQSize_T{QString::fromUtf8(name.c_str()), size};
  };

  QVBoxLayout* qVBoxLayoutKeyboard = new QVBoxLayout();

  qVBoxLayoutKeyboard->setSpacing(2);
  qVBoxLayoutKeyboard->setContentsMargins(0, 0, 0, 0);
  qVBoxLayoutKeyboard->setAlignment(Qt::AlignCenter);
  qVBoxLayoutKeyboard->addWidget(createKeyboardRow(
      {key("Ё"), key("Й"), key("Ц"), key("У"), key("К"), key("Е"), key("Н"),
       key("Г"), key("Ш"), key("Щ"), key("З"), key("Х"), key("Ъ")}));
  qVBoxLayoutKeyboard->addWidget(createKeyboardRow(
      {key("Ф"), key("Ы"), key("В"), key("А"), key("П"), key("Р"), key("О"),
       key("Л"), key("Д"), key("Ж"), key("Э")}));
  qVBoxLayoutKeyboard->addWidget(createKeyboardRow(
      {key("<-", QSize(KEY_SIZE * 2, KEY_SIZE)), key("Я"), key("Ч"), key("С"),
       key("M"), key("И"), key("Т"), key("Ь"), key("Б"), key("Ю"),
       key("Enter", QSize(KEY_SIZE * 2, KEY_SIZE))}));
  this->setLayout(qVBoxLayoutKeyboard);
};