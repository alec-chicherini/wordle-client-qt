#include <APIApplicationLogic.h>
#include <QTimer>
#include <schemas/server_game.hpp>
#include <userver/formats/json.hpp>
#include <userver/utils/boost_uuid4.hpp>

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
  wordle_json::RequestCheckTheRowBody request_check_the_row_body = {
      .user_uuid =
          userver::utils::BoostUuidFromString(user_uuid_.toStdString()),
      .game_uuid =
          userver::utils::BoostUuidFromString(game_uuid_.toStdString()),
      .word = word_.toStdString()};

  auto userver_json =
      userver::formats::json::ValueBuilder{request_check_the_row_body}
          .ExtractValue();
  qDebug() << "send RequestCheckTheRowBody "
           << userver::formats::json::ToString(userver_json);

  if (word_ == QString("ЕРЕСЬ")) {
    QTimer::singleShot(2000, this, [=, this] {
      emit ResponseCheckTheRow(
          CheckTheRowResult::kWordDoNotExists, 1,
          std::vector<TheCharColor>{TheCharColor::kNone, TheCharColor::kNone,
                                    TheCharColor::kNone, TheCharColor::kNone,
                                    TheCharColor::kNone},
          std::string(""));
    });
  } else if (word_ == QString("ЕМЧАК")) {
    QTimer::singleShot(2000, this, [=, this] {
      emit ResponseCheckTheRow(
          CheckTheRowResult::kWordExists, 1,
          std::vector<TheCharColor>{TheCharColor::kGreen, TheCharColor::kYellow,
                                    TheCharColor::kNone, TheCharColor::kYellow,
                                    TheCharColor::kGreen},
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
          std::vector<TheCharColor>{TheCharColor::kNone, TheCharColor::kNone,
                                    TheCharColor::kNone, TheCharColor::kNone,
                                    TheCharColor::kNone},
          std::string(""));
    });
  }
}
void APIApplicationLogic::RequestNewGame() {
  wordle_json::RequestNewGameBody request_new_game_body = {
      .user_uuid =
          userver::utils::BoostUuidFromString(user_uuid_.toStdString())};
  auto userver_json =
      userver::formats::json::ValueBuilder{request_new_game_body}
          .ExtractValue();
  qDebug() << "send RequestNewGameBody "
           << userver::formats::json::ToString(userver_json);

  QTimer::singleShot(2000, this, [=, this] {
    emit ResponseNewGame();
  });
}
