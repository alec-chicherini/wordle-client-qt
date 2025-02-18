#include <QApplication>
#include <QIcon>
#include <WidgetGame.h>
int main(int argc, char **argv)
{
    QApplication app (argc, argv);
    app.setWindowIcon(QIcon::fromTheme("wordle-task"));
    WidgetGame game;
    game.show();

    return app.exec();
}
