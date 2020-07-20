#pragma once
#pragma execution_character_set("utf-8")
#include <QMainWindow>
#include <QWidget>
#include "ui_RPCCorrection.h"

#include "qlayout.h"
#include "qbuttongroup.h"
#include "qradiobutton.h"
#include "qcombobox.h"
#include "qgroupbox.h"
#include "qcheckbox.h"
#include "qlabel.h"
#include "QPushButton"
#include "qlineedit.h"
#include "qfiledialog.h"
#include "qmessagebox.h"

class RPCCorrection : public QMainWindow
{
	Q_OBJECT

public:
	RPCCorrection(QWidget *parent = Q_NULLPTR);
	~RPCCorrection();

private:
	Ui::RPCCorrection ui;
	//布局
	QVBoxLayout* VLayout;
	QGroupBox* GroupBox_1;
	QGroupBox* GroupBox_2;
	QGroupBox* GroupBox_3;
	QGroupBox* GroupBox_4;
	QGroupBox* GroupBox_5;

    //GroupBox_1即输入图像内容
	QHBoxLayout* HLayout_Input;
	QPushButton* ButtonAddInputImage;
	QLineEdit* InputImageInfoLineEdit;

	//GroupBox_2即RPC选项内容
	QVBoxLayout* VLayout_RPC;
	QHBoxLayout* HLayout_RPC_1;
	QHBoxLayout* HLayout_RPC_2;
	QComboBox* ComboBoxInterpolation;
	QButtonGroup* ButtonGroupRPC;
	QRadioButton* ButtonRPC;
	QRadioButton* ButtonRPB;
	QRadioButton* ButtonFromImage;
	QPushButton* ButtonAddRPCInfo;
	QLineEdit* RPCInfoLineEdit;

	//GroupBox_3即DEM选项内容
	QVBoxLayout* VLayout_DEM;
	QHBoxLayout* HLayout_DEM_1;
	QHBoxLayout* HLayout_DEM_2;
	QButtonGroup* ButtonGroupDEM;
	QRadioButton* ButtonDEM;
	QRadioButton* ButtonNoDEM;
	QPushButton* ButtonAddDEMInfo;
	QLineEdit* DEMInfoLineEdit;

	//GroupBox_4即插值方式内容
	QHBoxLayout* HLayout_Interpolation;
	QLabel* InterpolationLabel;
	QComboBox* InterpolationComboBox;

	//GroupBox_5即输出路径
	QVBoxLayout* VLayout_Output;
	QHBoxLayout* HBoxLayout_Output_1;
	QHBoxLayout* HBoxLayout_Output_2;
	QPushButton* ButtonAddOutputPath;
	QLineEdit* OutputPathLineEdit;
	QLabel* OutputNameLabel;
	QLineEdit* OutputNameLineEdit;

	//确定界面
	QPushButton* ButtonRPCCorrection;
	//界面初始化
	void iniUI(); //初始化UI界面
	void iniSlots(); //初始化信号与槽
	void iniInterpolationComboBox(); //初始化插值列表
	//选择RPC和DEM模式
	int RPC_Mode;
	int DEM_Mode;
private slots:
	//void InterpolationChanged(); //
	void RPCChanged(int RPC_Mode); //
	void DEMChanged(int DEM_MODE); //

	void AddInputFilePath();
	void AddRPCFilePath();
	void AddDEMFilePath();
	void AddOutputFilePath();

	void RPCCorrect();
};
