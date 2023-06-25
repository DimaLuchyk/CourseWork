#include "MainWindow.h"

#include "Packet.h"
#include "PacketGenerator.h"
#include "Serializer.h"

coursework::windows::MainWindow::MainWindow(std::shared_ptr<NetworkClient> client, QWidget *parent)
    :
    QWidget(parent),
    m_client(client)
{
    m_fileListWidget = new QListWidget(this);
    m_downloadButton = new QPushButton("Download", this);
    m_uploadButton = new QPushButton("Upload", this);
    m_updateButton = new QPushButton("Update", this);
    m_deleteButton = new QPushButton("Delete", this);

    // Connect button signals to slots
    connect(m_downloadButton, &QPushButton::clicked, this, &MainWindow::downloadFileRequest);
    connect(m_uploadButton, &QPushButton::clicked, this, &MainWindow::uploadFileRequest);
    connect(m_updateButton, &QPushButton::clicked, this, &MainWindow::updateFilesRequest);
    connect(m_deleteButton, &QPushButton::clicked, this, &MainWindow::deleteFileRequest);

    // Set the layout
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_fileListWidget);
    layout->addWidget(m_downloadButton);
    layout->addWidget(m_uploadButton);
    layout->addWidget(m_updateButton);
    layout->addWidget(m_deleteButton);

    setLayout(layout);

    CustomItemDelegate* itemDelegate = new CustomItemDelegate(m_fileListWidget);
    m_fileListWidget->setItemDelegate(itemDelegate);

}

void coursework::windows::MainWindow::downloadFileRequest()
{
    // Retrieve the selected file from the list
    QListWidgetItem *selectedItem = m_fileListWidget->currentItem();
    if (selectedItem)
    {
        QString selectedFile = selectedItem->text();
        // Perform download operation for the selected file
        QString saveLocation = QFileDialog::getSaveFileName(nullptr, "Save File", selectedFile);
        if (saveLocation.isEmpty())
        {
            return;
        }

        qDebug() << "file location: " << saveLocation;

        m_currentFile = saveLocation;

        coursework::protocol::Payload payload{selectedFile};
        auto header = coursework::protocol::PacketGenerator::generatePacketHeader(protocol::PacketType::DOWNLOAD_FILE, sizeof(payload));

        m_client->sendData(coursework::protocol::Serializer::combineToPacket(header, payload));
    }
}

void coursework::windows::MainWindow::uploadFileRequest()
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
        payload.fileName = fileInfo.fileName();
        payload.fileData = file.readAll();

        auto header = coursework::protocol::PacketGenerator::generatePacketHeader(protocol::PacketType::ADD_FILE, sizeof(payload));
        m_client->sendData(coursework::protocol::Serializer::combineToPacket(header, payload));
    }
}

void coursework::windows::MainWindow::updateFilesRequest()
{
    protocol::Payload payload;
    auto header = protocol::PacketGenerator::generatePacketHeader(protocol::PacketType::GET_EXISTED_FILES, sizeof(payload));

    m_client->sendData(protocol::Serializer::combineToPacket(header, payload));
}

void coursework::windows::MainWindow::deleteFileRequest()
{
    // Retrieve the selected file from the list
    QListWidgetItem *selectedItem = m_fileListWidget->currentItem();
    if (selectedItem)
    {
        QString selectedFile = selectedItem->text();

        qDebug() << "file to delete: " << selectedFile;

        coursework::protocol::Payload payload{selectedFile};
        auto header = coursework::protocol::PacketGenerator::generatePacketHeader(protocol::PacketType::REMOVE_FILE, sizeof(payload));

        m_client->sendData(coursework::protocol::Serializer::combineToPacket(header, payload));
    }
}

void coursework::windows::MainWindow::updateFiles(const QString& files)
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

    m_fileListWidget->clear();

    for(const auto& file : fileNames)
    {
        addFileToList(file);
    }
}

void coursework::windows::MainWindow::saveDownloadedFile(QByteArray fileData)
{
    qDebug() << "saveDownloadedFile";

    QFile file(m_currentFile);
    if (file.open(QIODevice::WriteOnly))
    {
        file.write(fileData);
        file.close();

        qDebug() << "File was saved!";
    }
}

void coursework::windows::MainWindow::addFileToList(const QString &filePath)
{
    // Create a QListWidgetItem for the file
    QListWidgetItem *item = new QListWidgetItem(QFileInfo(filePath).fileName());
    m_fileListWidget->addItem(item);
}