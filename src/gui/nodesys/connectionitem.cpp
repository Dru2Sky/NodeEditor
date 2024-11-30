#include "nodeeditor/gui/nodesys/connectionitem.h"
#include "nodeeditor/gui/nodesys/portitem.h"
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>

namespace ehow {

ConnectionItem::ConnectionItem(PortItem* startPort, PortItem* endPort) 
    : QGraphicsPathItem(nullptr)
    , _startPort(startPort)
    , _endPort(endPort)
{
    setZValue(1); // 确保连接线在节点上方

    setPen(QPen(QColor(200, 200, 200), 2));

    setFlag(QGraphicsItem::ItemIsSelectable);

    if (_startPort) {
        _startPort->setConnection(this);
    }
    if (_endPort) {
        _endPort->setConnection(this);
    }

    setPending(!_endPort);
}

ConnectionItem::~ConnectionItem() {

}

void ConnectionItem::updatePath() {
    if(!_startPort) return;

    if(!needsUpdate()) return;

    QPointF start = _startPort->connectionPoint();
    QPointF end = _endPort ? _endPort->connectionPoint() : _endPoint;
    
    auto path = createPath(start, end);
    setPath(path);
    _cachePath = path;
    setNeedsUpdate(false);
}

void ConnectionItem::setEndPort(PortItem* endPort) {
    if(_endPort) {
        _endPort->setConnection(nullptr);
    }
    _endPort = endPort;
    if(_endPort) {
        _endPort->setConnection(this);
        updatePath();
    }
}


void ConnectionItem::setEndPoint(const QPointF& endPoint) {
    _endPoint = endPoint;
    updatePath();
}

void ConnectionItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    painter->setRenderHint(QPainter::Antialiasing);

    if(needsUpdate()) {
        updatePath();
    }

    QPen pen = this->pen();
    if(isPending()) {
        pen.setStyle(Qt::DashLine);
    }
    if (isSelected()) {
        pen.setColor(Qt::yellow);
        pen.setWidth(pen.width() + 1);
    }
    
    painter->setPen(pen);
    painter->drawPath(_cachePath);
}

QPainterPath ConnectionItem::createPath(const QPointF& start, const QPointF& end) const {
    QPainterPath path;
    path.moveTo(start);
    
    qreal dx = end.x() - start.x();
    qreal dy = end.y() - start.y();
    
    QPointF ctrl1(start.x() + dx * 0.5, start.y());
    QPointF ctrl2(end.x() - dx * 0.5, end.y());
    
    path.cubicTo(ctrl1, ctrl2, end);
    return path;
}

void ConnectionItem::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    if (event->button() == Qt::RightButton) {
        // 删除连接
        if (_startPort)
            _startPort->setConnection(nullptr);
        if (_endPort)
            _endPort->setConnection(nullptr);
        scene()->removeItem(this);
        delete this;
    }
    QGraphicsPathItem::mousePressEvent(event);
}

}