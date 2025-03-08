#pragma once
#include <APIApplicationLogic.h>
#include <QColor>
#include <QDebug>
#include <QHash>
#include <QObject>
#include <array>
#include <unordered_set>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
namespace std {
template <>
struct hash<QString> {
  std::size_t operator()(const QString& s) const noexcept {
    return (size_t)qHash(s);
  }
};
}  // namespace std
#endif
constexpr int kRowsNum = 6;
constexpr int kColsNum = 5;

class GameState : public QObject {
  Q_OBJECT

 public:
  void InputChar(const QString& ch);
  GameState(APIApplicationLogic& api_logic);
  QString GetRow(int);
  QVector<QColor> CheckTheRowColors(const std::vector<TheCharColor>& colors);

 public slots:
  void ProcessCheckTheRow(CheckTheRowResult result, int num_of_tries,
                          std::vector<TheCharColor> colors,
                          const std::string& word_answer);
  void ProcessNewGame(const wordle_data::ResponseNewGameBody& response);
  void Reset();
 signals:
  void SignalMsgBox(QString);
  void SignalUpdate(int);
  void SignalUpdateRowColors(int, QVector<QColor>);
  void SignalQuitOrRestart();
  void SignalReset();

 private:
  std::array<QString, kRowsNum> m_game_state_array;
  int m_row{0};
  APIApplicationLogic& api_application_logic_;
  bool keyboard_lock_;
};
