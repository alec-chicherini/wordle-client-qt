#include <APIApplicationLogic.h>
#include <QTimer>
APIApplicationLogic::APIApplicationLogic() {}
void APIApplicationLogic::RequestLogin(const QString& user_name,
                                       const QString& password) {
  if (user_name == QString("admin") && password == QString("admin"))
    QTimer::singleShot(
        2000, this, [=, this] { emit ResponseLogin(LoginStatus::kLoginIsOk); });
  else
    QTimer::singleShot(2000, this, [=, this] {
      emit ResponseLogin(LoginStatus::kUnauthorized);
    });
}
void APIApplicationLogic::RequestRegistration(const QString& user_name,
                                              const QString& password,
                                              const QString& e_mail) {
  if (user_name == QString("admin")) {
    QTimer::singleShot(2000, this, [=, this] {
      emit ResponseRegistration(RegistrationStatus::kUserNameDuplicate);
    });

  } else {
    QTimer::singleShot(2000, this, [=, this] {
      emit ResponseRegistration(RegistrationStatus::kRegistrationIsOk);
    });
  }
}