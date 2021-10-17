#pragma once

#include <QSpacerItem>
#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "ctrl/history_mgr.h"
#include "ui_start.h"


namespace urho3d
{
namespace editor
{
class RLabel : public QLabel
{
    Q_OBJECT
public:
    RLabel(QString str, QWidget* p = NULL)
        : QLabel(str, p)
    {
    }
    ~RLabel() {}
signals:
    void clicked();

public:
    void mouseReleaseEvent(QMouseEvent* ev) override;
};
class HistoryItem : public QWidget
{
    Q_OBJECT
public:
    HistoryItem(QWidget* parent, QString str);
    ~HistoryItem();
    void SetStart(QWidget* w, history_mgr* mgr);
public slots:
    void onItemClicked();
    void onDelete();

private:
    QWidget* _start;
    history_mgr* _historyMgr = nullptr;
    QHBoxLayout* _mainLayout = nullptr;
    //----------
    QWidget* _labelContainer = nullptr;
    QVBoxLayout* _labelLayout = nullptr;
    RLabel* _label = nullptr;
    QFrame* _line = nullptr;
    //-----------
    QSpacerItem* _hSpacer = nullptr;
    // QPushButton* _deleteButton = nullptr;
    RLabel* _deleteButton = nullptr;
};

class start_view:public QWidget
{
    Q_OBJECT
public:
    start_view();
    ~start_view();
public slots:
    void on_open_project();

protected:
    void init_history_list();

private:
    Ui::StartWindow _ui;
    int _content_width;
    history_mgr* _history_mgr = nullptr;
    QVBoxLayout* _main_layout = nullptr;
    QSpacerItem* _space = nullptr;
};
}
}