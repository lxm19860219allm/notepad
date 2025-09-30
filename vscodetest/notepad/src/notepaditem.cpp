#include "notepaditem.h"
#include <QJsonObject>

int NotepadItem::nextId = 1;

NotepadItem::NotepadItem(const QString& title, const QString& content)
    : id(nextId++), title(title), content(content)
{
}

QString NotepadItem::getTitle() const
{
    return title;
}

void NotepadItem::setTitle(const QString& title)
{
    this->title = title;
}

QString NotepadItem::getContent() const
{
    return content;
}

void NotepadItem::setContent(const QString& content)
{
    this->content = content;
}

int NotepadItem::getId() const
{
    return id;
}

QJsonObject NotepadItem::toJson() const
{
    QJsonObject json;
    json["id"] = id;
    json["title"] = title;
    json["content"] = content;
    return json;
}

void NotepadItem::fromJson(const QJsonObject& json)
{
    if (json.contains("id") && json["id"].isDouble()) {
        id = json["id"].toInt();
        // 更新nextId以确保唯一性
        if (id >= nextId) {
            nextId = id + 1;
        }
    }
    if (json.contains("title") && json["title"].isString()) {
        title = json["title"].toString();
    }
    if (json.contains("content") && json["content"].isString()) {
        content = json["content"].toString();
    }
}
