#pragma once
#include <QDebug>
#include <QObject>
#include <enums/server_game.hpp>

class APIApplicationLogic : public QObject {
  Q_OBJECT

 public:
  APIApplicationLogic();
  void RequestLogin(const QString& user_name, const QString& password);
  void RequestRegistration(const QString& user_name, const QString& password,
                           const QString& e_mail);
  void RequestCheckTheRow(const QString& word);
  void RequestNewGame();

 signals:
  void ResponseLogin(LoginStatus status);
  void ResponseRegistration(RegistrationStatus status);
  void ResponseCheckTheRow(CheckTheRowResult result, int num_of_tries,
                           std::vector<TheCharColor> colors,
                           const std::string& word_answer);
  void ResponseNewGame();

 private:
  QString game_uuid_{"01234567-89ab-cdef-0123-456789abcdef"};
  QString user_uuid_{"00000000-89ab-cdef-0123-456789abcdef"};
};
