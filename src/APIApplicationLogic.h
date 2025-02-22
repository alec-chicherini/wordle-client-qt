#pragma once
#include <QDebug>
#include <QObject>

enum class LoginStatus {
  kNone = 0,
  kUserNameEmpty,
  kPasswordEmpty,
  kUnauthorized,
  kLoginIsOk,
  kRequestInProgress,
  kServerError
};

enum class RegistrationStatus {
  kNone = 0,
  kUserNameEmpty,
  kPasswordEmpty,
  kPasswordConfirmEmpty,
  kPasswordMismatch,
  kUserNameDuplicate,
  kRegistrationIsOk,
  kRequestInProgress,
  kBadEmailAddress,
  kServerError
};

enum class CheckTheRowResult {
  kNone = 0,
  kWordDoNotExists,
  kWordExists,
  kWordIsAnswer,
  kServerError
};

enum class TheWordColor { kNone, kGreen, kYellow };

class APIApplicationLogic : public QObject {
  Q_OBJECT

 public:
  APIApplicationLogic();
  void RequestLogin(const QString& user_name, const QString& password);
  void RequestRegistration(const QString& user_name, const QString& password,
                           const QString& e_mail);
  void RequestCheckTheRow(const QString& word, const QString& game_id);
  void RequestNewGame();

 signals:
  void ResponseLogin(LoginStatus status);
  void ResponseRegistration(RegistrationStatus status);
  void ResponseCheckTheRow(CheckTheRowResult result, int num_of_tries,
                           std::vector<TheWordColor> colors,
                           const std::string& word_answer);
  void ResponseNewGame(const QString& game_id);
};
