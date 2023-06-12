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




#include <memory>

#include "Client.h"
#include "NetworkClient.h"

namespace coursework::windows
{


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

    };

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
    };



    class MainWindow : public QWidget
    {
    Q_OBJECT

    public:
        MainWindow(std::shared_ptr<NetworkClient> client, QWidget *parent = nullptr) : QWidget(parent)
        {
            fileListWidget = new QListWidget(this);
            downloadButton = new QPushButton("Download", this);
            uploadButton = new QPushButton("Upload", this);

            // Connect button signals to slots
            connect(downloadButton, &QPushButton::clicked, this, &MainWindow::downloadFile);
            connect(uploadButton, &QPushButton::clicked, this, &MainWindow::uploadFile);

            // Set the layout
            QVBoxLayout *layout = new QVBoxLayout(this);
            layout->addWidget(fileListWidget);
            layout->addWidget(downloadButton);
            layout->addWidget(uploadButton);

            setLayout(layout);

            CustomItemDelegate* itemDelegate = new CustomItemDelegate(fileListWidget);
            fileListWidget->setItemDelegate(itemDelegate);

            // Add custom items to the list widget
            CustomItem* item1 = new CustomItem("File1.txt", 1024, "Text");
            fileListWidget->addItem(item1);

            CustomItem* item2 = new CustomItem("Image.png", 2048, "Image");
            fileListWidget->addItem(item2);



           // addFileToList("/home/dany/Documents/MyFiles/Programming/DataBaseTest/main.cpp");
        }

    private slots:
        void downloadFile()
        {
            // Retrieve the selected file from the list
            QListWidgetItem *selectedItem = fileListWidget->currentItem();
            if (selectedItem)
            {
                QString selectedFile = selectedItem->text();
                // Perform download operation for the selected file
                qDebug() << "Downloading file: " << selectedFile;
            }
        }

        void uploadFile()
        {
            // Open a file dialog for selecting files to upload
            QStringList selectedFiles = QFileDialog::getOpenFileNames(this, "Select Files to Upload");
            // Process the selected files
            for (const QString &file : selectedFiles)
            {
                // Perform upload operation for each file
                qDebug() << "Uploading file: " << file;
            }
        }

    private:
        void addFileToList(const QString &filePath)
        {

            // Create a QListWidgetItem for the file
            QListWidgetItem *item = new QListWidgetItem(QFileInfo(filePath).fileName());
            item->setToolTip(" bytes");

            fileListWidget->addItem(item);
        }

    private:
        QListWidget *fileListWidget;
        QPushButton *downloadButton;
        QPushButton *uploadButton;

        std::shared_ptr<NetworkClient> m_client;
    };
}

#endif //COURSEWORK_MAINWINDOW_H
