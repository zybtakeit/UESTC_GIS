#include "UESTC_GIS.h"

UESTC_GIS::UESTC_GIS(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	iniUI();
	iniSlots();
}

void UESTC_GIS::iniUI() {
	VLayout = new QVBoxLayout();
	VLayout->setMargin(5); //����layout�����ұ߾�
	VLayout->setSpacing(5); //����layout�����±߾�
	HLayout = new QHBoxLayout();
	HLayout_Page1 = new QHBoxLayout();
	HLayout_Page2 = new QHBoxLayout();
	HLayout_Page3 = new QHBoxLayout();

	//��ʾģ��
	mapCanvas = new QgsMapCanvas();
	mapCanvas->enableAntiAliasing(true);
	mapCanvas->setCanvasColor(QColor(255, 255, 0));
	mapCanvas->setVisible(true);
	pixel = mapCanvas->getCoordinateTransform();
	//�϶�ģ��
	toolPan = new QgsMapToolPan(mapCanvas);

	//action����
	action_openImage = new QAction(tr("&��ͼ��"), this);
	action_coordTrans = new QAction(tr("&����ת��"), this);
	action_compareIamge = new QAction(tr("&ͼ��Ƚ�"), this);
	action_panMode = new QAction(tr("&�϶�"), this);
	action_panMode->setCheckable(true); 
	action_addListWidgetItem = new QAction(tr("&���ͼ��"), this);
	action_deleteListWidgetItem = new QAction(tr("&ɾ��ͼ��"), this);
	action_clearImage = new QAction(tr("&���ͼ��"), this);
	action_clearListWidget = new QAction(tr("&����б�"), this);

	//�˵�������
	menuBar = new QMenuBar();
	QMenu* menu_1 = new QMenu("ͼ��");
	menu_1->addAction(action_openImage);
	menu_1->addAction(action_addListWidgetItem);
	menu_1->addAction(action_deleteListWidgetItem);
	menu_1->addAction(action_clearImage);
	menu_1->addAction(action_clearListWidget);
	QMenu* menu_2 = new QMenu("��������");
	menu_2->addAction(action_coordTrans);
	menu_2->addAction(action_compareIamge);
	menuBar->addMenu(menu_1);
	menuBar->addMenu(menu_2);

	//��������һҳPage1
	QWidget* Page1 = new QWidget;
	toolBar_1 = new QToolBar();
	HLayout_Page1->addWidget(toolBar_1);
	toolBar_1->addAction(action_openImage);
	toolBar_1->addAction(action_addListWidgetItem);
	toolBar_1->addAction(action_deleteListWidgetItem);
	toolBar_1->addAction(action_clearImage);
	toolBar_1->addAction(action_clearListWidget);
	Page1->setLayout(HLayout_Page1);
	//�������ڶ�ҳPage2
	QWidget* Page2 = new QWidget;
	toolBar_2 = new QToolBar();
	HLayout_Page2->addWidget(toolBar_2);
	toolBar_2->addAction(action_coordTrans);
	toolBar_2->addAction(action_compareIamge);
	Page2->setLayout(HLayout_Page2);
	//����������ҳPage3
	QWidget* Page3 = new QWidget;
	toolBar_3 = new QToolBar();
	HLayout_Page3->addWidget(toolBar_3);
	toolBar_3->addAction(action_panMode);
	Page3->setLayout(HLayout_Page3);
	//������������
	tabWidget = new QTabWidget();
	tabWidget->addTab(Page1, "ͼ�����");
	tabWidget->addTab(Page2, "��������");
	tabWidget->addTab(Page3, "�鿴ͼ��");
	tabWidget->setFixedHeight(100);

	//�ײ�״̬��
	statusBar = new QStatusBar();
	lineEdit_1 = new QLineEdit("X����Ϊ:");
	lineEdit_1->setFixedWidth(400);
	lineEdit_2 = new QLineEdit("Y����Ϊ:");
	lineEdit_2->setFixedWidth(400);
	statusBar->addWidget(lineEdit_1);
	statusBar->addWidget(lineEdit_2);

	//�б��齨����
	listWidget = new QListWidget();
	listWidget->setFixedWidth(200);
	listWidget->clear();

	//��������
	HLayout->addWidget(listWidget);
	HLayout->addWidget(mapCanvas);
	VLayout->addWidget(menuBar);
	VLayout->addWidget(tabWidget);
	VLayout->addLayout(HLayout);
	VLayout->addWidget(statusBar);
	UESTC_GIS::setLayout(VLayout);
}

void UESTC_GIS::iniSlots() {
	connect(action_panMode, SIGNAL(toggled(bool)), this, SLOT(panMode(bool)));
	connect(action_openImage, SIGNAL(triggered()), this, SLOT(openImage()));
	connect(action_addListWidgetItem, SIGNAL(triggered()), this, SLOT(addListWidgetItem()));
	connect(action_deleteListWidgetItem, SIGNAL(triggered()), this, SLOT(deleteListWidgetItem()));
	connect(action_clearListWidget, SIGNAL(triggered()), this, SLOT(clearListWidget()));
	connect(action_clearImage, SIGNAL(triggered()), this, SLOT(clearImage()));
	connect(listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(listWidgetItemDoubleClicked(QListWidgetItem*)));
	connect(mapCanvas, SIGNAL(xyCoordinates(const QgsPointXY)), this, SLOT(mouseMove(const QgsPointXY)));
	connect(action_coordTrans, SIGNAL(triggered()), this, SLOT(transCoordination()));
	connect(action_compareIamge, SIGNAL(triggered()), this, SLOT(compareImages()));
}

void UESTC_GIS::transCoordination() {
	CSatCoordTrans sat;
	sat.Set_Parameters_ByGUI();
}

void UESTC_GIS::compareImages() {
	QCompare* window = new QCompare(this);
	window->show();
}

void UESTC_GIS::addListWidgetItem() {
	QString fileUrl = QFileDialog::getOpenFileName(this, "ѡ��һ�������ļ�", "", "Images(*.tif)");//��ѡ�ļ�
	if (fileUrl.isEmpty())
		return;
	QFileInfo FileInfo = QFileInfo(fileUrl);
	QString basename = FileInfo.fileName();
	QListWidgetItem* item = new QListWidgetItem();
	item->setText(basename);
	item->setData(1, fileUrl);
	listWidget->addItem(item);
}

void UESTC_GIS::deleteListWidgetItem() {
	int row = listWidget->currentRow();
	QListWidgetItem* item = listWidget->takeItem(row);
	delete item;
}

void UESTC_GIS::clearListWidget() {
	listWidget->clear();
}

void UESTC_GIS::clearImage() {
	qDeleteAll(mapCanvasLayerSet);
	mapCanvasLayerSet.clear();
	mapCanvas->refresh();
}

void UESTC_GIS::listWidgetItemDoubleClicked(QListWidgetItem* item) {
	QString fileUrl = (item->data(1)).toString();
	QString basename = item->text();
	changeRasterLayer(fileUrl, basename);
}

void UESTC_GIS::openImage() {
	QString fileUrl = QFileDialog::getOpenFileName(this, "ѡ��һ�������ļ�", "", "Images(*.tif)");//��ѡ�ļ�
	if (fileUrl.isEmpty())
		return;
	QFileInfo FileInfo = QFileInfo(fileUrl);
	QString basename = FileInfo.fileName();
	QListWidgetItem* item = new QListWidgetItem();
	item->setText(basename);
	item->setData(1, fileUrl);
	listWidget->addItem(item);
	changeRasterLayer(fileUrl, basename);
}

void UESTC_GIS::changeRasterLayer(QString url, QString basename) {
	QgsRasterLayer* rasterLayer = new QgsRasterLayer(url, basename, "gdal", QgsRasterLayer::LayerOptions());
	if (!rasterLayer->isValid())
	{
		QMessageBox::critical(this, "error", "layer is invalid");
		return;
	}
	if (mapCanvasLayerSet.length() == 0) {
		mapCanvasLayerSet.append(rasterLayer);
	}
	else {
		qDeleteAll(mapCanvasLayerSet); 
		mapCanvasLayerSet.clear(); 
		mapCanvasLayerSet.append(rasterLayer);
	}
	QgsRectangle rec = rasterLayer->extent();
	mapCanvas->setExtent(rasterLayer->extent());
	mapCanvas->setLayers(mapCanvasLayerSet);
	mapCanvas->setVisible(true);
	mapCanvas->freeze(false);
	mapCanvas->setVisible(true);
	mapCanvas->freeze(false); 
	mapCanvas->refresh();
}

void UESTC_GIS::mouseMove(const QgsPointXY& p) {
	QgsPointXY a = pixel->toMapCoordinates(p.x(),p.y());
	lineEdit_1->setText("X����Ϊ:" + QString::number(a.x(),'d',10));
	lineEdit_2->setText("Y����Ϊ:" + QString::number(a.y(), 'd', 10));
	//lineEdit_1->setText("X����Ϊ:" + QString::number(p.x()));
	//lineEdit_2->setText("Y����Ϊ:" + QString::number(p.y()));
}

void UESTC_GIS::panMode(bool checked) {
	if (checked) {
		mapCanvas->setMapTool(toolPan);
	}
	else {
		mapCanvas->unsetMapTool(toolPan);
	}
}
