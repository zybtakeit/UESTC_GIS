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
	GroupBox_1 = new QGroupBox(QObject::tr("选择输入图像"));
	GroupBox_2 = new QGroupBox(QObject::tr("添加RPC数据"));
	GroupBox_3 = new QGroupBox(QObject::tr("是否使用高程数据"));
	GroupBox_4 = new QGroupBox(QObject::tr("选择插值类型"));
	GroupBox_5 = new QGroupBox(QObject::tr("设定输出路径"));

	//选择输入图像
	HLayout_Input = new QHBoxLayout;
	ButtonAddInputImage = new QPushButton(QObject::tr("选择输入图像"));
	InputImageInfoLineEdit = new QLineEdit;
	InputImageInfoLineEdit->setDisabled(true);
	HLayout_Input->addWidget(ButtonAddInputImage, 1);
	HLayout_Input->addWidget(InputImageInfoLineEdit, 5);
	GroupBox_1->setLayout(HLayout_Input);

	//添加RPC数据
	VLayout_RPC = new QVBoxLayout;
	HLayout_RPC_1 = new QHBoxLayout;
	HLayout_RPC_2 = new QHBoxLayout;
	ButtonGroupRPC = new QButtonGroup;
	ButtonRPC = new QRadioButton(QObject::tr("使用RPC文件"));
	ButtonRPB = new QRadioButton(QObject::tr("使用RPB文件"));
	ButtonFromImage = new QRadioButton(QObject::tr("使用图像自身数据"));
	ButtonGroupRPC->addButton(ButtonRPC, 0);
	ButtonGroupRPC->addButton(ButtonRPB, 1);
	ButtonGroupRPC->addButton(ButtonFromImage, 2);
	ButtonFromImage->setChecked(true);
	RPC_Mode = 2;
	HLayout_RPC_1->addWidget(ButtonRPC);
	HLayout_RPC_1->addWidget(ButtonRPB);
	HLayout_RPC_1->addWidget(ButtonFromImage);
	ButtonAddRPCInfo = new QPushButton(QObject::tr("添加RPC文件"));
	RPCInfoLineEdit = new QLineEdit;
	RPCInfoLineEdit->setDisabled(true);
	HLayout_RPC_2->addWidget(ButtonAddRPCInfo,1);
	HLayout_RPC_2->addWidget(RPCInfoLineEdit,5);
	VLayout_RPC->addLayout(HLayout_RPC_1);
	VLayout_RPC->addLayout(HLayout_RPC_2);
	GroupBox_2->setLayout(VLayout_RPC);

	//添加DEM数据
	VLayout_DEM = new QVBoxLayout;
	HLayout_DEM_1 = new QHBoxLayout;
	HLayout_DEM_2 = new QHBoxLayout;
	ButtonGroupDEM = new QButtonGroup;
	ButtonDEM = new QRadioButton(QObject::tr("使用DEM文件"));
	ButtonNoDEM = new QRadioButton(QObject::tr("不使用DEM文件"));
	ButtonGroupDEM->addButton(ButtonDEM, 0);
	ButtonGroupDEM->addButton(ButtonNoDEM, 1);
	ButtonNoDEM->setChecked(true);
	DEM_Mode = 1;
	HLayout_DEM_1->addWidget(ButtonDEM);
	HLayout_DEM_1->addWidget(ButtonNoDEM);
	ButtonAddDEMInfo = new QPushButton(QObject::tr("添加DEM文件"));
	DEMInfoLineEdit = new QLineEdit;
	DEMInfoLineEdit->setDisabled(true);
	HLayout_DEM_2->addWidget(ButtonAddDEMInfo, 1);
	HLayout_DEM_2->addWidget(DEMInfoLineEdit, 5);
	VLayout_DEM->addLayout(HLayout_DEM_1);
	VLayout_DEM->addLayout(HLayout_DEM_2);
	GroupBox_3->setLayout(VLayout_DEM);

	//GroupBox_4即插值方式内容
	HLayout_Interpolation = new QHBoxLayout;
	InterpolationLabel = new QLabel(QObject::tr("选择插值类型"));
	InterpolationComboBox = new QComboBox;
	iniInterpolationComboBox();
	HLayout_Interpolation->addWidget(InterpolationLabel, 1);
	HLayout_Interpolation->addWidget(InterpolationComboBox, 5);
	GroupBox_4->setLayout(HLayout_Interpolation);

	//GroupBox_5即输出路径
	VLayout_Output = new QVBoxLayout;
	HBoxLayout_Output_1 = new QHBoxLayout;
	HBoxLayout_Output_2 = new QHBoxLayout;
	ButtonAddOutputPath = new QPushButton(QObject::tr("选择输出路径"));
	OutputPathLineEdit = new QLineEdit;
	OutputPathLineEdit->setDisabled(true);
	HBoxLayout_Output_1->addWidget(ButtonAddOutputPath, 1);
	HBoxLayout_Output_1->addWidget(OutputPathLineEdit, 5);
	OutputNameLabel = new QLabel(QObject::tr("输出图像名称"));
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

	ButtonRPCCorrection = new QPushButton(QObject::tr("开始RPC校正"));
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
	City_Zone.insert("线性插值", 10);
	City_Zone.insert("最近邻插值", 21);
	foreach(const QString & str, City_Zone.keys()) {
		InterpolationComboBox->addItem(str, City_Zone.value(str));
	}
}

void RPCCorrection::RPCCorrect() {
	qDebug() << "kaishi";
	if (RPC_Mode == 1 || RPC_Mode == 0) {
		if (RPCInfoLineEdit->text() == NULL) {
			QMessageBox::warning(NULL, "错误提示", "如果不提供RPC数据请选择使用图像自身数据", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
			return;
		}
	}
	if (DEM_Mode == 0 && DEMInfoLineEdit->text() == NULL) {
		QMessageBox::warning(NULL, "错误提示", "如果不提供DEM数据请选择不使用DEM数据", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
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
	QString fileUrl = QFileDialog::getOpenFileName(this, "选择Tif文件", "", "Images(*.tif)");//多选文件
	if (fileUrl.isEmpty())
		return;
	InputImageInfoLineEdit->setText(fileUrl);
}

void RPCCorrection::AddRPCFilePath() {
	QString RPCFilePath;
	if (RPC_Mode == 0) {
		RPCFilePath = QFileDialog::getOpenFileName(this, "选择RPC文件", "", "rpc(*_rpc.txt)");//多选文件
	}
	else if (RPC_Mode == 1) {
		RPCFilePath = QFileDialog::getOpenFileName(this, "选择RPB文件", "", "rpb(*.rpb)");//多选文件
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
	QString DEMFilePath = QFileDialog::getOpenFileName(this, "选择DEM文件", "", "dem(*.dem)");//多选文件
	if (DEMFilePath.isEmpty())
		return;
	DEMInfoLineEdit->setText(DEMFilePath);
}

void RPCCorrection::AddOutputFilePath() {
	QString OutputFilePath = QFileDialog::getExistingDirectory(this, "选择路径","/");
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