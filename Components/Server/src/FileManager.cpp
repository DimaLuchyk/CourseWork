#include "../inc/FileManager.h"

bool coursework::managers::FileManager::createFile(const QString &fileName, const QString &filePath, const QByteArray &fileData)
{
    QFile file(filePath + "/" + fileName);
    if (file.open(QIODevice::WriteOnly))
    {
        file.write(fileData);
        file.close();
        return true;
    }
    return false;
}

bool coursework::managers::FileManager::deleteFile(const QString &fileName, const QString &filePath)
{
    QFile file(filePath + "/" + fileName);
    if (file.exists())
    {
        return file.remove();
    }
    return false;
}
