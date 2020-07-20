#include "RPCCorrection.h"
#include "qdebug.h"
RPCCorrection::RPCCorrection(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	iniUI();
	iniSlots();
}

RPCCorrection::~RPCCorrection()
{
}

void RPCCorrection::iniUI() {
	QWidget* w = new QWidget();
	this->setCentralWidget(w);
	VLayout = new QVBoxLayout();
	GroupBox_1 = new QGroupBox(QObject::tr("ѡ������ͼ��"));
	GroupBox_2 = new QGroupBox(QObject::tr("���RPC����"));
	GroupBox_3 = new QGroupBox(QObject::tr("�Ƿ�ʹ�ø߳�����"));
	GroupBox_4 = new QGroupBox(QObject::tr("ѡ���ֵ����"));
	GroupBox_5 = new QGroupBox(QObject::tr("�趨���·��"));

	//ѡ������ͼ��
	HLayout_Input = new QHBoxLayout;
	ButtonAddInputImage = new QPushButton(QObject::tr("ѡ������ͼ��"));
	InputImageInfoLineEdit = new QLineEdit;
	InputImageInfoLineEdit->setDisabled(true);
	HLayout_Input->addWidget(ButtonAddInputImage, 1);
	HLayout_Input->addWidget(InputImageInfoLineEdit, 5);
	GroupBox_1->setLayout(HLayout_Input);

	//���RPC����
	VLayout_RPC = new QVBoxLayout;
	HLayout_RPC_1 = new QHBoxLayout;
	HLayout_RPC_2 = new QHBoxLayout;
	ButtonGroupRPC = new QButtonGroup;
	ButtonRPC = new QRadioButton(QObject::tr("ʹ��RPC�ļ�"));
	ButtonRPB = new QRadioButton(QObject::tr("ʹ��RPB�ļ�"));
	ButtonFromImage = new QRadioButton(QObject::tr("ʹ��ͼ����������"));
	ButtonGroupRPC->addButton(ButtonRPC, 0);
	ButtonGroupRPC->addButton(ButtonRPB, 1);
	ButtonGroupRPC->addButton(ButtonFromImage, 2);
	ButtonFromImage->setChecked(true);
	RPC_Mode = 2;
	HLayout_RPC_1->addWidget(ButtonRPC);
	HLayout_RPC_1->addWidget(ButtonRPB);
	HLayout_RPC_1->addWidget(ButtonFromImage);
	ButtonAddRPCInfo = new QPushButton(QObject::tr("���RPC�ļ�"));
	RPCInfoLineEdit = new QLineEdit;
	RPCInfoLineEdit->setDisabled(true);
	HLayout_RPC_2->addWidget(ButtonAddRPCInfo,1);
	HLayout_RPC_2->addWidget(RPCInfoLineEdit,5);
	VLayout_RPC->addLayout(HLayout_RPC_1);
	VLayout_RPC->addLayout(HLayout_RPC_2);
	GroupBox_2->setLayout(VLayout_RPC);

	//���DEM����
	VLayout_DEM = new QVBoxLayout;
	HLayout_DEM_1 = new QHBoxLayout;
	HLayout_DEM_2 = new QHBoxLayout;
	ButtonGroupDEM = new QButtonGroup;
	ButtonDEM = new QRadioButton(QObject::tr("ʹ��DEM�ļ�"));
	ButtonNoDEM = new QRadioButton(QObject::tr("��ʹ��DEM�ļ�"));
	ButtonGroupDEM->addButton(ButtonDEM, 0);
	ButtonGroupDEM->addButton(ButtonNoDEM, 1);
	ButtonNoDEM->setChecked(true);
	DEM_Mode = 1;
	HLayout_DEM_1->addWidget(ButtonDEM);
	HLayout_DEM_1->addWidget(ButtonNoDEM);
	ButtonAddDEMInfo = new QPushButton(QObject::tr("���DEM�ļ�"));
	DEMInfoLineEdit = new QLineEdit;
	DEMInfoLineEdit->setDisabled(true);
	HLayout_DEM_2->addWidget(ButtonAddDEMInfo, 1);
	HLayout_DEM_2->addWidget(DEMInfoLineEdit, 5);
	VLayout_DEM->addLayout(HLayout_DEM_1);
	VLayout_DEM->addLayout(HLayout_DEM_2);
	GroupBox_3->setLayout(VLayout_DEM);

	//GroupBox_4����ֵ��ʽ����
	HLayout_Interpolation = new QHBoxLayout;
	InterpolationLabel = new QLabel(QObject::tr("ѡ���ֵ����"));
	InterpolationComboBox = new QComboBox;
	iniInterpolationComboBox();
	HLayout_Interpolation->addWidget(InterpolationLabel, 1);
	HLayout_Interpolation->addWidget(InterpolationComboBox, 5);
	GroupBox_4->setLayout(HLayout_Interpolation);

	//GroupBox_5�����·��
	VLayout_Output = new QVBoxLayout;
	HBoxLayout_Output_1 = new QHBoxLayout;
	HBoxLayout_Output_2 = new QHBoxLayout;
	ButtonAddOutputPath = new QPushButton(QObject::tr("ѡ�����·��"));
	OutputPathLineEdit = new QLineEdit;
	OutputPathLineEdit->setDisabled(true);
	HBoxLayout_Output_1->addWidget(ButtonAddOutputPath, 1);
	HBoxLayout_Output_1->addWidget(OutputPathLineEdit, 5);
	OutputNameLabel = new QLabel(QObject::tr("���ͼ������"));
	OutputNameLineEdit = new QLineEdit;
	HBoxLayout_Output_2->addWidget(OutputNameLabel, 1);
	HBoxLayout_Output_2->addWidget(OutputNameLineEdit, 5);
	VLayout_Output->addLayout(HBoxLayout_Output_1);
	VLayout_Output->addLayout(HBoxLayout_Output_2);
	GroupBox_5->setLayout(VLayout_Output);

	VLayout->addWidget(GroupBox_1);
	VLayout->addWidget(GroupBox_2);
	VLayout->addWidget(GroupBox_3);
	VLayout->addWidget(GroupBox_4);
	VLayout->addWidget(GroupBox_5);

	ButtonRPCCorrection = new QPushButton(QObject::tr("��ʼRPCУ��"));
	ButtonRPCCorrection->setFixedWidth(200);
	VLayout->addWidget(ButtonRPCCorrection);
	w->setLayout(VLayout);
	w->setMinimumWidth(800);
}

void RPCCorrection::iniSlots() {
	connect(ButtonGroupRPC, SIGNAL(buttonClicked(int)), this, SLOT(RPCChanged(int)));
	connect(ButtonGroupDEM, SIGNAL(buttonClicked(int)), this, SLOT(DEMChanged(int)));
	connect(ButtonAddInputImage, SIGNAL(clicked()), this, SLOT(AddInputFilePath()));
	connect(ButtonAddRPCInfo, SIGNAL(clicked()), this, SLOT(AddRPCFilePath()));
	connect(ButtonAddDEMInfo, SIGNAL(clicked()), this, SLOT(AddDEMFilePath()));
	connect(ButtonAddOutputPath, SIGNAL(clicked()), this, SLOT(AddOutputFilePath()));
	connect(ButtonRPCCorrection, SIGNAL(clicked()), this, SLOT(RPCCorrect()));
}

void RPCCorrection::iniInterpolationComboBox() {
	InterpolationComboBox->clear();
	QMap<QString, int> City_Zone;
	City_Zone.insert("���Բ�ֵ", 10);
	City_Zone.insert("����ڲ�ֵ", 21);
	foreach(const QString & str, City_Zone.keys()) {
		InterpolationComboBox->addItem(str, City_Zone.value(str));
	}
}

void RPCCorrection::RPCCorrect() {
	qDebug() << "kaishi";
	if (RPC_Mode == 1 || RPC_Mode == 0) {
		if (RPCInfoLineEdit->text() == NULL) {
			QMessageBox::warning(NULL, "������ʾ", "������ṩRPC������ѡ��ʹ��ͼ����������", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
			return;
		}
	}
	if (DEM_Mode == 0 && DEMInfoLineEdit->text() == NULL) {
		QMessageBox::warning(NULL, "������ʾ", "������ṩDEM������ѡ��ʹ��DEM����", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		return;
	}
	const char* RPCInfoFilePath = RPCInfoLineEdit->text().toLatin1().data();
	const char* DEMInfoFilePath = DEMInfoLineEdit->text().toLatin1().data();
	const char* InputImageFilePath = InputImageInfoLineEdit->text().toLatin1().data();
	const char* OutputImageFilePath = OutputPathLineEdit->text().toLatin1().data();
	//GeoCorrection gc;
	//gc.RPCCorrect(InputImageFilePath, RPCInfoFilePath, DEMInfoFilePath, OutputImageFilePath,RPC_Mode,DEM_Mode);
}

void RPCCorrection::AddInputFilePath() {
	QString fileUrl = QFileDialog::getOpenFileName(this, "ѡ��Tif�ļ�", "", "Images(*.tif)");//��ѡ�ļ�
	if (fileUrl.isEmpty())
		return;
	InputImageInfoLineEdit->setText(fileUrl);
}

void RPCCorrection::AddRPCFilePath() {
	QString RPCFilePath;
	if (RPC_Mode == 0) {
		RPCFilePath = QFileDialog::getOpenFileName(this, "ѡ��RPC�ļ�", "", "rpc(*_rpc.txt)");//��ѡ�ļ�
	}
	else if (RPC_Mode == 1) {
		RPCFilePath = QFileDialog::getOpenFileName(this, "ѡ��RPB�ļ�", "", "rpb(*.rpb)");//��ѡ�ļ�
	}
	else {
		return;
	}
	if (RPCFilePath.isEmpty())
		return;
	RPCInfoLineEdit->setText(RPCFilePath);
}

void RPCCorrection::AddDEMFilePath() {
	if (DEM_Mode == 1) {
		return;
	}
	QString DEMFilePath = QFileDialog::getOpenFileName(this, "ѡ��DEM�ļ�", "", "dem(*.dem)");//��ѡ�ļ�
	if (DEMFilePath.isEmpty())
		return;
	DEMInfoLineEdit->setText(DEMFilePath);
}

void RPCCorrection::AddOutputFilePath() {
	QString OutputFilePath = QFileDialog::getExistingDirectory(this, "ѡ��·��","/");
	if (OutputFilePath.isEmpty())
		return;
	OutputPathLineEdit->setText(OutputFilePath);
}

void RPCCorrection::RPCChanged(int Rpc_Mode) {
	RPC_Mode = Rpc_Mode;
}

void RPCCorrection::DEMChanged(int Dem_MODE) {
	DEM_Mode = Dem_MODE;
}