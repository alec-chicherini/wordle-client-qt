#pragma once
#include <APIApplicationLogic.h>
#include <WidgetApplicationLogic.h>
#include <QLabel>
#include <QLineEdit>
#include <QObject>
#include <QPushButton>
#include <QWidget>
#include <QAtomicInt>
class WidgetAuthorization : public QWidget {
  Q_OBJECT

 public:
  WidgetAuthorization(WidgetApplicationLogic& widget_logic,
                      APIApplicationLogic& api_logic);

 private:
  WidgetApplicationLogic& widget_application_logic_;
  APIApplicationLogic& api_application_logic_;

 private:
  QLineEdit* line_edit_user_name_login;
  QLineEdit* line_edit_password_login;
  QPushButton* btn_enter_login;
  QLabel* label_info_login;
  QAtomicInt count_request_login; 

  QLineEdit* line_edit_user_name_registration;
  QLineEdit* line_edit_password_registration;
  QLineEdit* line_edit_password_confirm_registration;
  QLineEdit* line_edit_email_registration;
  QPushButton* btn_registration;
  QLabel* label_info_registration;
  QAtomicInt count_request_registration; 

 private slots:
  void LoginDataWasChanged();
  void RegistrationDataWasChanged();

 public slots:
  void ProcessLogOut();
  void ProcessLogin(LoginStatus status);
  void ProcessRegistration(RegistrationStatus status);
};