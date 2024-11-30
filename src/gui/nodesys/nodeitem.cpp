#include "nodeeditor/gui/nodesys/nodeitem.h"
#include "nodeeditor/gui/nodesys/portitem.h"
#include "nodeeditor/gui/nodesys/connectionitem.h"

#include <QPainter>
#include <QGraphicsScene>


namespace ehow {

NodeItem::NodeItem(const QString& title, QGraphicsItem *parent) 
    : QGraphicsItem(parent)
    , _title(title)
{
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
}

NodeItem::~NodeItem() {

}


QRectF NodeItem::boundingRect() const {
    const qreal h = HEADER_HEIGHT + 
        qMax(_inputPorts.size(), _outputPorts.size()) * PORT_SPACING;
    return QRectF(0, 0, WIDTH, h);
}

void NodeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    updateCache();

    painter->drawPixmap(0, 0, _nodeCache);

    if(isSelected()) {
        painter->setPen(QPen(Qt::yellow, 2));
        painter->drawRect(boundingRect());
    }
}

PortItem* NodeItem::addInputPort(const QString& name) {
    PortItem* port = new PortItem(PortItem::PortType::INPUT, name, this);
    _inputPorts.push_back(port);
    updateGeometry();
    return port;
}

PortItem* NodeItem::addOutputPort(const QString& name) {
    PortItem* port = new PortItem(PortItem::PortType::OUTPUT, name, this);
    _outputPorts.push_back(port);
    updateGeometry();
    return port;
}

void NodeItem::removePort(PortItem* port) {
    if(!port) return;
    if(port->portType() == PortItem::PortType::INPUT) {
        _inputPorts.erase(std::remove(_inputPorts.begin(), _inputPorts.end(), port), _inputPorts.end());
    } else {
        _outputPorts.erase(std::remove(_outputPorts.begin(), _outputPorts.end(), port), _outputPorts.end());
    }
    delete port;
    updateGeometry();
}

void NodeItem::setTitle(const QString& title) {
    if(_title == title) return;
    _title = title;
    update();
}

QVariant NodeItem::itemChange(GraphicsItemChange change, const QVariant &value) {
    if (change == ItemPositionChange) {
        if(scene()) {
            scene()->update();
        }
        const QPointF delta = value.toPointF() - pos();

        if(delta.manhattanLength() > 1.0) {
// 更新所有连接的路径
        for (PortItem* port : _inputPorts) {
            if (port->connection())
                port->connection()->setNeedsUpdate();
        }
        for (PortItem* port : _outputPorts) {
            if (port->connection())
                port->connection()->setNeedsUpdate();
            }
        }
        
    }
    return QGraphicsItem::itemChange(change, value);
}

void NodeItem::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    setZValue(1); //将节点置于最顶层
    QGraphicsItem::mousePressEvent(event);
}

void NodeItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
    setZValue(0); //恢复默认层级
    QGraphicsItem::mouseReleaseEvent(event);
}

void NodeItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    QGraphicsItem::mouseMoveEvent(event);
}

void NodeItem::updateGeometry() {
    prepareGeometryChange();
    updatePortsPosition();
    update();
}

void NodeItem::updatePortsPosition() {
    qreal y = HEADER_HEIGHT;
    
    // 更新输入端口位置
    for (PortItem* port : _inputPorts) {
        port->setPos(0, y);
        y += PORT_SPACING;
    }
    
    // 更新输出端口位置
    y = HEADER_HEIGHT;
    for (PortItem* port : _outputPorts) {
        port->setPos(WIDTH, y);
        y += PORT_SPACING;
    }
}

void NodeItem::updateCache() {
    if(_validCache) {
        return;
    }

    const QRectF& rect = boundingRect();
    _nodeCache = QPixmap(rect.size().toSize());
    _nodeCache.fill(Qt::transparent);

    QPainter painter(&_nodeCache);
    painter.setRenderHint(QPainter::Antialiasing);

    paintNodeContent(&painter, rect);
    _validCache = true;
}

void NodeItem::paintNodeContent(QPainter* painter, const QRectF& rect) {
    //绘制节点主体
    // const QRectF& rect = boundingRect();

    //绘制阴影
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(20, 20, 20, 150));
    painter->drawRoundedRect(rect.translated(3, 3), 5, 5);

    //绘制主体
    QLinearGradient gradient(rect.topLeft(), rect.bottomRight());
    gradient.setColorAt(0, QColor(60, 60, 60));
    gradient.setColorAt(1, QColor(30, 30, 30));

    painter->setBrush(gradient);
    painter->setPen(isSelected() ? QPen(Qt::white, 2) : QPen(QColor(30, 30, 30), 1));
    painter->drawRoundedRect(rect, 5, 5);

    // 绘制标题栏
    QRectF titleRect = rect.adjusted(0, 0, 0, -rect.height() + HEADER_HEIGHT);
    gradient = QLinearGradient(titleRect.topLeft(), titleRect.bottomLeft());
    gradient.setColorAt(0, QColor(80, 80, 80));
    gradient.setColorAt(1, QColor(60, 60, 60));
    
    painter->setBrush(gradient);
    painter->setPen(Qt::NoPen);
    painter->drawRoundedRect(titleRect, 5, 5);
    
    // 绘制标题文本
    painter->setPen(Qt::white);
    painter->setFont(QFont("Arial", 10, QFont::Bold));
    painter->drawText(titleRect.adjusted(10, 0, -10, 0), 
                     Qt::AlignVCenter | Qt::AlignLeft, 
                     _title);
}

}