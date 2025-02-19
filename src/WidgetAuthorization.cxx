#include <WidgetAuthorization.h>

#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QRegularExpressionValidator>
#include <QPushButton>
#include <QFrame>
#include <QLabel>

WidgetAuthorization::WidgetAuthorization(WidgetApplicationLogic& logic):widget_application_logic_(logic)
{
    [[maybe_unused]] bool connected;

    QVBoxLayout* qVBoxLayoutAuthorizationTop = new QVBoxLayout(this);
    qVBoxLayoutAuthorizationTop->setAlignment(Qt::AlignCenter);
    QWidget* widget_aligned_center = new QWidget;
    widget_aligned_center->setMaximumWidth(330);
    qVBoxLayoutAuthorizationTop->addWidget(widget_aligned_center);
    
    QVBoxLayout* qVBoxLayoutAuthorization = new QVBoxLayout(widget_aligned_center);
    {
        QLabel* labelLogin = new QLabel("Вход");
        qVBoxLayoutAuthorization->addWidget(labelLogin);

        QLineEdit* lineEditUserName = new QLineEdit();
        lineEditUserName->setPlaceholderText("Имя пользователя");
        qVBoxLayoutAuthorization->addWidget(lineEditUserName);

        QLineEdit* lineEditPassword = new QLineEdit();
        lineEditPassword->setPlaceholderText("Пароль");
        lineEditPassword->setEchoMode(QLineEdit::Password);
        qVBoxLayoutAuthorization->addWidget(lineEditPassword);

        QPushButton* btn_enter = new QPushButton("Войти");
        qVBoxLayoutAuthorization->addWidget(btn_enter);

        connected = QObject::connect(btn_enter, &QPushButton::clicked, &widget_application_logic_, [=, this]() { widget_application_logic_.GoTo(WidgetApplicationLogic::State::kMenu); });
        Q_ASSERT_X(connected, std::string(std::string(__FILE__) + " line:" + std::to_string(__LINE__)).c_str(), "connected is FALSE");
    }

    QFrame* line = new QFrame();
    line->setFrameShape(QFrame::HLine); // Vertical line
    line->setFrameShadow(QFrame::Sunken);
    line->setLineWidth(2);
    qVBoxLayoutAuthorization->addWidget(line);

    {
        QLabel* labelRegistration = new QLabel("Регистрация");
        qVBoxLayoutAuthorization->addWidget(labelRegistration);

        QLineEdit* lineEditUserName = new QLineEdit();
        lineEditUserName->setPlaceholderText("Имя пользователя");
        constexpr int kUserNameMaxLength = 32;
        lineEditUserName->setMaxLength(kUserNameMaxLength);
        qVBoxLayoutAuthorization->addWidget(lineEditUserName);

        //https://www.book2s.com/tutorials/qt-qregularexpressionvalidator.html
        // Create a QRegularExpressionValidator with a pattern for password validation
        // The above regular expression pattern enforces the following criteria:
        // - At least 8 characters long
        // - Contains at least one digit [0-9]
        // - Contains at least one lowercase letter [a-z]
        // - Contains at least one uppercase letter [A-Z]
        // - Contains at least one special character [@#$%^&+=]
        // - Does not contain whitespace characters
        QRegularExpressionValidator* validatorPassword = new QRegularExpressionValidator(QRegularExpression("^(?=.*[0-9])(?=.*[a-z])(?=.*[A-Z])(?=.*[@#$%^&+=])(?=\\S+$).{8,}$"));
        constexpr int kPasswordMaxLength = 16;
        QLineEdit* lineEditPassword = new QLineEdit();
        lineEditPassword->setPlaceholderText("Пароль");
        lineEditPassword->setEchoMode(QLineEdit::Password);
        lineEditPassword->setValidator(validatorPassword);
        lineEditPassword->setMaxLength(kPasswordMaxLength);
        qVBoxLayoutAuthorization->addWidget(lineEditPassword);

        QLineEdit* lineEditPasswordConfirm = new QLineEdit();
        lineEditPasswordConfirm->setPlaceholderText("Подтверждение пароля");
        lineEditPasswordConfirm->setEchoMode(QLineEdit::Password);
        lineEditPasswordConfirm->setValidator(validatorPassword);
        lineEditPasswordConfirm->setMaxLength(kPasswordMaxLength);
        qVBoxLayoutAuthorization->addWidget(lineEditPasswordConfirm);

        QRegularExpressionValidator* validatorEMail = new QRegularExpressionValidator(QRegularExpression("[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}"));
        QLineEdit* lineEditEMAil = new QLineEdit();
        lineEditEMAil->setValidator(validatorEMail);
        lineEditEMAil->setPlaceholderText("Почта");
        qVBoxLayoutAuthorization->addWidget(lineEditEMAil);

        QPushButton* btn_register = new QPushButton("Зарегистрировать");
        qVBoxLayoutAuthorization->addWidget(btn_register);

        connected = QObject::connect(btn_register, &QPushButton::clicked, &widget_application_logic_, [=, this]() { widget_application_logic_.GoTo(WidgetApplicationLogic::State::kMenu); });
        Q_ASSERT_X(connected, std::string(std::string(__FILE__) + " " + std::to_string(__LINE__)).c_str(), "connected is FALSE");
    }
};