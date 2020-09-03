#include <QApplication>
#include <ui/Welcome.h>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    Welcome w;
    w.show();
    return QApplication::exec();
}
