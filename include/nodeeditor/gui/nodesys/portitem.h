#ifndef NODEEDITOR_GUI_NODESYS_PORTITEM_H
#define NODEEDITOR_GUI_NODESYS_PORTITEM_H

#include <QGraphicsItem>

namespace ehow {

class NodeItem;
class ConnectionItem;
class PortItem : public QGraphicsItem {

public:
    enum class PortType {   
        INPUT,
        OUTPUT
    };

    explicit PortItem(PortType type, const QString& name, QGraphicsItem *parent = nullptr);
    ~PortItem() override;

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    PortType portType() const {
        return _type;
    }

    QString name() const {
        return _name;
    }

    NodeItem* node() const;

    ConnectionItem* connection() const {
        return _connection;
    }

    void setConnection(ConnectionItem* connection);

    QPointF connectionPoint() const;

    void setHighlighted(bool highlighted);
    bool isHighlighted() const {
        return _isHighlighted;
    }

    bool isInSnapRange(const QPointF& scenePos) const;

protected:
    // void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    // void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    // void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

private:
    // void initUI();
private:
    PortType _type;
    QString _name;
    ConnectionItem* _connection;
    ConnectionItem* _draggingConnection;

    bool _isHighlighted{false};

    static constexpr qreal RADIUS = 5.0;
    static constexpr qreal WIDTH = 10.0;
    static constexpr qreal HEIGHT = 18.0;
    static constexpr qreal SNAP_DISTANCE = 20.0;
};

}

#endif // NODEEDITOR_GUI_NODESYS_PORTITEM_H