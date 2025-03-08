#pragma once
#include <NetworkHelper.h>
#include <enum_check_the_row_result.pb.h>
#include <enum_the_char_color.pb.h>
#include <QDebug>
#include <QObject>
#include <enums/server_game.hpp>
using namespace wordle_data;
class APIApplicationLogic : public NetworkHelper {
  Q_OBJECT

 public:
  APIApplicationLogic();
  void RequestLogin(const QString& user_name, const QString& password);
  void RequestRegistration(const QString& user_name, const QString& password,
                           const QString& e_mail);

  void RequestCheckTheRowCallback(
      const wordle_data::ResponseCheckTheRowBody& response);
  void RequestCheckTheRow(const QString& word);
  void RequestNewGame();

 signals:
  void ResponseLogin(LoginStatus status);
  void ResponseRegistration(RegistrationStatus status);
  void ResponseCheckTheRow(CheckTheRowResult result, int num_of_tries,
                           std::vector<TheCharColor> colors,
                           const std::string& word_answer);
  void ResponseNewGame(const wordle_data::ResponseNewGameBody& response);

 private:
  QString game_uuid_{"01234567-89ab-cdef-0123-456789abcdef"};
  QString user_uuid_{"00000000-89ab-cdef-0123-456789abcdef"};
};
