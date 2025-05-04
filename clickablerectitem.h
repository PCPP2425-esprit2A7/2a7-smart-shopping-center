#ifndef CLICKABLERECTITEM_H
#define CLICKABLERECTITEM_H

#include <QGraphicsRectItem>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>
#include <QObject>

class ClickableRectItem : public QObject, public QGraphicsRectItem
{
    Q_OBJECT

public:
    explicit ClickableRectItem(QGraphicsItem *parent = nullptr);

signals:
    void hoverEntered();
    void hoverLeft();
    void clicked();

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // CLICKABLERECTITEM_H
