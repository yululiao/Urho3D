#include<QFileDialog>
#include <QMouseEvent>

#include "start_view.h"
#include "ctrl/utils.h"
#include "editor_app.h"

namespace urho3d
{
namespace editor
{
void RLabel::mouseReleaseEvent(QMouseEvent* ev)
{
    if (ev->button() == Qt::RightButton)
    {
        return;
    }
    emit clicked();
}


 HistoryItem::HistoryItem(QWidget* parent, QString str) 
 {
	 _start = NULL;
	 this->setMinimumHeight(32);
	 this->setMaximumHeight(32);

	 _mainLayout = new QHBoxLayout(this);
	 _mainLayout->setMargin(2);
     _mainLayout->setContentsMargins(10, 0, 50, 0);

	 _labelContainer = new QWidget(this);
	 _labelContainer->setMinimumHeight(25);
	 _labelLayout = new QVBoxLayout(_labelContainer);
	 _labelLayout->setMargin(2);
	 _labelLayout->setContentsMargins(0, 0, 0, 0);
	 _label = new RLabel(str, _labelContainer);
	 connect(_label, &RLabel::clicked, this, &HistoryItem::onItemClicked);
	 _label->setMinimumHeight(25);

	 _line = new QFrame(_labelContainer);
	 _line->setMinimumHeight(5);
	 _line->setFrameShape(QFrame::HLine);
	 _line->setFrameShadow(QFrame::Sunken);
	 _labelLayout->addWidget(_label);
	 _labelLayout->addWidget(_line);

	 _deleteButton = new RLabel("x", this);
	 _deleteButton->setMaximumWidth(25);
	 _deleteButton->setMinimumHeight(25);
	 connect(_deleteButton, &RLabel::clicked, this, &HistoryItem::onDelete);
	 _deleteButton->setToolTip(QStringLiteral("删除纪录"));
	 _deleteButton->setStyleSheet("QLabel{color:rgb(199,199,0)}");

	 _mainLayout->addWidget(_labelContainer);
	 _hSpacer = new QSpacerItem(30, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);
	 _mainLayout->addItem(_hSpacer);
	 _mainLayout->addWidget(_deleteButton);
 }

 HistoryItem::~HistoryItem() 
 {
 }

void HistoryItem::SetStart(QWidget* w, history_mgr* mgr) 
{
    this->_start = w;
    _historyMgr = mgr;
}

void HistoryItem::onItemClicked() 
{
    if (_start)
    {
        _start->close();
    }
	QString proj_name = _label->text();
	_historyMgr->delete_history(proj_name.toStdString().c_str());
	_historyMgr->add_project(proj_name.toStdString().c_str());
    EditorApp::get_instance()->open_work_space(Utils::qstr2str(_label->text()));
	_historyMgr->save();
}

void HistoryItem::onDelete()
{
	QString proj_name = _label->text();
    _historyMgr->delete_history(proj_name.toStdString().c_str());
	_historyMgr->save();
    this->close();
}

 start_view::start_view()
 {
     _history_mgr = new history_mgr();
     _ui.setupUi(this);
     _main_layout = new QVBoxLayout(_ui.history_container);
     _main_layout->setContentsMargins(0, 0, 0, 0);
     _main_layout->setMargin(2);
     _content_width = 0;
     auto fullTitle = QString::asprintf("%s[%d.%02d]", "MapEditor", 1, 0);
     setWindowTitle(fullTitle);

     init_history_list();
     connect(_ui.button_open, &QPushButton::clicked, this, &start_view::on_open_project);
 }

 start_view::~start_view() 
 {
     delete _history_mgr;
     _history_mgr = nullptr;
 }

void start_view::on_open_project() 
{
    QString select = QFileDialog::getExistingDirectory();
    if (select == "")
        return;
    EditorApp::get_instance()->open_work_space(Utils::qstr2str(select));

	_history_mgr->delete_history(select.toStdString().c_str());
	_history_mgr->add_project(select.toStdString().c_str());
	
	_history_mgr->save();
    this->close();
    
}

void start_view::init_history_list() 
{
    StringVector list = _history_mgr->get_history_list();
    for (int i = 0; i < list.Size(); i++)
    {
        HistoryItem* label = new HistoryItem(_ui.history_container, QString(list[i].CString()));
        label->SetStart(this, _history_mgr);
        _main_layout->addWidget(label);
    }
    _space = new QSpacerItem(10, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
    _main_layout->addItem(_space);
}

} // namespace editor
 }
