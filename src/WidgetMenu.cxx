#include <WidgetMenu.h>

#include <QDebug>
#include <QVBoxLayout>
#include <QPushButton>

WidgetMenu::WidgetMenu()
{
    [[maybe_unused]] bool connected;
    QVBoxLayout* qVBoxLayoutMenu = new QVBoxLayout(this);
    qVBoxLayoutMenu->setAlignment(Qt::AlignCenter);

    QPushButton* btnPlay = new QPushButton("Играть");
    qVBoxLayoutMenu->addWidget(btnPlay);

    QPushButton* btnStatistic = new QPushButton("Статистика");
    qVBoxLayoutMenu->addWidget(btnStatistic);

    QPushButton* btnProfile = new QPushButton("Профиль");
    qVBoxLayoutMenu->addWidget(btnProfile);

    QPushButton* btnLogOut = new QPushButton("Выход");
    qVBoxLayoutMenu->addWidget(btnLogOut);
};