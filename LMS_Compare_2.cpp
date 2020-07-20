#include "LMS_Compare_2.h"

LMS_Compare_2::LMS_Compare_2(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	iniUI();
	iniSignalSlots();
	bool comparing = false;
	imageDataInformation_1 = new LMS_DataInformation;
	imageDataInformation_2 = new LMS_DataInformation;
	util = new LMS_Compare_Util;
}

LMS_Compare_2::~LMS_Compare_2()
{
}

void LMS_Compare_2::iniUI() {
	//QWidget* w = new QWidget();
	//this->setCentralWidget(w);
	VLayout = new QVBoxLayout();
	VLayout_1 = new QVBoxLayout();
	HLayout_1_1 = new QHBoxLayout();
	HLayout_1_2 = new QHBoxLayout();
	HLayout_2 = new QHBoxLayout();
	HLayout_3 = new QHBoxLayout();

	imageGroupBox = new QGroupBox;
	moveGroupBox = new QGroupBox;

	button_ChooseImage1 = new QPushButton(tr("&ѡ���һ��ͼ��"), this);
	button_ChooseImage2 = new QPushButton(tr("&ѡ��ڶ���ͼ��"), this);
	lineEdit_Image1 = new QLineEdit;
	lineEdit_Image2 = new QLineEdit;
	button_ChooseTfw1 = new QPushButton(tr("&ѡ���һ��ͼ��tfw"), this);
	button_ChooseTfw2 = new QPushButton(tr("&ѡ��ڶ���ͼ��tfw"), this);
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
	slider->setMinimum(0);
	slider->setSingleStep(1);
	spinBox = new QDoubleSpinBox;
	spinBox->setMaximum(50);
	spinBox->setMinimum(1);
	spinBox->setSingleStep(0.1);
	button_StartToCompare = new QPushButton(tr("&��ʼ���жԱ�"), this);
	HLayout_2->addWidget(button_StartToCompare);
	HLayout_2->addWidget(slider);
	HLayout_2->addWidget(spinBox);
	moveGroupBox->setLayout(HLayout_2);

	scene = new QGraphicsScene();
	view = new LMS_GraphicView();
	view->setLayout(HLayout_3);
	view->setScene(scene);
	view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	VLayout->addWidget(imageGroupBox);
	VLayout->addWidget(moveGroupBox);
	VLayout->addWidget(view);
	this->setWindowFlag(Qt::Window);
	this->showMaximized();
	this->setLayout(VLayout);
}

void LMS_Compare_2::iniSignalSlots() {
	//�����źŲ�
	connect(spinBox, SIGNAL(valueChanged(double)), this, SLOT(scaleChanged(double)));
	connect(slider, SIGNAL(valueChanged(int)), this, SLOT(propotionChanged(int)));
	connect(button_ChooseImage1, SIGNAL(clicked()), this, SLOT(chooseImage1()));
	connect(button_ChooseImage2, SIGNAL(clicked()), this, SLOT(chooseImage2()));
	connect(button_ChooseTfw1, SIGNAL(clicked()), this, SLOT(chooseTfw1()));
	connect(button_ChooseTfw2, SIGNAL(clicked()), this, SLOT(chooseTfw2()));
	connect(button_StartToCompare, SIGNAL(clicked()), this, SLOT(startToCompare()));
	//connect(button_StartToCompare, SIGNAL(clicked()), this, SLOT(test()));
	connect(view, SIGNAL(mouseMovePoint(QPoint)), this, SLOT(on_action_mouseMovePoint(QPoint)));
	connect(view, SIGNAL(mouseClicked(QPoint)), this, SLOT(on_action_mouseClicked(QPoint)));
	connect(view, SIGNAL(mouseRelease()), this, SLOT(on_action_mouseRelease()));
	connect(view, SIGNAL(mouseWheel()), this, SLOT(on_action_mouseWheel()));
}

void LMS_Compare_2::test() {
	qDebug() << "test";
}


void LMS_Compare_2::startToCompare() {
	scene->clear();
	comparing = false;
	if (lineEdit_Image1->text() == NULL || lineEdit_Image2->text() == NULL) {
		QMessageBox::warning(NULL, "������ʾ", "ͼ���ļ�����ѡ��", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		return;
	}
	if (!QFile(lineEdit_Image1->text()).exists() || !QFile(lineEdit_Image1->text()).exists()) {
		QMessageBox::warning(NULL, "������ʾ", "ͼ���ļ�������", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
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
	if (!checkGeographicInformationExist(imageDataInformation_1) || !checkGeographicInformationExist(imageDataInformation_2)) {
		QMessageBox::warning(NULL, "������ʾ", "�޷��ҵ�ͼ�������Ϣ", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		return;
	}
	if (!checkOverlapExist(imageDataInformation_1, imageDataInformation_2)) {
		QMessageBox::warning(NULL, "������ʾ", "����ͼ��û���ص�����", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		//return;
	}
	view->setSceneRect(0, 0, this->width(), this->height());
	imageDataInformation_1->sceneStartX = 0;
	imageDataInformation_1->sceneStartY = 0;
	imageDataInformation_1->sceneWidth = imageDataInformation_1->imageWidth;
	imageDataInformation_1->sceneHeight = imageDataInformation_1->imageWidth;
	imageDataInformation_2->sceneStartX = (imageDataInformation_2->_UpperLeftPointX - imageDataInformation_1->_UpperLeftPointX) / (imageDataInformation_1->deltaX * scale);
	imageDataInformation_2->sceneStartY = (imageDataInformation_2->_UpperLeftPointY - imageDataInformation_1->_UpperLeftPointY) / (imageDataInformation_1->deltaY * scale);
	imageDataInformation_2->sceneWidth = (imageDataInformation_2->imageWidth * imageDataInformation_2->deltaX) / (imageDataInformation_1->deltaX );
	imageDataInformation_2->sceneHeight = (imageDataInformation_2->imageHeight * imageDataInformation_2->deltaY) / (imageDataInformation_1->deltaY);

	paint(imageDataInformation_1, 0);
	paint(imageDataInformation_2, slider->value());

	comparing = true;
}

void LMS_Compare_2::scaleChanged(double scaleValue) {
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

void LMS_Compare_2::propotionChanged(int value) {
	if (!comparing) {
		return;
	}
	scene->clear();
	paint(imageDataInformation_1, 0);
	paint(imageDataInformation_2, value);
	view->setScene(scene);
	view->updateTempScene();
}

void LMS_Compare_2::paint(unsigned char* AllBand, int PictureWidth, int PictureHeight, int startX, int startY) {
	QImage TifImage = QImage(AllBand, PictureWidth, PictureHeight, PictureWidth * 3, QImage::Format_RGB888); //��ͼ����Ϣ��RGB888����ʽ����QImage��
	QPixmap ConvertPixmap = QPixmap::fromImage(TifImage);
	QGraphicsPixmapItem* ImageItem = new QGraphicsPixmapItem(ConvertPixmap);
	ImageItem->setPos(startX, startY);
	scene->addItem(ImageItem);
	delete[] AllBand; //�ͷ�����
}

void LMS_Compare_2::paint(LMS_DataInformation* dataInformation,int value) {
	QRectF sceneRect = QRectF(view->sceneRect().x(), view->sceneRect().y(), view->sceneRect().width(), view->sceneRect().height());
	readArea* area = new readArea;
	util->getImageArea(sceneRect, dataInformation, area, view->getScale(), value);
	if (area->exist) {
		int imageUpperLeftX = (imageDataInformation_1->_UpperLeftPointX + imageDataInformation_1->deltaX * area->_UpperLeftX - dataInformation->_UpperLeftPointX) / dataInformation->deltaX;
		int imageUpperLeftY = (imageDataInformation_1->_UpperLeftPointY + imageDataInformation_1->deltaY * area->_UpperLeftY - dataInformation->_UpperLeftPointY) / dataInformation->deltaY;
		int imageBottomRightX = (imageDataInformation_1->_UpperLeftPointX + imageDataInformation_1->deltaX * area->_BottomRightX - dataInformation->_UpperLeftPointX) / dataInformation->deltaX;
		int imageBottomRightY = (imageDataInformation_1->_UpperLeftPointY + imageDataInformation_1->deltaY * area->_BottomRightY - dataInformation->_UpperLeftPointY) / dataInformation->deltaY;
		imageUpperLeftX = imageUpperLeftX < 0 ? 0: imageUpperLeftX;
		imageUpperLeftY = imageUpperLeftY < 0 ? 0: imageUpperLeftY;
		imageBottomRightX = imageBottomRightX < dataInformation->imageWidth ? imageBottomRightX : dataInformation->imageWidth;
		imageBottomRightY = imageBottomRightY < dataInformation->imageHeight ? imageBottomRightY : dataInformation->imageHeight;
		int imageWidth = imageBottomRightX - imageUpperLeftX - 1;
		int imageHeight = imageBottomRightY - imageUpperLeftY - 1;

		int sceneStartX = area->_UpperLeftX;
		int sceneStartY = area->_UpperLeftY;
		int sceneWidth = area->_BottomRightX - area->_UpperLeftX;
		int sceneHeight = area->_BottomRightY - area->_UpperLeftY;
		QRect range = QRect(imageUpperLeftX, imageUpperLeftY, imageWidth, imageHeight);
		unsigned char* image = util->readTiffImage(dataInformation->imageFileName.toLatin1().data(), range, sceneWidth, sceneHeight);
		QImage TifImage = QImage(image, sceneWidth, sceneHeight, sceneWidth * 3, QImage::Format_RGB888); //��ͼ����Ϣ��RGB888����ʽ����QImage��
		QPixmap ConvertPixmap = QPixmap::fromImage(TifImage);
		QGraphicsPixmapItem* ImageItem = new QGraphicsPixmapItem(ConvertPixmap);
		ImageItem->setPos(sceneStartX, sceneStartY);
		scene->addItem(ImageItem);
		delete[] image;
		image = NULL;
	}
}

bool LMS_Compare_2::checkOverlapExist(LMS_DataInformation* imageDataInformation_1, LMS_DataInformation* imageDataInformation_2) {
	int _UpperRightX_1 = imageDataInformation_1->_UpperLeftPointX + imageDataInformation_1->imageWidth * imageDataInformation_1->deltaX;
	int _LowerLeftY_1 = imageDataInformation_1->_UpperLeftPointY + imageDataInformation_1->imageHeight * imageDataInformation_1->deltaY;
	int _UpperRightX_2 = imageDataInformation_2->_UpperLeftPointX + imageDataInformation_2->imageWidth * imageDataInformation_2->deltaX;
	int _LowerLeftY_2 = imageDataInformation_2->_UpperLeftPointY + imageDataInformation_2->imageHeight * imageDataInformation_2->deltaY;
	return (((imageDataInformation_1->_UpperLeftPointX > imageDataInformation_2->_UpperLeftPointX && imageDataInformation_1->_UpperLeftPointX < _UpperRightX_2)
		|| (imageDataInformation_1->_UpperLeftPointX < imageDataInformation_2->_UpperLeftPointX&& imageDataInformation_2->_UpperLeftPointX < _UpperRightX_1))
		&& ((imageDataInformation_1->_UpperLeftPointY > imageDataInformation_2->_UpperLeftPointY && imageDataInformation_1->_UpperLeftPointY < _LowerLeftY_2)
			|| (imageDataInformation_1->_UpperLeftPointY < imageDataInformation_2->_UpperLeftPointY&& imageDataInformation_2->_UpperLeftPointY < _LowerLeftY_1))) ? true : false;
}

bool LMS_Compare_2::checkGeographicInformationExist(LMS_DataInformation* imageDataInformation) {
	if (true) {
		//Ѱ��ͬ��tfw�ļ�
	}
	if (imageDataInformation->tfwFileName != NULL) {
		if (readTfw(imageDataInformation, imageDataInformation->tfwFileName)) {
			if (util->readTiffInformation(imageDataInformation->imageFileName.toLatin1().data(), imageDataInformation, false)) {
				return true;
			}
			else {
				return false;
			}
		}
		else {
			if (util->readTiffInformation(imageDataInformation->imageFileName.toLatin1().data(), imageDataInformation, true)) {
				QMessageBox::warning(NULL, "����", "tfw�ļ����ܳ����Ѵ�ͼ���ȡ��������", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
				return true;
			}
			else {
				return false;
			}
		}
	}
	else {
		if (util->readTiffInformation(imageDataInformation->imageFileName.toLatin1().data(), imageDataInformation, true)) {
			return true;
		}
		else {
			return false;
		}
	}
}

bool LMS_Compare_2::readTfw(LMS_DataInformation* imageData, QString fileName) {
	if (!QFile(fileName).exists()) {
		QMessageBox::warning(NULL, "������ʾ", "tfw�ļ�������", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
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

void LMS_Compare_2::chooseImage1() {
	QString fileUrl = QFileDialog::getOpenFileName(this, "ѡ��Tif�ļ�", "", "Images(*.tif)");//��ѡ�ļ�
	if (fileUrl.isEmpty())
		return;
	lineEdit_Image1->setText(fileUrl);
}

void LMS_Compare_2::chooseImage2() {
	QString fileUrl = QFileDialog::getOpenFileName(this, "ѡ��Tif�ļ�", "", "Images(*.tif)");//��ѡ�ļ�
	if (fileUrl.isEmpty())
		return;
	lineEdit_Image2->setText(fileUrl);
}

void LMS_Compare_2::chooseTfw1() {
	QString fileUrl = QFileDialog::getOpenFileName(this, "ѡ��Tif�ļ�", "", "Images(*.tfw)");//��ѡ�ļ�
	if (fileUrl.isEmpty())
		return;
	lineEdit_Tfw1->setText(fileUrl);
}

void LMS_Compare_2::chooseTfw2() {
	QString fileUrl = QFileDialog::getOpenFileName(this, "ѡ��Tif�ļ�", "", "Images(*.tfw)");//��ѡ�ļ�
	if (fileUrl.isEmpty())
		return;
	lineEdit_Tfw2->setText(fileUrl);
}

void LMS_Compare_2::on_action_mouseWheel()
{
	if (comparing == false)
	{
		return;
	}
	scene->clear();
	paint(imageDataInformation_1, 0);
	paint(imageDataInformation_2, slider->value());
	view->setScene(scene);
	view->updateTempScene();
}

void LMS_Compare_2::on_action_mouseRelease()
{
	if (comparing == false)
	{
		return;
	}
	scene->clear();
	paint(imageDataInformation_1, 0);
	paint(imageDataInformation_2, slider->value());
	view->setScene(scene);
	view->updateTempScene();
}