#include <QApplication>
#include <ui/Welcome.h>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    Welcome w(argc == 2 ? argv[1][0] : 0);
    w.show();
    return QApplication::exec();
}
