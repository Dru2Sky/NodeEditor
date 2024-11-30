#ifndef NODEEDITOR_GUI_NODESYS_NODEITEM_H
#define NODEEDITOR_GUI_NODESYS_NODEITEM_H

#include <QGraphicsItem>
#include <vector>

namespace ehow {

class PortItem;
class NodeItem : public QGraphicsItem {
    
public:
    explicit NodeItem(const QString& title, QGraphicsItem *parent = nullptr);
    ~NodeItem() override;

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    PortItem* addInputPort(const QString& name);
    PortItem* addOutputPort(const QString& name);
    std::vector<PortItem*> inputPorts() const {
        return _inputPorts;
    }
    std::vector<PortItem*> outputPorts() const {
        return _outputPorts;
    }
    void removePort(PortItem* port);

    QString title() const {
        return _title;
    }
    void setTitle(const QString& title);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

private:
    void updateGeometry();
    void updatePortsPosition();

    void updateCache();
    void paintNodeContent(QPainter* painter, const QRectF& rect);

private:
    QString _title;
    std::vector<PortItem*> _inputPorts;
    std::vector<PortItem*> _outputPorts;

    bool _validCache{false};
    QPixmap _nodeCache;

    static constexpr qreal WIDTH = 150.0;
    static constexpr qreal HEADER_HEIGHT = 30.0;
    static constexpr qreal PORT_SPACING = 20.0;
    
};

}

#endif // NODEEDITOR_GUI_NODESYS_NODEITEM_H