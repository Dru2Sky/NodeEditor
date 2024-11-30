#ifndef NODEEDITOR_GUI_NODESYS_NODEVIEW_H
#define NODEEDITOR_GUI_NODESYS_NODEVIEW_H

#include <QGraphicsView>

namespace ehow {

class NodeView : public QGraphicsView {
    Q_OBJECT

public:
    explicit NodeView(QWidget *parent = nullptr);
    ~NodeView() override;

protected:
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    


private:
    qreal _zoomFactor{ 1.0 };
    QPointF _lastMousePos;
    static constexpr qreal ZOOM_FACTOR_DELTA = 0.1;
};

}

#endif // NODEEDITOR_GUI_NODESYS_NODEVIEW_H
