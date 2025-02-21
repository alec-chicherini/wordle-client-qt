#include <WidgetAuthorization.h>

#include <QDebug>
#include <QFrame>
#include <QHBoxLayout>
#include <QRegularExpressionValidator>
#include <QVBoxLayout>

#include <QtHelper.h>
#include <algorithm>
#include <map>
#include <vector>
using namespace std::string_literals;
const std::map<LoginStatus, std::string> kLoginStatusString = {
    {LoginStatus::kNone, ""s},
    {LoginStatus::kUserNameEmpty, "Введите имя пользователя"s},
    {LoginStatus::kPasswordEmpty, "Введите пароль"s},
    {LoginStatus::kUnauthorized, "Неправильный логин или пароль"s},
    {LoginStatus::kLoginIsOk, "Авторизация прошла успешно"s},
    {LoginStatus::kRequestInProgress, "Соединение с сервером.."s}};

const std::map<RegistrationStatus, std::string> kRegistrationStatusString = {
    {RegistrationStatus::kNone, ""s},
    {RegistrationStatus::kUserNameEmpty, "Введите имя пользователя"s},
    {RegistrationStatus::kPasswordEmpty, "Введите пароль"s},
    {RegistrationStatus::kPasswordConfirmEmpty,
     "Введите подтверждение пароля"s},
    {RegistrationStatus::kPasswordMismatch, "Пароли не совпадают"s},
    {RegistrationStatus::kUserNameDuplicate, "Имя пользователя занято"s},
    {RegistrationStatus::kRegistrationIsOk, "Регистрация возможна"s},
    {RegistrationStatus::kBadEmailAddress, "Неправильный формат адреса почты"s},
    {RegistrationStatus::kRequestInProgress, "Соединение с сервером.."s}};

void WidgetAuthorization::LoginDataWasChanged() {
  std::vector<std::string> resultMessage;
  btn_enter_login->setEnabled(false);
  if (line_edit_user_name_login->text().isEmpty()) {
    resultMessage.push_back(kLoginStatusString.at(LoginStatus::kUserNameEmpty));
  }
  if (line_edit_password_login->text().isEmpty()) {
    resultMessage.push_back(kLoginStatusString.at(LoginStatus::kPasswordEmpty));
  }
  if (line_edit_user_name_login->text().isEmpty() == false &&
      line_edit_password_login->text().isEmpty() == false) {
    count_request_login++;
    api_application_logic_.RequestLogin(line_edit_user_name_login->text(),
                                        line_edit_password_login->text());
    QString styleSheet("QLabel { color : darkGreen; }");
    label_info_login->setStyleSheet(styleSheet);
    label_info_login->setText(QString::fromStdString(
        kLoginStatusString.at(LoginStatus::kRequestInProgress)));
    return;
  }
  QString styleSheet("QLabel { color : red; }");
  label_info_login->setStyleSheet(styleSheet);
  std::string result_message_all = std::accumulate(
      std::next(resultMessage.begin()), resultMessage.end(), resultMessage[0],
      [](const std::string& lhs, const std::string& rhs) {
        return lhs + "\n" + rhs;
      });

  label_info_login->setText(QString::fromStdString(result_message_all));
}
void WidgetAuthorization::RegistrationDataWasChanged() {
  std::vector<std::string> resultMessage;
  btn_registration->setEnabled(false);
  if (line_edit_user_name_registration->text().isEmpty()) {
    resultMessage.push_back(
        kRegistrationStatusString.at(RegistrationStatus::kUserNameEmpty));
  }
  if (line_edit_password_registration->text().isEmpty()) {
    resultMessage.push_back(
        kRegistrationStatusString.at(RegistrationStatus::kPasswordEmpty));
  }
  if (line_edit_password_confirm_registration->text().isEmpty()) {
    resultMessage.push_back(kRegistrationStatusString.at(
        RegistrationStatus::kPasswordConfirmEmpty));
  }
  if (line_edit_password_registration->text().isEmpty() == false &&
      line_edit_password_confirm_registration->text().isEmpty() == false) {
    if (line_edit_password_registration->text() !=
        line_edit_password_confirm_registration->text()) {
      resultMessage.push_back(
          kRegistrationStatusString.at(RegistrationStatus::kPasswordMismatch));
    }
  }

  QRegularExpressionValidator* validatorEMail = new QRegularExpressionValidator(
      QRegularExpression("[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}"));
  int pos;
  QString email_entered = line_edit_email_registration->text();
  if (line_edit_email_registration->text().isEmpty() == false &&
      validatorEMail->validate(email_entered, pos) != QValidator::Acceptable) {
    resultMessage.push_back(
        kRegistrationStatusString.at(RegistrationStatus::kBadEmailAddress));
  }

  if (resultMessage.empty()) {
    count_request_registration++;
    api_application_logic_.RequestRegistration(
        line_edit_user_name_registration->text(),
        line_edit_password_registration->text(),
        line_edit_email_registration->text());
    QString styleSheet("QLabel { color : darkGreen; }");
    label_info_registration->setStyleSheet(styleSheet);
    label_info_registration->setText(QString::fromStdString(
        kRegistrationStatusString.at(RegistrationStatus::kRequestInProgress)));

    return;
  } else {
    QString styleSheet("QLabel { color : red; }");
    label_info_registration->setStyleSheet(styleSheet);
    std::string result_message_all = std::accumulate(
        std::next(resultMessage.begin()), resultMessage.end(), resultMessage[0],
        [](const std::string& lhs, const std::string& rhs) {
          return lhs + "\n" + rhs;
        });

    label_info_registration->setText(
        QString::fromStdString(result_message_all));
  }
}
WidgetAuthorization::WidgetAuthorization(WidgetApplicationLogic& widget_logic,
                                         APIApplicationLogic& api_logic)
    : widget_application_logic_(widget_logic),
      api_application_logic_(api_logic),
      count_request_login(0),
      count_request_registration(0) {
  [[maybe_unused]] bool connected;

  QVBoxLayout* q_vbox_layout_authorizationTop = new QVBoxLayout(this);
  q_vbox_layout_authorizationTop->setAlignment(Qt::AlignCenter);
  QWidget* widget_aligned_center = new QWidget;
  widget_aligned_center->setMaximumWidth(330);
  q_vbox_layout_authorizationTop->addWidget(widget_aligned_center);

  QVBoxLayout* q_vbox_layout_authorization =
      new QVBoxLayout(widget_aligned_center);

  QLabel* label_login = new QLabel("Вход");
  q_vbox_layout_authorization->addWidget(label_login);

  line_edit_user_name_login = new QLineEdit();
  line_edit_user_name_login->setPlaceholderText("Имя пользователя");
  q_vbox_layout_authorization->addWidget(line_edit_user_name_login);
  connected = QObject::connect(
      line_edit_user_name_login, &QLineEdit::textChanged, this,
      [=, this](const QString&) { emit LoginDataWasChanged(); });
  IS_CONENCTED_OK

  line_edit_password_login = new QLineEdit();
  line_edit_password_login->setPlaceholderText("Пароль");
  line_edit_password_login->setEchoMode(QLineEdit::Password);
  q_vbox_layout_authorization->addWidget(line_edit_password_login);
  connected = QObject::connect(
      line_edit_password_login, &QLineEdit::textChanged, this,
      [=, this](const QString&) { emit LoginDataWasChanged(); });
  IS_CONENCTED_OK

  btn_enter_login = new QPushButton("Войти");
  q_vbox_layout_authorization->addWidget(btn_enter_login);
  connected = QObject::connect(
      btn_enter_login, &QPushButton::clicked, &widget_application_logic_,
      [=, this]() {
        widget_application_logic_.GoTo(WidgetApplicationLogic::State::kMenu);
      });
  IS_CONENCTED_OK

  label_info_login = new QLabel;
  q_vbox_layout_authorization->addWidget(label_info_login);

  QFrame* line_horizontal = new QFrame();
  line_horizontal->setFrameShape(QFrame::HLine);
  line_horizontal->setFrameShadow(QFrame::Sunken);
  line_horizontal->setLineWidth(2);
  q_vbox_layout_authorization->addWidget(line_horizontal);

  QLabel* label_registration = new QLabel("Регистрация");
  q_vbox_layout_authorization->addWidget(label_registration);

  line_edit_user_name_registration = new QLineEdit();
  line_edit_user_name_registration->setPlaceholderText("Имя пользователя");
  q_vbox_layout_authorization->addWidget(line_edit_user_name_registration);
  connected = QObject::connect(
      line_edit_user_name_registration, &QLineEdit::textChanged, this,
      [=, this](const QString&) { emit RegistrationDataWasChanged(); });
  IS_CONENCTED_OK
  line_edit_password_registration = new QLineEdit();
  line_edit_password_registration->setPlaceholderText("Пароль");
  line_edit_password_registration->setEchoMode(QLineEdit::Password);
  q_vbox_layout_authorization->addWidget(line_edit_password_registration);
  connected = QObject::connect(
      line_edit_password_registration, &QLineEdit::textChanged, this,
      [=, this](const QString&) { emit RegistrationDataWasChanged(); });
  IS_CONENCTED_OK

  line_edit_password_confirm_registration = new QLineEdit();
  line_edit_password_confirm_registration->setPlaceholderText(
      "Подтверждение пароля");
  line_edit_password_confirm_registration->setEchoMode(QLineEdit::Password);
  q_vbox_layout_authorization->addWidget(
      line_edit_password_confirm_registration);
  connected = QObject::connect(
      line_edit_password_confirm_registration, &QLineEdit::textChanged, this,
      [=, this](const QString&) { emit RegistrationDataWasChanged(); });
  IS_CONENCTED_OK

  line_edit_email_registration = new QLineEdit();
  line_edit_email_registration->setPlaceholderText("Почта");
  q_vbox_layout_authorization->addWidget(line_edit_email_registration);
  connected = QObject::connect(
      line_edit_email_registration, &QLineEdit::textChanged, this,
      [=, this](const QString&) { emit RegistrationDataWasChanged(); });
  IS_CONENCTED_OK

  btn_registration = new QPushButton("Зарегистрировать");
  q_vbox_layout_authorization->addWidget(btn_registration);
  connected = QObject::connect(
      btn_registration, &QPushButton::clicked, &widget_application_logic_,
      [=, this]() {
        widget_application_logic_.GoTo(WidgetApplicationLogic::State::kMenu);
      });
  IS_CONENCTED_OK

  label_info_registration = new QLabel("");
  q_vbox_layout_authorization->addWidget(label_info_registration);

  connected = QObject::connect(&api_application_logic_,
                               &APIApplicationLogic::ResponseLogin, this,
                               &WidgetAuthorization::ProcessLogin);
  IS_CONENCTED_OK
  connected = QObject::connect(&api_application_logic_,
                               &APIApplicationLogic::ResponseRegistration, this,
                               &WidgetAuthorization::ProcessRegistration);
  IS_CONENCTED_OK
  connected = QObject::connect(
      &widget_application_logic_, &WidgetApplicationLogic::StateChanged, this,
      [=, this](WidgetApplicationLogic::State state) {
        if (state == WidgetApplicationLogic::State::kAuthorization) {
          ProcessLogOut();
        }
      });
  IS_CONENCTED_OK

  ProcessLogOut();
};

void WidgetAuthorization::ProcessLogin(LoginStatus status) {
  count_request_login -= 1;
  if (count_request_login != 0) return;
  if (status == LoginStatus::kLoginIsOk) {
    line_edit_user_name_login->setEnabled(false);
    line_edit_password_login->setEnabled(false);
    btn_enter_login->setEnabled(true);
    QString styleSheet("QLabel { color : green; }");
    label_info_login->setStyleSheet(styleSheet);
    label_info_login->setText(
        QString::fromStdString(kLoginStatusString.at(LoginStatus::kLoginIsOk)));

    line_edit_user_name_registration->setEnabled(false);
    line_edit_password_registration->setEnabled(false);
    line_edit_password_confirm_registration->setEnabled(false);
    line_edit_email_registration->setEnabled(false);
    btn_registration->setEnabled(false);
    label_info_registration->setText(QString());
  } else if (status == LoginStatus::kUnauthorized) {
    QString styleSheet("QLabel { color : red; }");
    label_info_login->setStyleSheet(styleSheet);
    label_info_login->setText(QString::fromStdString(
        kLoginStatusString.at(LoginStatus::kUnauthorized)));
  }
}

void WidgetAuthorization::ProcessRegistration(RegistrationStatus status) {
  count_request_registration -= 1;
  if (count_request_registration != 0) return;
  if (status == RegistrationStatus::kUserNameDuplicate) {
    QString styleSheet("QLabel { color : red; }");
    label_info_registration->setStyleSheet(styleSheet);
    label_info_registration->setText(QString::fromStdString(
        kRegistrationStatusString.at(RegistrationStatus::kUserNameDuplicate)));
  } else if (status == RegistrationStatus::kRegistrationIsOk) {
    QString styleSheet("QLabel { color : green; }");
    label_info_registration->setStyleSheet(styleSheet);
    label_info_registration->setText(QString::fromStdString(
        kRegistrationStatusString.at(RegistrationStatus::kRegistrationIsOk)));
    btn_registration->setEnabled(true);
  }
}

void WidgetAuthorization::ProcessLogOut() {
  line_edit_user_name_login->setEnabled(true);
  line_edit_user_name_login->setText(QString());
  line_edit_password_login->setEnabled(true);
  line_edit_password_login->setText(QString());
  btn_enter_login->setEnabled(false);

  line_edit_user_name_registration->setEnabled(true);
  line_edit_user_name_registration->setText(QString());
  line_edit_password_registration->setEnabled(true);
  line_edit_password_registration->setText(QString());
  line_edit_password_confirm_registration->setEnabled(true);
  line_edit_password_confirm_registration->setText(QString());
  line_edit_email_registration->setEnabled(true);
  line_edit_email_registration->setText(QString());
  btn_registration->setEnabled(false);

  emit LoginDataWasChanged();
  emit RegistrationDataWasChanged();
}