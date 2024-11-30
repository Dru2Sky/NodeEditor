#ifndef NODEEDITOR_GUI_NODESYS_CONNECTIONITEM_H
#define NODEEDITOR_GUI_NODESYS_CONNECTIONITEM_H

#include <QGraphicsPathItem>

namespace ehow {

class PortItem;
class ConnectionItem : public QGraphicsPathItem {
    
public:
    explicit ConnectionItem(PortItem* startPort, PortItem* endPort = nullptr);
    ~ConnectionItem() override;

    void updatePath();
    void setEndPort(PortItem* endPort);
    void setEndPoint(const QPointF& endPoint);

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    bool needsUpdate() const {
        return _needsUpdate;
    }
    void setNeedsUpdate(bool update = true) {
        _needsUpdate = update;
    }

    PortItem* startPort() const {
        return _startPort;
    }
    PortItem* endPort() const {
        return _endPort;
    }

    bool isPending() const {
        return _isPending;
    }
    void setPending(bool pending = true) {
        if(_isPending == pending) {
            return;
        }
        _isPending = pending;
        update();
    }
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

private:
    QPainterPath createPath(const QPointF& start, const QPointF& end) const;

private:
    PortItem* _startPort{nullptr};
    PortItem* _endPort{nullptr};
    QPointF _endPoint;
    bool _needsUpdate;
    QPainterPath _cachePath;
    bool _isPending{false};
};

}

#endif // NODEEDITOR_GUI_NODESYS_CONNECTIONITEM_H