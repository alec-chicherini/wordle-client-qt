#pragma once
#include <QDebug>
#include <QObject>

enum class LoginStatus {
  kNone = 0,
  kUserNameEmpty,
  kPasswordEmpty,
  kUnauthorized,
  kLoginIsOk
};

class APIApplicationLogic : public QObject {
  Q_OBJECT

 public:
  APIApplicationLogic();
  void RequestLogin(const QString& user_name, const QString& password);
  // public slots:
  //    void GoTo(State state);
  //
 signals:
  void ResponseLogin(LoginStatus status);
  //
  //
  // private:
  //    State current_state_;
};
