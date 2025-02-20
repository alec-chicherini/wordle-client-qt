#include <APIApplicationLogic.h>

APIApplicationLogic::APIApplicationLogic() {}
void APIApplicationLogic::RequestLogin(const QString& user_name,
                                       const QString& password) {
  emit ResponseLogin(LoginStatus::kLoginIsOk);
};