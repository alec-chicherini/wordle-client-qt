#include <APIApplicationLogic.h>
#include <request_check_the_row_body.pb.h>
#include <request_new_game_body.pb.h>
#include <QTimer>
#include <QNetworkAccessManager>
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
void APIApplicationLogic::RequestCheckTheRow(const QString& word_) {
  UUID* user_uuid = new UUID;
  user_uuid->set_value(user_uuid_.toStdString());
  UUID* game_uuid = new UUID;
  game_uuid->set_value(game_uuid_.toStdString());
  wordle_data::RequestCheckTheRowBody request_check_the_row_body;
  request_check_the_row_body.set_allocated_game_uuid(game_uuid);
  request_check_the_row_body.set_allocated_user_uuid(user_uuid);
  request_check_the_row_body.set_word(word_.toStdString());

  std::string serialized;
  request_check_the_row_body.SerializeToString(&serialized);

  qDebug() << "send RequestCheckTheRowBody " << serialized;

  if (word_ == QString("ЕРЕСЬ")) {
    QTimer::singleShot(2000, this, [=, this] {
      emit ResponseCheckTheRow(
          CheckTheRowResult::kWordDoNotExists, 1,
          std::vector<TheCharColor>{
              TheCharColor::kNoneTheCharColor, TheCharColor::kNoneTheCharColor,
              TheCharColor::kNoneTheCharColor, TheCharColor::kNoneTheCharColor,
              TheCharColor::kNoneTheCharColor},
          std::string(""));
    });
  } else if (word_ == QString("ЕМЧАК")) {
    QTimer::singleShot(2000, this, [=, this] {
      emit ResponseCheckTheRow(CheckTheRowResult::kWordExists, 1,
                               std::vector<TheCharColor>{
                                   TheCharColor::kGreen, TheCharColor::kYellow,
                                   TheCharColor::kNoneTheCharColor,
                                   TheCharColor::kYellow, TheCharColor::kGreen},
                               std::string(""));
    });
  } else if (word_ == QString("ЕССЕЙ")) {
    QTimer::singleShot(2000, this, [=, this] {
      emit ResponseCheckTheRow(
          CheckTheRowResult::kWordIsAnswer, 1,
          std::vector<TheCharColor>{TheCharColor::kGreen, TheCharColor::kGreen,
                                    TheCharColor::kGreen, TheCharColor::kGreen,
                                    TheCharColor::kGreen},
          std::string(""));
    });
  } else {
    QTimer::singleShot(2000, this, [=, this] {
      emit ResponseCheckTheRow(
          CheckTheRowResult::kWordDoNotExists, 1,
          std::vector<TheCharColor>{
              TheCharColor::kNoneTheCharColor, TheCharColor::kNoneTheCharColor,
              TheCharColor::kNoneTheCharColor, TheCharColor::kNoneTheCharColor,
              TheCharColor::kNoneTheCharColor},
          std::string(""));
    });
  }
}
void APIApplicationLogic::RequestNewGame() {
  UUID* user_uuid = new UUID;
  user_uuid->set_value(user_uuid_.toStdString());
  wordle_data::RequestNewGameBody request_new_game_body;
  request_new_game_body.set_allocated_user_uuid(user_uuid);

  std::string serialized;
  request_new_game_body.SerializeToString(&serialized);

  QNetworkAccessManager* manager = new QNetworkAccessManager(this);
  connect(manager, &QNetworkAccessManager::finished, this, 
      [=,this](QNetworkReply *reply){
            for (const auto& header : reply->rawHeaderList())
              qDebug() << "reply header " << header;
      });
  manager->get(QNetworkRequest(QUrl("http://repotest.ru:8088/")));
  

  QTimer::singleShot(2000, this, [=, this] { emit ResponseNewGame(); });
}