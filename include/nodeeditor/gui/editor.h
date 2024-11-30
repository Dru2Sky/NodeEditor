#ifndef NODEEDITOR_GUI_EDITOR_H
#define NODEEDITOR_GUI_EDITOR_H

#include <QWidget>

namespace ehow {

class NodeScene;
class NodeView;
class Editor : public QWidget {
    Q_OBJECT

public:
    explicit Editor(QWidget *parent = nullptr);
    ~Editor() override;

private:
    NodeScene* m_scene;
    NodeView* m_view;
};

}

#endif // NODEEDITOR_GUI_EDITOR_H