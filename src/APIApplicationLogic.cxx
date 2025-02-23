#include <APIApplicationLogic.h>
#include <QTimer>
APIApplicationLogic::APIApplicationLogic() {}
void APIApplicationLogic::RequestLogin(const QString& user_name,
                                       const QString& password) {
  if (user_name == QString("admin") && password == QString("admin")) {
    QTimer::singleShot(
        2000, this, [=, this] { emit ResponseLogin(LoginStatus::kLoginIsOk); });
  } else {
    QTimer::singleShot(2000, this, [=, this] {
      emit ResponseLogin(LoginStatus::kUnauthorized);
    });
  }
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
void APIApplicationLogic::RequestCheckTheRow(const QString& word,
                                             const QString&) {
  if (word == QString("ЕРЕСЬ")) {
    QTimer::singleShot(2000, this, [=, this] {
      emit ResponseCheckTheRow(
          CheckTheRowResult::kWordDoNotExists, 1,
          std::vector<TheWordColor>{TheWordColor::kNone, TheWordColor::kNone,
                                    TheWordColor::kNone, TheWordColor::kNone,
                                    TheWordColor::kNone},
          std::string(""));
    });
  } else if (word == QString("ЕМЧАК")) {
    QTimer::singleShot(2000, this, [=, this] {
      emit ResponseCheckTheRow(
          CheckTheRowResult::kWordExists, 1,
          std::vector<TheWordColor>{TheWordColor::kGreen, TheWordColor::kYellow,
                                    TheWordColor::kNone, TheWordColor::kYellow,
                                    TheWordColor::kGreen},
          std::string(""));
    });
  } else if (word == QString("ЕССЕЙ")) {
    QTimer::singleShot(2000, this, [=, this] {
      emit ResponseCheckTheRow(
          CheckTheRowResult::kWordIsAnswer, 1,
          std::vector<TheWordColor>{TheWordColor::kGreen, TheWordColor::kGreen,
                                    TheWordColor::kGreen, TheWordColor::kGreen,
                                    TheWordColor::kGreen},
          std::string(""));
    });
  } else {
    QTimer::singleShot(2000, this, [=, this] {
      emit ResponseCheckTheRow(
          CheckTheRowResult::kWordDoNotExists, 1,
          std::vector<TheWordColor>{TheWordColor::kNone, TheWordColor::kNone,
                                    TheWordColor::kNone, TheWordColor::kNone,
                                    TheWordColor::kNone},
          std::string(""));
    });
  }
}
void APIApplicationLogic::RequestNewGame() {
  QTimer::singleShot(
      2000, this, [=, this] { emit ResponseNewGame(QString("1234567890")); });
}
