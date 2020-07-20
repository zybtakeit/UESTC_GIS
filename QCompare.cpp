#include "QCompare.h"

QCompare::QCompare(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	iniUI();
	iniSignalSlots();

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

QCompare::~QCompare()
{
}

void QCompare::iniUI() {
	QWidget* w = new QWidget();
	this->setCentralWidget(w);

	action_addListWidgetItem = new QAction(tr("&���ͼ��"), this);

	//�˵�������
	menuBar = new QMenuBar();
	QMenu* menu_1 = new QMenu("ͼ��");
	menu_1->addAction(action_addListWidgetItem);
	QMenu* menu_2 = new QMenu("��������");
	menuBar->addMenu(menu_1);

	VLayout1 = new QVBoxLayout();
	VLayout2 = new QVBoxLayout();
	VLayout2_1 = new QVBoxLayout();
	HLayout2_1 = new QHBoxLayout();
	HLayout = new QHBoxLayout(this);
	//HLayout2_1->setSizeConstraint(QLayout::SetMinimumSize);
	//VLayout1->setSizeConstraint(QLayout::SetFixedSize);
	button = new QPushButton();
	//�����
	dockWidget = new QDockWidget();
	//��״TreeWidget
	treeWidget = new QTreeWidget();
	iniTree();
	//
	groupBox = new QGroupBox;
	//����
	spinBox = new QSpinBox;
	spinBox->setMaximum(100);
	spinBox->setMinimum(1);
	spinBox->setSingleStep(1);
	//spinBox->setFixedHeight(20);
	//������
	slider = new QSlider;
	slider->setOrientation(Qt::Horizontal);
	slider->setMaximum(100);
	slider->setMinimum(1);
	slider->setSingleStep(1);
	//;
	label = new QLabel;
	HLayout2_1->addWidget(spinBox);
	HLayout2_1->addWidget(slider);
	groupBox->setLayout(HLayout2_1);
	groupBox->setFixedHeight(60);

	mapCanvas = new QgsMapCanvas();
	mapCanvas->enableAntiAliasing(true);
	mapCanvas->setCanvasColor(QColor(255, 255, 0));
	mapCanvas->setVisible(true);

	VLayout2->addWidget(groupBox);
	VLayout2->addWidget(mapCanvas);
	VLayout1->addWidget(treeWidget);
	HLayout->addLayout(VLayout1);
	HLayout->addLayout(VLayout2);
	w->setLayout(HLayout);
}

void QCompare::iniTree() {
	QString dataStr = "";
	treeWidget->clear();
	treeWidget->setColumnCount(2);
	treeWidget->setFixedWidth(300);
	QStringList labelList;
	labelList.append("ͼƬ");
	labelList.append("ͼƬλ��");
	treeWidget->setHeaderLabels(labelList);
}

void QCompare::iniSignalSlots() {
	//�����źŲ�
	connect(spinBox, SIGNAL(valueChanged(int)), slider, SLOT(setValue(int)));
	connect(slider, SIGNAL(valueChanged(int)), spinBox, SLOT(setValue(int)));
}
//���ͼ��
void QCompare::on_action_addImage_triggered() {
	QString fileUrl = QFileDialog::getOpenFileName(this, "ѡ��һ�������ļ�", "", "Images(*.tif)");//��ѡ�ļ�
	if (fileUrl.isEmpty())
		return;
	QFileInfo FileInfo = QFileInfo(fileUrl);
	QString filename = FileInfo.fileName();
	QTreeWidgetItem* item = new QTreeWidgetItem(QCompare::itTopitem);
	item->setText(QCompare::colItem, filename);
	item->setText(QCompare::colItemType, fileUrl);
	item->setCheckState(colItem,Qt::Unchecked);
	item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsAutoTristate);
	treeWidget->addTopLevelItem(item);
}
//ͼ��Ա�
void QCompare::on_action_compareImage_triggered() {
	if (treeWidget->topLevelItemCount() == 0)
		return;
	int checkNumber = 0;
	QVector<int> itemChecked; //�洢ѡ���ͼ������
	QTreeWidgetItem* item;
	for (int i = 0; i < treeWidget->topLevelItemCount(); i++) {
		item = treeWidget->topLevelItem(i);
		if (item->checkState(0) == Qt::Checked)
			itemChecked.append(i);
			checkNumber++;
	}
	//���ѡ�������ţ��Ϳ�ʼ�Ա�
	if (itemChecked.size() == 2) {  
		const char* Url_1 = (treeWidget->topLevelItem(itemChecked.at(0))->text(1)).toLatin1().data();
		const char* Url_2 = (treeWidget->topLevelItem(itemChecked.at(1))->text(1)).toLatin1().data();
		qDebug() << "��ʼ�Ա�";
	}
	//ѡ��������Ϊ2�Ĵ�����ʾ
	else {  
		QMessageBox::information(NULL, "������ʾ", "�Ա�ʱ��Ҫѡ������ͼ��", QMessageBox::Ok);
	}
}

//ɾ��ѡ��ͼ��
void QCompare::on_action_removeImage_triggered() {
	QTreeWidgetItem* item = treeWidget->currentItem();
	if (item == NULL)
		return;
	treeWidget->takeTopLevelItem(treeWidget->indexOfTopLevelItem(item));
	delete item;
}