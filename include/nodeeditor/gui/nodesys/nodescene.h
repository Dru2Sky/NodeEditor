#ifndef NODEEDITOR_GUI_NODESYS_NODESCENE_H
#define NODEEDITOR_GUI_NODESYS_NODESCENE_H

#include <QGraphicsScene>
#include <vector>


class QMenu;

namespace ehow {

class NodeItem;
class ConnectionItem;
class NodeScene : public QGraphicsScene {
    Q_OBJECT

public:
    explicit NodeScene(QObject *parent = nullptr);
    ~NodeScene() override;  

    void createNode(const QString& type, const QPointF& pos = QPointF(0, 0));
    void removeNode(NodeItem* node);
    void removeConnection(ConnectionItem* connection);

    std::vector<NodeItem*>& nodes() {
        return _nodes;
    }
    std::vector<ConnectionItem*>& connections() {
        return _connections;
    }
protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent* event) override;
    void dragEnterEvent(QGraphicsSceneDragDropEvent* event) override;
    void dragMoveEvent(QGraphicsSceneDragDropEvent* event) override;
    void dropEvent(QGraphicsSceneDragDropEvent* event) override;
    void drawBackground(QPainter* painter, const QRectF& rect) override;

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
private:
    void createContextMenu(const QPointF& pos);
    QMenu* buildNodeCreationMenu();
    void drawGrid(QPainter* painter, const QRectF& rect, qreal step);
    void createGridPixmap();


    void updateDraggingConnection(const QPointF& scenePos);
    void finishConnection(const QPointF& scenePos);
private:
    std::vector<NodeItem*> _nodes;

    ConnectionItem* _draggingConnection{nullptr};
    std::vector<ConnectionItem*> _connections;
    
    QPixmap _gridPixmap;


    static constexpr qreal GRID_STEP = 20.0;
    static constexpr qreal GRID_STEP_SMALL = 4.0;
};

}

#endif // NODEEDITOR_GUI_NODESYS_NODESCENE_H