#ifndef COURSEWORK_MAINWINDOW_H
#define COURSEWORK_MAINWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QListWidget>
#include <QFileDialog>
#include <QFileInfo>
#include <QVBoxLayout>
#include <QStyledItemDelegate>
#include <QPainter>
#include <QApplication>
#include <QList>

#include <memory>

#include "Client.h"
#include "NetworkClient.h"

namespace coursework::windows
{

    class MainWindow : public QWidget
    {
    Q_OBJECT

    private: //internal classes
        class CustomItem : public QListWidgetItem
        {
        public:
            CustomItem(const QString &filename, qint64 size, const QString &type)
                    : QListWidgetItem(filename)
            {
                setData(Qt::UserRole + 1, size);
                setData(Qt::UserRole + 2, type);
            }

            qint64 getSize() const
            {
                return data(Qt::UserRole + 1).toLongLong();
            }

            QString getType() const
            {
                return data(Qt::UserRole + 2).toString();
            }

        }; //Item for QListWidget
        class CustomItemDelegate : public QStyledItemDelegate
        {
        public:
            explicit CustomItemDelegate(QObject *parent = nullptr)
                    : QStyledItemDelegate(parent)
            {
            }

            void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override
            {
                painter->save();

                // Retrieve data from the custom item
                CustomItem* item = reinterpret_cast<CustomItem*>(index.internalPointer());
                if (!item)
                    return;

                QString filename = item->text();
                qint64 size = item->getSize();
                QString type = item->getType();

                // Draw the filename
                QStyleOptionViewItem opt = option;
                initStyleOption(&opt, index);
                opt.text = filename;
                opt.displayAlignment = Qt::AlignLeft | Qt::AlignVCenter;
                opt.rect.adjust(30, 0, 0, 0);
                QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &opt, painter);

                // Draw the size
                QRect sizeRect = opt.rect;
                sizeRect.adjust(150, 0, 0, 0);
                painter->drawText(sizeRect, Qt::AlignLeft | Qt::AlignVCenter, QString::number(size));

                // Draw the type
                QRect typeRect = opt.rect;
                typeRect.adjust(300, 0, 0, 0);
                painter->drawText(typeRect, Qt::AlignLeft | Qt::AlignVCenter, type);

                painter->restore();
            }
        }; //ItemDelegate for creation of the item in QListWidget

    public:
        MainWindow(std::shared_ptr<NetworkClient> client, QWidget *parent = nullptr);

    private slots:
        void downloadFileRequest();

        void uploadFileRequest();

        void updateFilesRequest();

        void deleteFileRequest();

    public slots:

        void updateFiles(const QString& files);

        void saveDownloadedFile(QByteArray fileData);

    private:
        void addFileToList(const QString &filePath);

    private:
        QList<QString> m_files;

        QListWidget * m_fileListWidget;
        QPushButton * m_downloadButton;
        QPushButton * m_uploadButton;
        QPushButton * m_updateButton;
        QPushButton * m_deleteButton;

        QString m_currentFile; //used to save the path to the file that we are going to download

        std::shared_ptr<NetworkClient> m_client;
    };
}

#endif //COURSEWORK_MAINWINDOW_H
