#ifndef CIMAGEDROPVIEW_H
#define CIMAGEDROPVIEW_H

#include <QGraphicsView>
#include <QDragEnterEvent>
#include <QDropEvent>

class CImageDropView : public QGraphicsView
{
    Q_OBJECT

signals:
    void imageDropped( const QImage& image, const QString& nameWithExt );

public:
    explicit CImageDropView(QWidget* parent = nullptr);

protected:
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dragMoveEvent(QDragMoveEvent* event) override;
    void dropEvent(QDropEvent* event) override;
};

#endif // CIMAGEDROPVIEW_H
