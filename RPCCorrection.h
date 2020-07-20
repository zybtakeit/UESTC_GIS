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
	//����
	QVBoxLayout* VLayout;
	QGroupBox* GroupBox_1;
	QGroupBox* GroupBox_2;
	QGroupBox* GroupBox_3;
	QGroupBox* GroupBox_4;
	QGroupBox* GroupBox_5;

    //GroupBox_1������ͼ������
	QHBoxLayout* HLayout_Input;
	QPushButton* ButtonAddInputImage;
	QLineEdit* InputImageInfoLineEdit;

	//GroupBox_2��RPCѡ������
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

	//GroupBox_3��DEMѡ������
	QVBoxLayout* VLayout_DEM;
	QHBoxLayout* HLayout_DEM_1;
	QHBoxLayout* HLayout_DEM_2;
	QButtonGroup* ButtonGroupDEM;
	QRadioButton* ButtonDEM;
	QRadioButton* ButtonNoDEM;
	QPushButton* ButtonAddDEMInfo;
	QLineEdit* DEMInfoLineEdit;

	//GroupBox_4����ֵ��ʽ����
	QHBoxLayout* HLayout_Interpolation;
	QLabel* InterpolationLabel;
	QComboBox* InterpolationComboBox;

	//GroupBox_5�����·��
	QVBoxLayout* VLayout_Output;
	QHBoxLayout* HBoxLayout_Output_1;
	QHBoxLayout* HBoxLayout_Output_2;
	QPushButton* ButtonAddOutputPath;
	QLineEdit* OutputPathLineEdit;
	QLabel* OutputNameLabel;
	QLineEdit* OutputNameLineEdit;

	//ȷ������
	QPushButton* ButtonRPCCorrection;
	//�����ʼ��
	void iniUI(); //��ʼ��UI����
	void iniSlots(); //��ʼ���ź����
	void iniInterpolationComboBox(); //��ʼ����ֵ�б�
	//ѡ��RPC��DEMģʽ
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
