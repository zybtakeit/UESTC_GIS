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
	//ui�������
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

	//�����ʼ��
	void iniUI(); //��ʼ��UI����
	void iniSlots(); //��ʼ���ź����

	//����
	void changeRasterLayer(QString url, QString basename); //�ı�QgsMapCanvas����ʾ��ͼ��

	//qgisģ��
	QgsMapCanvas* mapCanvas; //��ʾģ��
	QList<QgsMapLayer*> mapCanvasLayerSet; //���ڴ洢������ʾ��ͼ��
	const QgsMapToPixel* pixel; //����ת������
	QgsMapToolPan* toolPan; //�ƶ�����

	//QAction�������
	QAction* action_addListWidgetItem; //���б������ͼ����Ϣ��������ͼ��)
	QAction* action_deleteListWidgetItem; //ɾ���б���ͼ����Ϣ
	QAction* action_clearListWidget; //���ͼ���б�
	QAction* action_openImage; //���б������ͼ����ʾͼ��
	QAction* action_clearImage; //���ͼ��
	QAction* action_panMode;  //ʹ���ƶ�����
	QAction* action_coordTrans; //������ת������
	QAction* action_compareIamge; //��ͼ��ȽϹ���

private slots:
	void addListWidgetItem(); //���б������ͼ����Ϣ��������ͼ��)
	void deleteListWidgetItem(); //ɾ���б���ͼ����Ϣ
	void clearListWidget(); //���ͼ���б�
	void clearImage(); //���ͼ��
	void listWidgetItemDoubleClicked(QListWidgetItem* item); //˫��ͼ��ʱ������������ͼ��
	void mouseMove(const QgsPointXY& p); //����ƶ������¼�
	void panMode(bool checked); //ʹ���ƶ�����
	void openImage(); //���б������ͼ����ʾͼ��
	void transCoordination();
	void compareImages();
}; 
