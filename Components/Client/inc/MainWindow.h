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
        MainWindow(std::shared_ptr<NetworkClient> client, QWidget *parent = nullptr) : QWidget(parent), m_client(client)
        {
            m_fileListWidget = new QListWidget(this);
            m_downloadButton = new QPushButton("Download", this);
            m_uploadButton = new QPushButton("Upload", this);
            m_updateButton = new QPushButton("Update", this);

            // Connect button signals to slots
            connect(m_downloadButton, &QPushButton::clicked, this, &MainWindow::downloadFileRequest);
            connect(m_uploadButton, &QPushButton::clicked, this, &MainWindow::uploadFileRequest);
            connect(m_updateButton, &QPushButton::clicked, this, &MainWindow::updateFilesRequest);

            // Set the layout
            QVBoxLayout *layout = new QVBoxLayout(this);
            layout->addWidget(m_fileListWidget);
            layout->addWidget(m_downloadButton);
            layout->addWidget(m_uploadButton);
            layout->addWidget(m_updateButton);

            setLayout(layout);

            CustomItemDelegate* itemDelegate = new CustomItemDelegate(m_fileListWidget);
            m_fileListWidget->setItemDelegate(itemDelegate);

        }

    private slots:
        void downloadFileRequest()
        {
            // Retrieve the selected file from the list
            QListWidgetItem *selectedItem = m_fileListWidget->currentItem();
            if (selectedItem)
            {
                QString selectedFile = selectedItem->text();
                // Perform download operation for the selected file
                qDebug() << "Downloading file: " << selectedFile;
            }
        }

        void uploadFileRequest()
        {
            // Open a file dialog for selecting files to upload
            QStringList selectedFiles = QFileDialog::getOpenFileNames(this, "Select Files to Upload");
            // Process the selected files
            for (const QString &filePath : selectedFiles)
            {
                QFile file(filePath);
                if(!file.open(QIODevice::ReadOnly))
                {
                    qDebug() << "Failed to open file. Filename: " << filePath;
                    continue;
                }

                qDebug() << "Uploading file: " << file;

                coursework::protocol::FilePaylaod payload;
                payload.clientUuid = m_client->getId().toString();
                QFileInfo fileInfo(filePath);
                qDebug() << "fileSize:" << fileInfo.size();
                payload.fileName = fileInfo.fileName();
                payload.fileData = file.readAll();

                qDebug() << "fileSize in payload: " << payload.fileData.size();
                qDebug() << "payload size: " << sizeof(payload);

                auto header = coursework::protocol::PacketGenerator::generatePacketHeader(protocol::PacketType::ADD_FILE, sizeof(payload));
                m_client->sendData(coursework::protocol::PacketGenerator::combineToPacket(header, payload));
            }
        }

        void updateFilesRequest()
        {
            protocol::Payload payload;
            auto header = protocol::PacketGenerator::generatePacketHeader(protocol::PacketType::GET_EXISTED_FILES, sizeof(payload));

            m_client->sendData(protocol::PacketGenerator::combineToPacket(header, payload));
        }

    public slots:

        void updateFiles(const QString& files)
        {
            qDebug() << "updateFiles" << files;
            QStringList fileNames;

            // Parse the packet to extract individual file names
            QString delimiter = "|"; // Delimiter used to separate file names
            int startIndex = 0;
            int delimiterIndex = files.indexOf(delimiter, startIndex);

            while (delimiterIndex != -1)
            {
                QString fileName = files.mid(startIndex, delimiterIndex - startIndex);
                fileNames.append(fileName);

                startIndex = delimiterIndex + delimiter.size();
                delimiterIndex = files.indexOf(delimiter, startIndex);
            }

            for(const auto& file : fileNames)
            {
                addFileToList(file);
            }
        }

    private:
        void addFileToList(const QString &filePath)
        {
            // Create a QListWidgetItem for the file
            QListWidgetItem *item = new QListWidgetItem(QFileInfo(filePath).fileName());
            m_fileListWidget->addItem(item);
        }

    private:
        QList<QString> m_files;

        QListWidget * m_fileListWidget;
        QPushButton * m_downloadButton;
        QPushButton * m_uploadButton;
        QPushButton * m_updateButton;

        std::shared_ptr<NetworkClient> m_client;
    };
}

#endif //COURSEWORK_MAINWINDOW_H
