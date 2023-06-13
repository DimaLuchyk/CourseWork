#ifndef COURSEWORK_FILEMANAGER_H
#define COURSEWORK_FILEMANAGER_H

#include <QFile>
#include <QString>

namespace coursework::managers
{

    class FileManager
    {
    public:
        static bool createFile(const QString &fileName, const QString &filePath, const QByteArray &fileData);

        static bool deleteFile(const QString &fileName, const QString &filePath);
    };

}

#endif //COURSEWORK_FILEMANAGER_H
