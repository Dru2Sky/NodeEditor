#include <QApplication>

#include "nodeeditor/gui/editor.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    ehow::Editor editor;
    editor.resize(800, 600);
    editor.show();

    return app.exec();
}
