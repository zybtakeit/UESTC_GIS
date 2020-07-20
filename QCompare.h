#pragma once
#pragma execution_character_set("utf-8")
#include <QMainWindow>
#include "ui_QCompare.h"
#include <qcheckbox.h>
#include <qslider.h>
#include <qspinbox.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qtreewidget.h>
#include <qdockwidget.h>
#include <qfiledialog.h>
#include <qdebug.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qmessagebox.h>
#include <qgsmapcanvas.h>
#include <qgsmaplayer.h>
#include <qgsrasterlayer.h>
//#include "readTiffInformation.h"
class QCompare : public QMainWindow
{
	Q_OBJECT

public:
	QCompare(QWidget *parent = Q_NULLPTR);
	~QCompare();

private:
	Ui::QCompare ui;
	QDockWidget* dockWidget;
	QTreeWidget* treeWidget;
	QPushButton* button;
	QGroupBox* groupBox;
	QLabel* label;
	QSlider* slider;
	QSpinBox* spinBox;
	QVBoxLayout* VLayout1;
	QVBoxLayout* VLayout2;
	QHBoxLayout* HLayout2_1;
	QVBoxLayout* VLayout2_1;
	QHBoxLayout* HLayout;
	QMenuBar* menuBar;

	QList<QgsMapLayer*> mapCanvasLayerSet; //���ڴ洢������ʾ��ͼ��
	QgsMapCanvas* mapCanvas;
	void iniUI();
	void iniSignalSlots();
	//treeWidget����
	enum treeItemUrl{itTopitem=1001,itGroupItem,itImageItem};
	enum treeColNum{colItem=0,colItemType=1};
	void iniTree();
	void addImageItem(QTreeWidgetItem* parItem, QString fileName);

	QAction* action_addListWidgetItem; //���б������ͼ����Ϣ��������ͼ��)

private slots:
	void addListWidgetItem(); //���б������ͼ����Ϣ��������ͼ��)
	void on_action_addImage_triggered();
	void on_action_compareImage_triggered();
	void on_action_removeImage_triggered();
};
