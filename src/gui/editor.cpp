#include "nodeeditor/gui/editor.h"
#include "nodeeditor/gui/nodesys/nodescene.h"
#include "nodeeditor/gui/nodesys/nodeitem.h"
#include "nodeeditor/gui/nodesys/nodeview.h"

#include <QVBoxLayout>
namespace ehow {

Editor::Editor(QWidget *parent) : QWidget(parent) {
    m_scene = new NodeScene();
    m_view = new NodeView();

    m_view->setScene(m_scene);

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(m_view);
    setLayout(layout);
    
    // initUI();
}

Editor::~Editor() {

}

}