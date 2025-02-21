#pragma once
#include <QDebug>
#include <QObject>

enum class LoginStatus {
  kNone = 0,
  kUserNameEmpty,
  kPasswordEmpty,
  kUnauthorized,
  kLoginIsOk,
  kRequestInProgress
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
};

class APIApplicationLogic : public QObject {
  Q_OBJECT

 public:
  APIApplicationLogic();
  void RequestLogin(const QString& user_name, const QString& password);
  void RequestRegistration(const QString& user_name, const QString& password,
                           const QString& e_mail);
 signals:
  void ResponseLogin(LoginStatus status);
  void ResponseRegistration(RegistrationStatus status);
};
