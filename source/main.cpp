#include "headers/generator.hpp"

#include <QtWidgets/QApplication>

int main(int argc, char *argv[]){
    QApplication app(argc, argv);
    Generator w;
    w.show();
    return app.exec();
}