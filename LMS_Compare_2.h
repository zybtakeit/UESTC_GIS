#pragma once
#pragma execution_character_set("utf-8")
#include <QMainWindow>
#include "ui_LMS_Compare.h"
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
#include <qmenubar.h>
#include <qgraphicsview.h>
#include <qgraphicsscene.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <qfileinfo.h>
#include <qgraphicsitem.h>
#include <qapplication.h>
#include "LMS_GraphicView.h"
#include "LMS_Compare_Util.h"
#include "ui_LMS_Compare_2.h"

class LMS_Compare_2 : public QWidget
{
	Q_OBJECT

public:
	LMS_Compare_2(QWidget* parent = Q_NULLPTR);
	~LMS_Compare_2();

private:
	Ui::LMS_Compare_2 ui;
	QGroupBox* imageGroupBox;
	QGroupBox* moveGroupBox;
	QSlider* slider;
	QDoubleSpinBox* spinBox;
	QVBoxLayout* VLayout;
	QVBoxLayout* VLayout_1;
	QHBoxLayout* HLayout_1_1;
	QHBoxLayout* HLayout_1_2;
	QHBoxLayout* HLayout_2;
	QHBoxLayout* HLayout_3;

	QPushButton* button_ChooseImage1;
	QPushButton* button_ChooseImage2;
	QLineEdit* lineEdit_Image1;
	QLineEdit* lineEdit_Image2;
	QPushButton* button_ChooseTfw1;
	QPushButton* button_ChooseTfw2;
	QLineEdit* lineEdit_Tfw1;
	QLineEdit* lineEdit_Tfw2;

	QPushButton* button_StartToCompare;
	QGraphicsScene* scene;
	LMS_GraphicView* view;

	QGraphicsPixmapItem* backgroundImage;
	QGraphicsPixmapItem* coverImage;

	LMS_DataInformation* imageDataInformation_1;
	LMS_DataInformation* imageDataInformation_2;

	LMS_Compare_Util* util;

	double scale = 1.0;
	bool comparing;

	void iniUI();
	void iniSignalSlots();
	bool checkOverlapExist(LMS_DataInformation* imageDataInformation_1, LMS_DataInformation* imageDataInformation_2);
	bool checkGeographicInformationExist(LMS_DataInformation* dataInformation);
	bool readTfw(LMS_DataInformation* imageDataInformation_1, QString fileName);
	void paint(unsigned char* AllBand, int PictureWidth, int PictureHeight, int startX, int startY);
	void paint(LMS_DataInformation* dataInformation, int value);

private slots:
	void startToCompare();
	void chooseImage1();
	void chooseImage2();
	void chooseTfw1();
	void chooseTfw2();
	void test();
	void propotionChanged(int value);
	void scaleChanged(double scale);

	//void on_action_mouseMovePoint(QPoint point);
	//void on_action_mouseClicked(QPoint point);
	void on_action_mouseRelease();
	void on_action_mouseWheel();
};
