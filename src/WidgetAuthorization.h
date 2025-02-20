#pragma once
#include <WidgetApplicationLogic.h>
#include <QObject>
#include <QWidget>
class WidgetAuthorization : public QWidget {
  Q_OBJECT

 public:
  WidgetAuthorization(WidgetApplicationLogic& logic);

 private:
  WidgetApplicationLogic& widget_application_logic_;
  QLineEdit* line_edit_user_name_login;
  QLineEdit* line_edit_password_login;
  QPushButton* btn_enter_login;
  QLabel* label_info_login;

  QLineEdit* line_edit_user_name_registration;
  QLineEdit* line_edit_password_registration;
  QLineEdit* line_edit_password_confirm_registration;
  QLineEdit* line_edit_email_registration;
  QPushButton* btn_registration;
  QLabel* label_info_registration;

 private slots:
  void LoginDataWasChanged() {
    qDebug() << "LoginDataWasChanged";
    // label_info_login->setStyleSheet("QLabel { color : red; }");
  }

    void RegistrationDataWasChanged() {
    qDebug() << "RegistrationDataWasChanged";
    //
  };