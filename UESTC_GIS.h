#pragma once
#pragma execution_character_set("utf-8")
#include <QtWidgets/QWidget>
#include "ui_UESTC_GIS.h"
#include "SatCoordTrans.h"
#include "QCompare.h"

#include <math.h>
#include <qgsmapcanvas.h>
#include <qgsmaplayer.h>
#include <qgsrasterlayer.h>
#include <QList>
#include <qgsproject.h>
#include <qgraphicsscene.h>
#include <qgsmaptoolpan.h>
#include <qboxlayout.h>

#include <qboxlayout.h>
#include <qspinbox.h>
#include <qslider.h>
#include <qmessagebox.h>
#include <qtabwidget.h>
#include <qlistwidget.h>
#include <qstatusbar.h>
#include <qlineedit.h>
#include <qtoolbar.h>
#include <qmenubar.h>
#include <qfiledialog.h>
class UESTC_GIS : public QWidget
{
	Q_OBJECT

public:
	UESTC_GIS(QWidget *parent = Q_NULLPTR);

private:
	Ui::UESTC_GISClass ui;
	//ui界面设计
	QVBoxLayout* VLayout;
	QHBoxLayout* HLayout;
	QHBoxLayout* HLayout_Page1;
	QHBoxLayout* HLayout_Page2;
	QHBoxLayout* HLayout_Page3;
	QTabWidget* tabWidget;
	QListWidget* listWidget;
	QStatusBar* statusBar;
	QToolBar* toolBar_1;
	QToolBar* toolBar_2;
	QToolBar* toolBar_3;
	QLineEdit* lineEdit_1;
	QLineEdit* lineEdit_2;
	QSpinBox* spinBox;
	QSlider* slider;
	QLineEdit* lineEdit;
	QMenuBar* menuBar;

	//界面初始化
	void iniUI(); //初始化UI界面
	void iniSlots(); //初始化信号与槽

	//功能
	void changeRasterLayer(QString url, QString basename); //改变QgsMapCanvas中显示的图像

	//qgis模块
	QgsMapCanvas* mapCanvas; //显示模块
	QList<QgsMapLayer*> mapCanvasLayerSet; //用于存储用来显示的图层
	const QgsMapToPixel* pixel; //用于转换坐标
	QgsMapToolPan* toolPan; //移动工具

	//QAction动作设计
	QAction* action_addListWidgetItem; //向列表中添加图像信息（不加载图像)
	QAction* action_deleteListWidgetItem; //删除列表中图像信息
	QAction* action_clearListWidget; //清空图像列表
	QAction* action_openImage; //向列表中添加图像并显示图像
	QAction* action_clearImage; //清空图像
	QAction* action_panMode;  //使用移动工具
	QAction* action_coordTrans; //打开坐标转换工具
	QAction* action_compareIamge; //打开图像比较工具

private slots:
	void addListWidgetItem(); //向列表中添加图像信息（不加载图像)
	void deleteListWidgetItem(); //删除列表中图像信息
	void clearListWidget(); //清空图像列表
	void clearImage(); //清空图像
	void listWidgetItemDoubleClicked(QListWidgetItem* item); //双击图像时触发，即家在图像
	void mouseMove(const QgsPointXY& p); //鼠标移动触发事件
	void panMode(bool checked); //使用移动工具
	void openImage(); //向列表中添加图像并显示图像
	void transCoordination();
	void compareImages();
}; 
