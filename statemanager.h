#pragma once

#include <QObject>
#include <stack>
#include <list>


class QPlainTextEdit;
class QListWidgetItem;
class GameInterface;

class StateManager : public QObject
{
    Q_OBJECT
public:
    explicit StateManager(QObject *parent = nullptr);

    [[nodiscard]] bool save(GameInterface* iface, QListWidgetItem* item, QPlainTextEdit* header, quint32 version = 1) const;
    [[nodiscard]] std::optional<QString> load(GameInterface* iface, QListWidgetItem* item) const;
    [[nodiscard]] bool snapshot(GameInterface* iface, QListWidgetItem* item, QPlainTextEdit* header);
    [[nodiscard]] std::optional<QByteArray> undo(GameInterface *iface, QListWidgetItem *item, QPlainTextEdit* header);
    [[nodiscard]] std::optional<QByteArray> redo(GameInterface *iface, QListWidgetItem *item, QPlainTextEdit* header);

    void clearState();


private:
    std::list<QByteArray> undoList;
    std::stack<QByteArray> redoStack;

signals:
};

