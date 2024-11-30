#include "nodeeditor/gui/nodesys/portitem.h"
#include "nodeeditor/gui/nodesys/connectionitem.h"
#include "nodeeditor/gui/nodesys/nodeitem.h"

#include <QPainter>
#include <QPen>
#include <QFont>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QDebug>

namespace ehow {

PortItem::PortItem(PortType type, const QString& name, QGraphicsItem *parent) 
    : QGraphicsItem(parent)
    , _type(type)
    , _name(name) 
    , _connection(nullptr) 
{
    setAcceptHoverEvents(true);
}

PortItem::~PortItem() 
{

}

QRectF PortItem::boundingRect() const {
    return QRectF(-WIDTH / 2, -HEIGHT / 2, WIDTH, HEIGHT);
}

void PortItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    // 绘制端口圆点
    painter->setBrush(isHighlighted() ? Qt::yellow : 
                     (connection() ? Qt::green : Qt::red));
    painter->setPen(QPen(Qt::black, 1));
    painter->drawEllipse(-RADIUS, -RADIUS, 2*RADIUS, 2*RADIUS);
    
    
    // 绘制端口名称
    painter->setPen(Qt::white);
    painter->setFont(QFont("Arial", 8));
    
    QRectF textRect;
    if (portType() == PortType::INPUT) {
        textRect = QRectF(RADIUS + 5, -HEIGHT/2, WIDTH * 8, HEIGHT);
    } else {
        textRect = QRectF(-WIDTH * 8 - RADIUS - 5, -HEIGHT/2, WIDTH * 8, HEIGHT);
    }
    
    painter->drawText(textRect, 
                     portType() == PortType::INPUT ? Qt::AlignLeft : Qt::AlignRight, 
                     name());
}

NodeItem* PortItem::node() const {
    return dynamic_cast<NodeItem*>(parentItem());
}

void PortItem::setConnection(ConnectionItem* connection) {
    if (connection == _connection) {
        return;
    }
    _connection = connection;
}

QPointF PortItem::connectionPoint() const {
    return mapToScene(QPointF(0, 0));
}

void PortItem::setHighlighted(bool highlighted) {
    if(highlighted == _isHighlighted) {
        return;
    }
    _isHighlighted = highlighted;
    update();
}

bool PortItem::isInSnapRange(const QPointF& scenePos) const {
    return (mapToScene(QPointF(0, 0)) - scenePos).manhattanLength() < SNAP_DISTANCE;
}

// void PortItem::mousePressEvent(QGraphicsSceneMouseEvent* event) {
//     if (event->button() == Qt::LeftButton) {
//         // 创建临时连接线
//         if (portType() == PortType::OUTPUT && !connection()) {
//             _draggingConnection = new ConnectionItem(this);
//             scene()->addItem(_draggingConnection);
//         }
//     }
//     QGraphicsItem::mousePressEvent(event);
// }

// void PortItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
//     if (_draggingConnection) {
//         // 检查是否释放在另一个端口上
//         auto items = scene()->items(event->scenePos());
//         PortItem* targetPort = nullptr;
        
//         for (auto&& item : items) {
//             if (PortItem* port = qgraphicsitem_cast<PortItem*>(item)) {
//                 if (port != this && port->portType() == PortType::INPUT && !port->connection()) {
//                     if(port->isHighlighted()) {
//                         targetPort = port;
//                         break;
//                     }
//                 }
//             }
//         }
        
//         if (targetPort) {
//             // 创建正式连接
//             _draggingConnection->setPending(false);
//             _draggingConnection->setEndPort(targetPort);
//             targetPort->setHighlighted(false);
            
//         } else {
//             // 删除临时连接
//             scene()->removeItem(_draggingConnection);
//             delete _draggingConnection;
//         }

//         _draggingConnection = nullptr;
//     }
//     QGraphicsItem::mouseReleaseEvent(event);
// }

// void PortItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
//     if (_draggingConnection) {

//         auto items = scene()->items(event->scenePos());
//         bool foundValidPort = false;

//         qInfo() << __FUNCTION__ << "items size:" << items.size();
//         for(auto&& item : items) {
//             if(PortItem* port = qgraphicsitem_cast<PortItem*>(item)) {
//                 if(port != this && port->portType() == PortType::INPUT && !port->connection()) {
//                     port->setHighlighted(port->isInSnapRange(event->scenePos()));
//                     if(port->isHighlighted()) {
//                         _draggingConnection->setEndPort(port);
//                         foundValidPort = true;
//                         break;
//                     }
//                 }
//             }
//         }

//         if(!foundValidPort) {
//             _draggingConnection->setEndPoint(event->scenePos());
//         }
//     }
//     QGraphicsItem::mouseMoveEvent(event);
    
// }

}