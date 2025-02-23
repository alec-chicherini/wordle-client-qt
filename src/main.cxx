#include <QApplication>
#include <QIcon>
#include <WidgetApplication.h>
int main(int argc, char **argv)
{
    QApplication app (argc, argv);
    app.setWindowIcon(QIcon::fromTheme("wordle-client-qt"));
    WidgetApplication application;
    application.show();

    return app.exec();
}
