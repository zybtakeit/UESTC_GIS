#include "LMS_Compare.h"

LMS_Compare::LMS_Compare(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	iniUI();
	iniSignalSlots();
	bool comparing = false;
	imageDataInformation_1 = new LMS_DataInformation;
	imageDataInformation_2 = new LMS_DataInformation;
	area = new crossArea;
	util = new LMS_Compare_Util;
}

LMS_Compare::~LMS_Compare()
{
}

void LMS_Compare::iniUI() {
	QWidget* w = new QWidget();
	this->setCentralWidget(w);
	VLayout = new QVBoxLayout();
	VLayout_1 = new QVBoxLayout();
	HLayout_1_1 = new QHBoxLayout();
	HLayout_1_2 = new QHBoxLayout();
	HLayout_2 = new QHBoxLayout();

	imageGroupBox = new QGroupBox;
	moveGroupBox = new QGroupBox;

	button_ChooseImage1 = new QPushButton(tr("&选择第一幅图像"), this);
	button_ChooseImage2 = new QPushButton(tr("&选择第二幅图像"), this);
	lineEdit_Image1 = new QLineEdit;
	lineEdit_Image2 = new QLineEdit;
	button_ChooseTfw1 = new QPushButton(tr("&选择第一幅图像tfw"), this);
	button_ChooseTfw2 = new QPushButton(tr("&选择第二幅图像tfw"), this);
	lineEdit_Tfw1 = new QLineEdit;
	lineEdit_Tfw2 = new QLineEdit;
	HLayout_1_1->addWidget(button_ChooseImage1);
	HLayout_1_1->addWidget(lineEdit_Image1);
	HLayout_1_1->addWidget(button_ChooseTfw1);
	HLayout_1_1->addWidget(lineEdit_Tfw1);
	HLayout_1_2->addWidget(button_ChooseImage2);
	HLayout_1_2->addWidget(lineEdit_Image2);
	HLayout_1_2->addWidget(button_ChooseTfw2);
	HLayout_1_2->addWidget(lineEdit_Tfw2);
	VLayout_1->addLayout(HLayout_1_1);
	VLayout_1->addLayout(HLayout_1_2);
	imageGroupBox->setLayout(VLayout_1);

	slider = new QSlider;
	slider->setOrientation(Qt::Horizontal);
	slider->setMaximum(100);
	slider->setMinimum(1);
	slider->setSingleStep(1);
	spinBox = new QDoubleSpinBox;
	spinBox->setMaximum(50);
	spinBox->setMinimum(1);
	spinBox->setSingleStep(0.1);
	button_StartToCompare = new QPushButton(tr("&开始进行对比"), this);
	HLayout_2->addWidget(button_StartToCompare);
	HLayout_2->addWidget(slider);
	HLayout_2->addWidget(spinBox);
	moveGroupBox->setLayout(HLayout_2);

	scene = new QGraphicsScene();
	view = new QGraphicsView();
	view->setScene(scene);
	//view->setLayout(HLayout_2_2);

	VLayout->addWidget(imageGroupBox);
	VLayout->addWidget(moveGroupBox);
	VLayout->addWidget(view);
	w->setLayout(VLayout);
	w->setWindowFlag(Qt::Window);
	w->showMaximized();
	//w->setMinimumWidth(800);
	//w->setMinimumHeight(800);
	//w->setFixedWidth(QApplication::desktop()->width());
}

void LMS_Compare::iniSignalSlots() {
	//连接信号槽
	connect(spinBox, SIGNAL(valueChanged(double)), this, SLOT(scaleChanged(double)));
	connect(slider, SIGNAL(valueChanged(int)), this, SLOT(propotionChanged(int)));
	connect(button_ChooseImage1, SIGNAL(clicked()), this, SLOT(chooseImage1()));
	connect(button_ChooseImage2, SIGNAL(clicked()), this, SLOT(chooseImage2()));
	connect(button_ChooseTfw1, SIGNAL(clicked()), this, SLOT(chooseTfw1()));
	connect(button_ChooseTfw2, SIGNAL(clicked()), this, SLOT(chooseTfw2()));
	connect(button_StartToCompare, SIGNAL(clicked()), this, SLOT(startToCompare()));
	//connect(button_StartToCompare, SIGNAL(clicked()), this, SLOT(test()));
}

void LMS_Compare::test() {
	qDebug() << "test";
}


void LMS_Compare::startToCompare() {
	scene->clear();
	//scene->removeItem(backgroundImage);
	//scene->removeItem(coverImage);
	if (lineEdit_Image1->text() == NULL || lineEdit_Image2->text() == NULL) {
		QMessageBox::warning(NULL, "错误提示", "图像文件必须选择", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		return;
	}
	if (!QFile(lineEdit_Image1->text()).exists() || !QFile(lineEdit_Image1->text()).exists()) {
		QMessageBox::warning(NULL, "错误提示", "图像文件不存在", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		return;
	}
	imageDataInformation_1->imageFileName = lineEdit_Image1->text();//.toLatin1().data();
	imageDataInformation_2->imageFileName = lineEdit_Image2->text();//.toLatin1().data();
	if (lineEdit_Tfw1->text() != NULL) {
		imageDataInformation_1->tfwFileName = lineEdit_Tfw1->text();//.toLatin1().data();
	}
	if (lineEdit_Tfw2->text() != NULL) {
		imageDataInformation_2->tfwFileName = lineEdit_Tfw2->text();//.toLatin1().data();
		qDebug() << "??";
	}
	if (!checkGeographicInformationExist(imageDataInformation_1)|| !checkGeographicInformationExist(imageDataInformation_2)) {
		QMessageBox::warning(NULL, "错误提示", "无法找到图像地理信息", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		return;
	}
	if (!checkOverlapExist(imageDataInformation_1, imageDataInformation_2)) {
		QMessageBox::warning(NULL, "错误提示", "两幅图像没有重叠区域", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		//return;
	}
	QRect range_1 = QRect(0, 0, imageDataInformation_1->imageWidth - 1, imageDataInformation_1->imageHeight - 1);
	scale = imageDataInformation_1->imageWidth * 0.001;
	spinBox->setValue(scale);
	unsigned char* image_1 = util->readTiffImage(imageDataInformation_1->imageFileName.toLatin1().data(), range_1, 1000, int(imageDataInformation_1->imageHeight/scale));
	paint(image_1, 1000, int(imageDataInformation_1->imageHeight / scale), 0, 0);

	QRect range_2 = QRect(0, 0, imageDataInformation_2->imageWidth - 1, imageDataInformation_2->imageHeight - 1);
	int width = (imageDataInformation_2->imageWidth * imageDataInformation_2->deltaX) / (imageDataInformation_1->deltaX * scale);
	int height = (imageDataInformation_2->imageHeight * imageDataInformation_2->deltaY) / (imageDataInformation_1->deltaY * scale);
	unsigned char* image_2 = util->readTiffImage(imageDataInformation_2->imageFileName.toLatin1().data(), range_2, width, height);
	int startX = (imageDataInformation_2->_UpperLeftPointX - imageDataInformation_1->_UpperLeftPointX) / (imageDataInformation_1->deltaX * scale);
	int startY = (imageDataInformation_2->_UpperLeftPointY - imageDataInformation_1->_UpperLeftPointY) / (imageDataInformation_1->deltaY * scale);
	paint(image_2, width, height, startX, startY);

	comparing = true;
}

void LMS_Compare::scaleChanged(double scaleValue) {
	scale = scaleValue;
	if (scale < 1 || scale>50) {
		return;
	}
	if (!comparing) {
		return;
	}
	scene->clear();
	int value = slider->value();
	QRect range_1 = QRect(0, 0, imageDataInformation_1->imageWidth - 1, imageDataInformation_1->imageHeight - 1);
	int width = imageDataInformation_1->imageWidth / scale;
	int height = imageDataInformation_1->imageHeight / scale;
	unsigned char* image_1 = util->readTiffImage(imageDataInformation_1->imageFileName.toLatin1().data(), range_1, width, height);
	paint(image_1, width, height, 0, 0);

	QRect range_2 = QRect(int(imageDataInformation_2->imageWidth * value * 0.01), 0, int(imageDataInformation_2->imageWidth - 1 - imageDataInformation_2->imageWidth * value * 0.01), imageDataInformation_2->imageHeight - 1);
	width = (imageDataInformation_2->imageWidth * imageDataInformation_2->deltaX * (1 - value * 0.01)) / (imageDataInformation_1->deltaX * scale);
	height = (imageDataInformation_2->imageHeight * imageDataInformation_2->deltaY) / (imageDataInformation_1->deltaY * scale);
	unsigned char* image_2 = util->readTiffImage(imageDataInformation_2->imageFileName.toLatin1().data(), range_2, width, height);
	int startX = (imageDataInformation_2->imageWidth * value * 0.01 * imageDataInformation_2->deltaX + imageDataInformation_2->_UpperLeftPointX - imageDataInformation_1->_UpperLeftPointX) / (imageDataInformation_1->deltaX * scale);
	int startY = (imageDataInformation_2->_UpperLeftPointY - imageDataInformation_1->_UpperLeftPointY) / (imageDataInformation_1->deltaY * scale);
	paint(image_2, width, height, startX, startY);
}

void LMS_Compare::propotionChanged(int value) {
	if (!comparing) {
		return;
	}
	scene->clear();

	QRect range_1 = QRect(0, 0, imageDataInformation_1->imageWidth - 1, imageDataInformation_1->imageHeight - 1);
	int width = imageDataInformation_1->imageWidth / scale;
	int height = imageDataInformation_1->imageHeight / scale;
	unsigned char* image_1 = util->readTiffImage(imageDataInformation_1->imageFileName.toLatin1().data(), range_1, width, height);
	paint(image_1, width, height, 0, 0);

	QRect range_2 = QRect(int(imageDataInformation_2->imageWidth * value * 0.01), 0, int(imageDataInformation_2->imageWidth - 1 - imageDataInformation_2->imageWidth * value * 0.01), imageDataInformation_2->imageHeight - 1);
	width = (imageDataInformation_2->imageWidth * imageDataInformation_2->deltaX * (1- value * 0.01)) / (imageDataInformation_1->deltaX * scale);
	height = (imageDataInformation_2->imageHeight * imageDataInformation_2->deltaY) / (imageDataInformation_1->deltaY * scale);
	unsigned char* image_2 = util->readTiffImage(imageDataInformation_2->imageFileName.toLatin1().data(), range_2, width, height);
	int startX = (imageDataInformation_2->imageWidth * value * 0.01 * imageDataInformation_2->deltaX + imageDataInformation_2->_UpperLeftPointX - imageDataInformation_1->_UpperLeftPointX) / (imageDataInformation_1->deltaX * scale);
	int startY = (imageDataInformation_2->_UpperLeftPointY - imageDataInformation_1->_UpperLeftPointY) / (imageDataInformation_1->deltaY * scale);
	paint(image_2, width, height, startX, startY);
}

void LMS_Compare::paint(unsigned char* AllBand,int PictureWidth,int PictureHeight,int startX,int startY) {
	QImage TifImage = QImage(AllBand, PictureWidth, PictureHeight, PictureWidth * 3, QImage::Format_RGB888); //将图像信息以RGB888的形式读入QImage中
	QPixmap ConvertPixmap = QPixmap::fromImage(TifImage);
	QGraphicsPixmapItem* ImageItem = new QGraphicsPixmapItem(ConvertPixmap);
	ImageItem->setPos(startX, startY);
	scene->addItem(ImageItem);
	delete[] AllBand; //释放数组
}

bool LMS_Compare::checkOverlapExist(LMS_DataInformation* imageDataInformation_1, LMS_DataInformation* imageDataInformation_2) {
	int _UpperRightX_1 = imageDataInformation_1->_UpperLeftPointX + imageDataInformation_1->imageWidth * imageDataInformation_1->deltaX;
	int _LowerLeftY_1 = imageDataInformation_1->_UpperLeftPointY + imageDataInformation_1->imageHeight * imageDataInformation_1->deltaY;
	int _UpperRightX_2 = imageDataInformation_2->_UpperLeftPointX + imageDataInformation_2->imageWidth * imageDataInformation_2->deltaX;
	int _LowerLeftY_2 = imageDataInformation_2->_UpperLeftPointY + imageDataInformation_2->imageHeight * imageDataInformation_2->deltaY;
	return (((imageDataInformation_1->_UpperLeftPointX > imageDataInformation_2->_UpperLeftPointX && imageDataInformation_1->_UpperLeftPointX < _UpperRightX_2)
		|| (imageDataInformation_1->_UpperLeftPointX < imageDataInformation_2->_UpperLeftPointX && imageDataInformation_2->_UpperLeftPointX < _UpperRightX_1))
		&& ((imageDataInformation_1->_UpperLeftPointY > imageDataInformation_2->_UpperLeftPointY && imageDataInformation_1->_UpperLeftPointY < _LowerLeftY_2)
		|| (imageDataInformation_1->_UpperLeftPointY < imageDataInformation_2->_UpperLeftPointY && imageDataInformation_2->_UpperLeftPointY < _LowerLeftY_1))) ? true : false;
}

bool LMS_Compare::checkGeographicInformationExist(LMS_DataInformation* imageDataInformation) {
	if (true) {
		//寻找同名tfw文件
	}
	if (imageDataInformation->tfwFileName != NULL) {
		if (readTfw(imageDataInformation, imageDataInformation->tfwFileName)){
			if (util->readTiffInformation(imageDataInformation->imageFileName.toLatin1().data(), imageDataInformation, false)) {
				return true;
			}
			else {
				return false;
			}
		}
		else {
			if (util->readTiffInformation(imageDataInformation->imageFileName.toLatin1().data(), imageDataInformation, true)) {
				QMessageBox::warning(NULL, "警告", "tfw文件可能出错，已从图像读取地理数据", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
				return true;
			}
			else {
				return false;
			}
		}
	}else {
		if (util->readTiffInformation(imageDataInformation->imageFileName.toLatin1().data(), imageDataInformation, true)) {
			return true;
		}
		else {
			return false;
		}
	}
}

bool LMS_Compare::readTfw(LMS_DataInformation* imageData,QString fileName) {
	if (!QFile(fileName).exists()) {
		QMessageBox::warning(NULL, "错误提示", "tfw文件不存在", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		return false;
	}
	QFile file(fileName);
	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QTextStream aStream(&file);
		imageData->deltaX = aStream.readLine().trimmed().toDouble();
		imageData->rotationX = aStream.readLine().trimmed().toDouble();
		imageData->rotationY = aStream.readLine().trimmed().toDouble();
		imageData->deltaY = aStream.readLine().trimmed().toDouble();
		imageData->_UpperLeftPointX = aStream.readLine().trimmed().toDouble();
		imageData->_UpperLeftPointY = aStream.readLine().trimmed().toDouble();
	}
	file.close();
	return true;
}

void LMS_Compare::chooseImage1() {
	QString fileUrl = QFileDialog::getOpenFileName(this, "选择Tif文件", "", "Images(*.tif)");//多选文件
	if (fileUrl.isEmpty())
		return;
	lineEdit_Image1->setText(fileUrl);
}

void LMS_Compare::chooseImage2() {
	QString fileUrl = QFileDialog::getOpenFileName(this, "选择Tif文件", "", "Images(*.tif)");//多选文件
	if (fileUrl.isEmpty())
		return;
	lineEdit_Image2->setText(fileUrl);
}

void LMS_Compare::chooseTfw1() {
	QString fileUrl = QFileDialog::getOpenFileName(this, "选择Tif文件", "", "Images(*.tfw)");//多选文件
	if (fileUrl.isEmpty())
		return;
	lineEdit_Tfw1->setText(fileUrl);
}

void LMS_Compare::chooseTfw2() {
	QString fileUrl = QFileDialog::getOpenFileName(this, "选择Tif文件", "", "Images(*.tfw)");//多选文件
	if (fileUrl.isEmpty())
		return;
	lineEdit_Tfw2->setText(fileUrl);
}
