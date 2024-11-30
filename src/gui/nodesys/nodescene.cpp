#include "nodeeditor/gui/nodesys/nodescene.h"
#include "nodeeditor/gui/nodesys/nodeitem.h"
#include "nodeeditor/gui/nodesys/portitem.h"
#include "nodeeditor/gui/nodesys/connectionitem.h"
#include "nodeeditor/gui/nodesys/nodescene.h"

#include <QGraphicsSceneDragDropEvent>
#include <QGraphicsSceneContextMenuEvent>
#include <QMimeData>
#include <QMenu>
#include <QPainter>

namespace ehow {

NodeScene::NodeScene(QObject *parent) 
    : QGraphicsScene(parent)
{
    setBackgroundBrush(QBrush(QColor(50, 50, 50)));
    // 设置场景大小
    setSceneRect(-1000, -1000, 2000, 2000);

}

NodeScene::~NodeScene() {

}

void NodeScene::createNode(const QString& type, const QPointF& pos) {
    NodeItem* node = new NodeItem(type);
    node->setPos(pos);

    if(type == "Add") {
        node->addInputPort("In1");
        node->addInputPort("In2");
        node->addOutputPort("Out");
    } else if(type == "Sub") {
        node->addInputPort("In1");
        node->addInputPort("In2");
        node->addOutputPort("Out");
    }

    addItem(node);
    _nodes.push_back(node);
}

void NodeScene::removeNode(NodeItem* node) {
    if(!node) return;
    // 移除所有连接
    for(ConnectionItem* connection : _connections) {
        if(connection->startPort()->node() == node || 
            connection->endPort()->node() == node) {
            removeConnection(connection);
        }
    }
    removeItem(node);
    _nodes.erase(std::remove(_nodes.begin(), _nodes.end(), node), _nodes.end());
    delete node;
}

void NodeScene::removeConnection(ConnectionItem* connection) {
    if(!connection) return;
    removeItem(connection);
    _connections.erase(std::remove(_connections.begin(), _connections.end(), connection), _connections.end());
    delete connection;
}

void NodeScene::contextMenuEvent(QGraphicsSceneContextMenuEvent* event) {
    QGraphicsItem* item = itemAt(event->scenePos(), QTransform());

    if(item && item->type() == NodeItem::Type) {
        QMenu menu;
        menu.addAction("Delete", this, [this, item]() {
            removeNode(static_cast<NodeItem*>(item));
        });
        menu.addAction("Duplicate", this, [this, item]() {
            // createNode(static_cast<NodeItem*>(item)->type(), item->scenePos());
        });
        menu.exec(event->screenPos());
    } else {
        createContextMenu(event->scenePos());
    }
}

void NodeScene::createContextMenu(const QPointF& pos) {
    QMenu menu;
    menu.addMenu(buildNodeCreationMenu());
    menu.exec(QCursor::pos());
}

void NodeScene::dragEnterEvent(QGraphicsSceneDragDropEvent* event) {
    if (event->mimeData()->hasFormat("application/x-node")) {
        event->acceptProposedAction();
    }
}

void NodeScene::dragMoveEvent(QGraphicsSceneDragDropEvent* event) {
    if (event->mimeData()->hasFormat("application/x-node")) {
        event->acceptProposedAction();
    }
}

void NodeScene::dropEvent(QGraphicsSceneDragDropEvent* event) {
    if (event->mimeData()->hasFormat("application/x-node")) {
        QString nodeType = event->mimeData()->data("application/x-node");
        createNode(nodeType, event->scenePos());
        event->acceptProposedAction();
    }
}

void NodeScene::drawBackground(QPainter* painter, const QRectF& rect) {
    QGraphicsScene::drawBackground(painter, rect);

    if(_gridPixmap.isNull()) {
        createGridPixmap();
    }
    // 使用缓存的网格图片平铺背景
    const int gridSize = static_cast<int>(GRID_STEP * 5);
    const QRect aligned = rect.toAlignedRect();
    
    const int startX = aligned.left() - (aligned.left() % gridSize);
    const int startY = aligned.top() - (aligned.top() % gridSize);
    
    for (int x = startX; x < aligned.right(); x += gridSize) {
        for (int y = startY; y < aligned.bottom(); y += gridSize) {
            painter->drawPixmap(x, y, _gridPixmap);
        }
    }
}

void NodeScene::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    if(event->button() == Qt::LeftButton) {
        auto item = itemAt(event->scenePos(), QTransform());

        if(PortItem* port = dynamic_cast<PortItem*>(item)) {
            if(port->portType() == PortItem::PortType::OUTPUT && !port->connection()) {
                //创建连接
                _draggingConnection = new ConnectionItem(port);
                addItem(_draggingConnection);
                event->accept();
                return;
            }
        }
    }
    QGraphicsScene::mousePressEvent(event);
}
void NodeScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    if(_draggingConnection) {
        updateDraggingConnection(event->scenePos());
        event->accept();
        return;
    }
    QGraphicsScene::mouseMoveEvent(event);
}
void NodeScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
    if(_draggingConnection) {
        finishConnection(event->scenePos());
        _draggingConnection = nullptr;
        event->accept();
        return;
    }
    QGraphicsScene::mouseReleaseEvent(event);
}

QMenu* NodeScene::buildNodeCreationMenu() {
    QMenu* menu = new QMenu;
    menu->addAction("Add Node", this, [this]() {
        createNode("Add", QPointF(0, 0));
    });
    menu->addAction("Sub Node", this, [this]() {
        createNode("Sub", QPointF(0, 0));
    });
    return menu;
}


void NodeScene::drawGrid(QPainter* painter, const QRectF& rect, qreal step) {
    qreal left = int(rect.left()) - (int(rect.left()) % int(step));
    qreal top = int(rect.top()) - (int(rect.top()) % int(step));

    QVarLengthArray<QLineF, 100> lines;

    for (qreal x = left; x < rect.right(); x += step)
        lines.append(QLineF(x, rect.top(), x, rect.bottom()));
    for (qreal y = top; y < rect.bottom(); y += step)
        lines.append(QLineF(rect.left(), y, rect.right(), y));

    painter->drawLines(lines.data(), lines.size());
}

void NodeScene::createGridPixmap() {
    // 创建网格缓存
    const int size = static_cast<int>(GRID_STEP * 5);  // 缓存 5x5 的网格单元
    _gridPixmap = QPixmap(size, size);
    _gridPixmap.fill(backgroundBrush().color());

    QPainter painter(&_gridPixmap);
    painter.setPen(QPen(QColor(60, 60, 60), 0.5));

    //绘制细网格
    for (int i = 0; i < size; i += static_cast<int>(GRID_STEP_SMALL)) {
        painter.drawLine(i, 0, i, size);
        painter.drawLine(0, i, size, i);
    }
    //绘制主网格
    painter.setPen(QPen(QColor(80, 80, 80), 1));
    for (int i = 0; i < size; i += static_cast<int>(GRID_STEP)) {
        painter.drawLine(i, 0, i, size);
        painter.drawLine(0, i, size, i);
    }
}

void NodeScene::updateDraggingConnection(const QPointF& scenePos) {
    for(NodeItem* node : _nodes) {
        for(PortItem* port : node->inputPorts()) {
            port->setHighlighted(false);
        }
    }

    auto item = itemAt(scenePos, QTransform());
    if(PortItem* port = dynamic_cast<PortItem*>(item)) {
        if(port->portType() == PortItem::PortType::INPUT && !port->connection()) {
            if(port->isInSnapRange(scenePos)) {
                port->setHighlighted(true);
                _draggingConnection->setEndPoint(port->connectionPoint());
                return;
            }
        }
    }

    _draggingConnection->setEndPoint(scenePos);
}

void NodeScene::finishConnection(const QPointF& scenePos) {
    QGraphicsItem* item = itemAt(scenePos, QTransform());
    if (PortItem* port = qgraphicsitem_cast<PortItem*>(item)) {
        if (port->portType() == PortItem::PortType::INPUT && 
            !port->connection() && 
            port->isHighlighted()) {
            // 完成连接
            _draggingConnection->setPending(false);
            _draggingConnection->setEndPort(port);
            _connections.push_back(_draggingConnection);
        } else {
            // 删除临时连接
            removeItem(_draggingConnection);
            delete _draggingConnection;
        }
    } else {
        // 删除临时连接
        removeItem(_draggingConnection);
        delete _draggingConnection;
    }
    
    // 清除所有端口的高亮状态
    for (NodeItem* node : _nodes) {
        for (PortItem* port : node->inputPorts()) {
            port->setHighlighted(false);
        }
    }
}
}