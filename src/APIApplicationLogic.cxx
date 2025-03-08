#include <APIApplicationLogic.h>
#include <NetworkHelper.h>
#include <QTimer>
 using namespace std::literals;
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

  SendRequest(
      request_check_the_row_body,
      "https://wordle-server-game.repotest.ru/v1/check_the_row"s,
      [=, this](const wordle_data::ResponseCheckTheRowBody& response) -> void {
        constexpr size_t GAME_WORD_SIZE_ = 5;
        emit ResponseCheckTheRow(
            response.check_the_row_result(), response.number_of_attempts_left(),
            std::vector<TheCharColor>{
                response.the_char_colors(0), response.the_char_colors(1),
                response.the_char_colors(2), response.the_char_colors(3),
                response.the_char_colors(4)},
            response.word_answer());
      });
}
void APIApplicationLogic::RequestNewGame() {
  UUID* user_uuid = new UUID;
  user_uuid->set_value(user_uuid_.toStdString());
  wordle_data::RequestNewGameBody request_new_game_body;
  request_new_game_body.set_allocated_user_uuid(user_uuid);

  SendRequest(
      request_new_game_body,
      "https://wordle-server-game.repotest.ru/v1/new_game"s,
      [=, this](const wordle_data::ResponseNewGameBody& response) -> void {
        game_uuid_ = QString::fromStdString(response.game_uuid().value());
        emit ResponseNewGame(response);
      });
}