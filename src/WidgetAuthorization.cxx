#include <WidgetAuthorization.h>

#include <QDebug>
#include <QFrame>
#include <QHBoxLayout>
#include <QRegularExpressionValidator>
#include <QVBoxLayout>

#include <QtHelper.h>
#include <map>

using namespace std::string_literals;
std::map<LoginStatus, std::string> kLoginStatusString = {
    {LoginStatus::kNone, ""s},
    {LoginStatus::kUserNameEmpty, "Введите имя пользователя"s},
    {LoginStatus::kPasswordEmpty, "Введите пароль"s},
    {LoginStatus::kUnauthorized, "Неправильный логин или пароль"s},
    {LoginStatus::kLoginIsOk, "Авторизация прошла успешно"s}};

void WidgetAuthorization::LoginDataWasChanged() {
  std::string resultMessage;

  if (line_edit_user_name_login->text().isEmpty()) {
    resultMessage += kLoginStatusString[LoginStatus::kUserNameEmpty];
  }
  if (line_edit_password_login->text().isEmpty()) {
    resultMessage += "\n";
    resultMessage += kLoginStatusString[LoginStatus::kPasswordEmpty];
  }
  if (line_edit_user_name_login->text().isEmpty() == false &&
      line_edit_password_login->text().isEmpty() == false) {
    api_application_logic_.RequestLogin(line_edit_user_name_login->text(),
                                        line_edit_password_login->text());
    return;
  }
  QString styleSheet("QLabel { color : red; }");
  label_info_login->setStyleSheet(styleSheet);
  label_info_login->setText(QString::fromStdString(resultMessage));
}

WidgetAuthorization::WidgetAuthorization(WidgetApplicationLogic& widget_logic,
                                         APIApplicationLogic& api_logic)
    : widget_application_logic_(widget_logic),
      api_application_logic_(api_logic) {
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
  // constexpr int kUserNameMaxLength = 32;
  // line_edit_user_name_registration->setMaxLength(kUserNameMaxLength);
  q_vbox_layout_authorization->addWidget(line_edit_user_name_registration);
  connected = QObject::connect(
      line_edit_user_name_registration, &QLineEdit::textChanged, this,
      [=, this](const QString&) { emit RegistrationDataWasChanged(); });
  IS_CONENCTED_OK

  // https://www.book2s.com/tutorials/qt-qregularexpressionvalidator.html
  //  Create a QRegularExpressionValidator with a pattern for password
  //  validation The above regular expression pattern enforces the following
  //  criteria:
  //  - At least 8 characters long
  //  - Contains at least one digit [0-9]
  //  - Contains at least one lowercase letter [a-z]
  //  - Contains at least one uppercase letter [A-Z]
  //  - Contains at least one special character [@#$%^&+=]
  //  - Does not contain whitespace characters
  // QRegularExpressionValidator* validatorPassword =
  //    new QRegularExpressionValidator(
  //        QRegularExpression("^(?=.*[0-9])(?=.*[a-z])(?=.*[A-Z])(?=.*[@#$%^&+"
  //                           "=])(?=\\S+$).{8,}$"));
  // constexpr int kPasswordMaxLength = 16;
  line_edit_password_registration = new QLineEdit();
  line_edit_password_registration->setPlaceholderText("Пароль");
  line_edit_password_registration->setEchoMode(QLineEdit::Password);
  // line_edit_password_registration->setValidator(validatorPassword);
  // line_edit_password_registration->setMaxLength(kPasswordMaxLength);
  q_vbox_layout_authorization->addWidget(line_edit_password_registration);
  connected = QObject::connect(
      line_edit_password_registration, &QLineEdit::textChanged, this,
      [=, this](const QString&) { emit RegistrationDataWasChanged(); });
  IS_CONENCTED_OK

  line_edit_password_confirm_registration = new QLineEdit();
  line_edit_password_confirm_registration->setPlaceholderText(
      "Подтверждение пароля");
  line_edit_password_confirm_registration->setEchoMode(QLineEdit::Password);
  // line_edit_password_confirm_registration->setValidator(validatorPassword);
  // line_edit_password_confirm_registration->setMaxLength(kPasswordMaxLength);
  q_vbox_layout_authorization->addWidget(
      line_edit_password_confirm_registration);
  connected = QObject::connect(
      line_edit_password_confirm_registration, &QLineEdit::textChanged, this,
      [=, this](const QString&) { emit RegistrationDataWasChanged(); });
  IS_CONENCTED_OK

  // QRegularExpressionValidator* validatorEMail = new
  // QRegularExpressionValidator(
  // QRegularExpression("[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}"));
  line_edit_email_registration = new QLineEdit();
  // line_edit_email_registration->setValidator(validatorEMail);
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
  if (status == LoginStatus::kLoginIsOk) {
    line_edit_user_name_login->setEnabled(false);
    line_edit_password_login->setEnabled(false);
    btn_enter_login->setEnabled(true);
    QString styleSheet("QLabel { color : green; }");
    label_info_login->setStyleSheet(styleSheet);
    label_info_login->setText(
        QString::fromStdString(kLoginStatusString[LoginStatus::kLoginIsOk]));

    line_edit_user_name_registration->setEnabled(false);
    line_edit_password_registration->setEnabled(false);
    line_edit_password_confirm_registration->setEnabled(false);
    line_edit_email_registration->setEnabled(false);
    btn_registration->setEnabled(false);
    label_info_registration->setText(QString());
  } else if (status == LoginStatus::kUnauthorized) {
    QString styleSheet("QLabel { color : red; }");
    label_info_login->setStyleSheet(styleSheet);
    label_info_login->setText(
        QString::fromStdString(kLoginStatusString[LoginStatus::kUnauthorized]));
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