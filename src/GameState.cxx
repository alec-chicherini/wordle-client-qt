#include <GameState.h>
#include <QtHelper.h>
#include <QApplication>
#include <QColor>
#include <QRandomGenerator>

QString GameState::GetRow(int row) { return m_game_state_array[row]; };

QVector<QColor> GameState::CheckTheRowColors(
    const std::vector<TheCharColor>& colors) {
  QVector<QColor> result(kColsNum);
  for (qsizetype i = 0; i < qsizetype(kColsNum); i++) {
    if (colors[i] == TheCharColor::kGreen) {
      result[i] = Qt::green;
    } else if (colors[i] == TheCharColor::kYellow) {
      result[i] = Qt::yellow;
    } else {
      result[i] = Qt::lightGray;
    }
  }
  return result;
};

void GameState::InputChar(const QString& ch) {
  if (keyboard_lock_ == true) return;
  QString& current_word = m_game_state_array[m_row];

  if (ch == "<-") {
    if (current_word.length()) {
      current_word.resize(current_word.length() - 1);
      emit SignalUpdate(m_row);
    }
  } else if (ch == "Enter" && current_word.length() == kColsNum) {
    keyboard_lock_ = true;
    api_application_logic_.RequestCheckTheRow(current_word);
  } else if (ch == "Enter" && current_word.length() != kColsNum) {
    emit SignalMsgBox(QString("Букв меньше чем надо"));
  } else {
    if (current_word.length() != kColsNum) {
      current_word.push_back(ch);
      emit SignalUpdate(m_row);
    }
  }
};

GameState::GameState(APIApplicationLogic& api_logic)
    : api_application_logic_(api_logic), keyboard_lock_(false) {
  [[maybe_unused]] bool connected;
  connected = QObject::connect(&api_application_logic_,
                               &APIApplicationLogic::ResponseCheckTheRow, this,
                               &GameState::ProcessCheckTheRow);
  IS_CONENCTED_OK
  connected = QObject::connect(&api_application_logic_,
                               &APIApplicationLogic::ResponseNewGame, this,
                               &GameState::ProcessNewGame);
  IS_CONENCTED_OK
};

void GameState::Reset() { api_application_logic_.RequestNewGame(); }

void GameState::ProcessNewGame() {
  for (size_t i = 0; i < kRowsNum; i++) {
    m_game_state_array[i] = QString();
    emit SignalUpdateRowColors(
        i,
        CheckTheRowColors(
            {TheCharColor::kNoneTheCharColor, TheCharColor::kNoneTheCharColor,
             TheCharColor::kNoneTheCharColor, TheCharColor::kNoneTheCharColor,
             TheCharColor::kNoneTheCharColor}));
    emit SignalUpdate(i);
  }
  m_row = 0;
  emit SignalReset();
}

void GameState::ProcessCheckTheRow(CheckTheRowResult check_the_row_result,
                                   int num_of_tries,
                                   std::vector<TheCharColor> colors,
                                   const std::string& word_answer) {
  keyboard_lock_ = false;
  if (check_the_row_result == CheckTheRowResult::kWordIsAnswer) {
    emit SignalUpdateRowColors(m_row, CheckTheRowColors(colors));
    emit SignalMsgBox(QString("Вы победили.\nВы отгадали слово:\n%1")
                          .arg(QString::fromStdString(word_answer)));
    emit SignalQuitOrRestart();
  } else if (check_the_row_result == CheckTheRowResult::kWordExists) {
    emit SignalUpdateRowColors(m_row, CheckTheRowColors(colors));
    if (m_row != kRowsNum - 1) {
      m_row++;
    } else {
      emit SignalMsgBox(QString("Вы проиграли.\nБыло загадано слово:\n%1")
                            .arg(QString::fromStdString(word_answer)));
      emit SignalQuitOrRestart();
    }
  } else if (check_the_row_result == CheckTheRowResult::kWordDoNotExists) {
    emit SignalMsgBox(QString("Такого слова не существует"));
  }
}