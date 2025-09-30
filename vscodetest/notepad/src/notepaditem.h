#ifndef NOTEPADITEM_H
#define NOTEPADITEM_H

#include <QString>
#include <QJsonObject>

class NotepadItem
{
public:
    NotepadItem(const QString& title = "新标签", const QString& content = "");

    QString getTitle() const;
    void setTitle(const QString& title);

    QString getContent() const;
    void setContent(const QString& content);

    int getId() const;

    // 序列化方法
    QJsonObject toJson() const;
    void fromJson(const QJsonObject& json);

private:
    static int nextId;
    int id;
    QString title;
    QString content;
};

#endif // NOTEPADITEM_H
