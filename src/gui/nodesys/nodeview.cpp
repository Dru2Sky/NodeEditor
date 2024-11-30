#include "nodeeditor/gui/nodesys/nodeview.h"

#include <QWheelEvent>
#include <QMouseEvent>
#include <QScrollBar>

namespace ehow {

NodeView::NodeView(QWidget *parent) 
    : QGraphicsView(parent) 
{
    // 设置渲染属性
    setRenderHints(QPainter::Antialiasing | 
                   QPainter::TextAntialiasing | 
                   QPainter::SmoothPixmapTransform);
                   
    // 设置拖拽模式
    setDragMode(QGraphicsView::RubberBandDrag);
    
    // 设置视图属性
    setViewportUpdateMode(QGraphicsView::MinimalViewportUpdate);
    setCacheMode(QGraphicsView::CacheBackground);
    setOptimizationFlag(QGraphicsView::DontAdjustForAntialiasing, true);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    
    // 设置变换锚点
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setResizeAnchor(QGraphicsView::AnchorUnderMouse);
}

NodeView::~NodeView() {

}


void NodeView::wheelEvent(QWheelEvent *event) {

    if (event->modifiers() & Qt::ControlModifier) {
        // 缩放
        const qreal delta = event->angleDelta().y();
        if (delta > 0) {
            _zoomFactor += ZOOM_FACTOR_DELTA;
        } else {
            _zoomFactor -= ZOOM_FACTOR_DELTA;
        }
        
        _zoomFactor = qBound(0.1, _zoomFactor, 2.0);
        setTransform(QTransform::fromScale(_zoomFactor, _zoomFactor));
    } else {
        // 正常滚动
        QGraphicsView::wheelEvent(event);
    }
}

void NodeView::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::MiddleButton) {
        // 开始平移
        setDragMode(QGraphicsView::ScrollHandDrag);
        _lastMousePos = event->pos();
        event->accept();
    }
    QGraphicsView::mousePressEvent(event);
}

void NodeView::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::MiddleButton) {
        setDragMode(QGraphicsView::RubberBandDrag);
    }
    QGraphicsView::mouseReleaseEvent(event);
}

void NodeView::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::MiddleButton) {
        // 平移视图
        QPointF delta = event->pos() - _lastMousePos;
        _lastMousePos = event->pos();
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - delta.x());
        verticalScrollBar()->setValue(verticalScrollBar()->value() - delta.y());
        event->accept();
    }
    QGraphicsView::mouseMoveEvent(event);
}

}