#include "cimagedropview.h"
#include <QMimeData>
#include <QGraphicsPixmapItem>
#include <QFileInfo>
#include <QImage>
#include <QImageReader>

CImageDropView::CImageDropView(QWidget* parent) : QGraphicsView(parent)
{
    setAcceptDrops(true);
}

void
CImageDropView::dragEnterEvent(QDragEnterEvent* event)
{
    if (event->mimeData()->hasUrls()) event->acceptProposedAction();
}

void
CImageDropView::dragMoveEvent(QDragMoveEvent* event)
{
    event->acceptProposedAction();
}

void
CImageDropView::dropEvent(QDropEvent *event)
{
    const QMimeData *mimeData = event->mimeData();

    if (mimeData->hasUrls())
    {
        for (const QUrl &url : mimeData->urls()) {
            QString filePath = url.toLocalFile();
            QFileInfo fileInfo(filePath);

            // Check file extension (case-insensitive)
            QString ext = fileInfo.suffix().toLower();
            if (ext == "jpg" || ext == "jpeg" || ext == "png")
            {
                // Validate it's a real graphics file (checks headers/magic bytes)
                QImageReader reader(filePath);
                if (reader.canRead())
                {
                    QString nameWithExt = fileInfo.fileName();
                    // Load image from file and signal to MainWindow
                    QImage image = reader.read();
                    if (!image.isNull())
                    {
                        emit imageDropped(image, nameWithExt);
                    }
                }
                else
                {
                    qDebug() << "File has .jpg or .png extension but is not a valid image.";
                }
            }
        }
        event->acceptProposedAction();
    }
}
