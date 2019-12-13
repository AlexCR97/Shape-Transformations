#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QColorDialog>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>

#include "Shapes/shape.h"
#include "Shapes/shapemodel.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_horizontalSliderScale_valueChanged(int value)
{
    this->ui->renderAreaWidget->setScale(value);
}

// INSPECTOR //////////////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_pushButtonImportShapes_clicked()
{
    this->on_pushButtonClearShapes_clicked();

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Text File"), "", tr("Text Files (*.txt)"));
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly))
        return;

    QHash<QString, QList<cr::Point>> verticesMap;
    QHash<QString, QColor> colorsMap;

    QTextStream data(&file);
    while (!data.atEnd()) {
        QString line = data.readLine();
        QStringList tokens = line.split(" ");

        // x y 1 id r g b
        double x = tokens[0].toDouble();
        double y = tokens[1].toDouble();
        //double c = tokens[2].toDouble();
        QString id = tokens[3];
        int r = tokens[4].toInt();
        int g = tokens[5].toInt();
        int b = tokens[6].toInt();

        verticesMap[id].append(cr::Point(x, y));
        colorsMap[id] = QColor(r, g, b);
    }

    QHash<QString, QList<cr::Point>>::Iterator i;
    for (i = verticesMap.begin(); i != verticesMap.end(); i++) {
        QString id = i.key();

        QList<cr::Point> vertices = i.value();
        cr::Shape shape(vertices);
        QColor color = colorsMap[id];

        cr::ShapeModel model(id, shape, color);
        this->ui->renderAreaWidget->addShape(model);
        this->ui->listWidgetShapes->addItem(id);
    }

    file.close();
}

void MainWindow::on_pushButtonExportShapes_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Export shapes"), "", tr("Text Files (*.txt)"));
    if (fileName.isNull() || fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::information(this, "Failed to open file", file.errorString());
        return;
    }

    QTextStream writer(&file);
    for (int i = 0; i < this->ui->listWidgetShapes->count(); i++) {
        QListWidgetItem *item = this->ui->listWidgetShapes->item(i);
        QString id = item->text();

        cr::ShapeModel& model = this->ui->renderAreaWidget->getShape(id);
        cr::Shape& shape = model.getShape();

        // x y 1 id r g b
        for (cr::Point& p : shape.getVertices()) {
            writer << QString::number(p.x) + " " +
                      QString::number(p.y) +
                      " 1 " +
                      id + " " +
                      QString::number(model.color.red()) + " " +
                      QString::number(model.color.green()) + " " +
                      QString::number(model.color.blue()) << '\n';
        }
    }

    file.close();
}

void MainWindow::on_listWidgetShapes_itemClicked(QListWidgetItem *item)
{
    QString id = item->text();
    cr::ShapeModel& model = this->ui->renderAreaWidget->getShape(id);
    cr::Shape& shape = model.getShape();

    this->ui->listWidgetShapeVertices->clear();
    for (cr::Point& p : shape.getVertices()) {
        QString item = QString::number(p.x) + " " + QString::number(p.y);
        this->ui->listWidgetShapeVertices->addItem(item);
    }

    this->ui->lineEditShapeId->setText(id);
}

void MainWindow::on_pushButtonDeleteShape_clicked()
{
    if (this->ui->listWidgetShapes->selectedItems().count() == 0)
        return;

    auto reply = QMessageBox::question(this, "", "Are you sure you want to delete the selected shape?");
    if (reply != QMessageBox::Yes)
        return;

    QListWidgetItem *selectedShape = this->ui->listWidgetShapes->currentItem();
    QString id = selectedShape->text();

    this->ui->renderAreaWidget->removeShape(id);
    delete selectedShape;
}

void MainWindow::on_pushButtonClearShapes_clicked()
{
    this->ui->listWidgetShapes->clear();
    this->ui->renderAreaWidget->clearShapes();
}

// PROPERTIES ////////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_pushButtonShapeColor_clicked()
{
    QColor c = QColorDialog::getColor();
}

void MainWindow::on_pushButtonAddVertex_clicked()
{
    double x = this->ui->doubleSpinBoxVertexX->value();
    double y = this->ui->doubleSpinBoxVertexY->value();

    QString item = QString::number(x) + " " + QString::number(y);

    this->ui->listWidgetShapeVertices->addItem(item);
}

void MainWindow::on_listWidgetShapeVertices_itemClicked(QListWidgetItem *item)
{
    delete item;
}

void MainWindow::on_pushButtonCreateShape_clicked()
{
    QString id = this->ui->lineEditShapeId->text();
    if (id.isNull() || id.isEmpty()) {
        QMessageBox::information(this, "", "You must enter a valid name");
        return;
    }

    if (this->ui->renderAreaWidget->checkShape(id)) {
        QMessageBox::information(this, "", "A shape with that name already exsists");
        return;
    }

    int verticesCount = this->ui->listWidgetShapeVertices->count();
    if (verticesCount < 1) {
        QMessageBox::information(this, "", "You must enter at least 1 vertex");
        return;
    }

    QList<cr::Point> vertices;
    for (int i = 0; i < verticesCount; i++) {
        QListWidgetItem *item = this->ui->listWidgetShapeVertices->item(i);
        QString line = item->text();
        QStringList tokens = line.split(" ");

        double x = tokens[0].toDouble();
        double y = tokens[1].toDouble();

        vertices.append(cr::Point(x, y));
    }

    cr::Shape shape(vertices);
    QColor color(190, 100, 50);
    cr::ShapeModel model(id, shape, color);

    this->ui->renderAreaWidget->addShape(model);
    this->ui->listWidgetShapes->addItem(id);
}

void MainWindow::on_pushButtonEditShape_clicked()
{
    QString id = this->ui->lineEditShapeId->text();
    if (id.isNull() || id.isEmpty()) {
        QMessageBox::information(this, "", "You must enter a valid name");
        return;
    }

    if (!this->ui->renderAreaWidget->checkShape(id)) {
        QMessageBox::information(this, "", "No shape with the specified name found");
        return;
    }

    int verticesCount = this->ui->listWidgetShapeVertices->count();
    if (verticesCount < 1) {
        QMessageBox::information(this, "", "You must enter at least 1 vertex");
        return;
    }

    QList<cr::Point> vertices;
    for (int i = 0; i < verticesCount; i++) {
        QListWidgetItem *item = this->ui->listWidgetShapeVertices->item(i);
        QString line = item->text();
        QStringList tokens = line.split(" ");

        double x = tokens[0].toDouble();
        double y = tokens[1].toDouble();

        vertices.append(cr::Point(x, y));
    }

    QColor color(190, 100, 50);

    cr::ShapeModel& model = this->ui->renderAreaWidget->getShape(id);
    cr::Shape& shape = model.getShape();
    shape.setVertices(vertices);
    model.color = color;

    QMessageBox::information(this, "", "Successfully edited shape!");
}

// TRANSFORM ////////////////////////////////////////////////////////////////////////////////

void MainWindow::on_horizontalSliderDegrees_valueChanged(int value)
{
    this->ui->labelDegrees->setText(QString::number(value) + " degrees");
}

void MainWindow::on_pushButtonApplyTranslate_clicked()
{
    double xTranslate = this->ui->doubleSpinBoxTranslateX->value();
    double yTranslate = this->ui->doubleSpinBoxTranslateY->value();

    if (!this->ui->checkBoxApplyToAll->isChecked()) {
        if (this->ui->listWidgetShapes->selectedItems().count() == 0) {
            QMessageBox::information(this, "", "You must select a shape or check 'Apply to all' to make a transformation");
            return;
        }

        QListWidgetItem *selectedShape = this->ui->listWidgetShapes->currentItem();
        QString id = selectedShape->text();

        this->ui->renderAreaWidget->applyTransformTranslate(id, cr::Point(xTranslate, yTranslate));
        this->on_listWidgetShapes_itemClicked(selectedShape);

        return;
    }

    for (int i = 0; i < this->ui->listWidgetShapes->count(); i++) {
        QListWidgetItem *selectedShape = this->ui->listWidgetShapes->item(i);
        QString id = selectedShape->text();
        this->ui->renderAreaWidget->applyTransformTranslate(id, cr::Point(xTranslate, yTranslate));
        this->on_listWidgetShapes_itemClicked(selectedShape);
    }
}

void MainWindow::on_pushButtonApplyScale_clicked()
{
    double xScale = this->ui->doubleSpinBoxScaleX->value();
    double yScale = this->ui->doubleSpinBoxScaleY->value();

    if (!this->ui->checkBoxApplyToAll->isChecked()) {
        if (this->ui->listWidgetShapes->selectedItems().count() == 0) {
            QMessageBox::information(this, "", "You must select a shape or check 'Apply to all' to make a transformation");
            return;
        }

        QListWidgetItem *selectedShape = this->ui->listWidgetShapes->currentItem();
        QString id = selectedShape->text();

        this->ui->renderAreaWidget->applyTransformScale(id, cr::Point(xScale, yScale));
        this->on_listWidgetShapes_itemClicked(selectedShape);

        return;
    }

    for (int i = 0; i < this->ui->listWidgetShapes->count(); i++) {
        QListWidgetItem *selectedShape = this->ui->listWidgetShapes->item(i);
        QString id = selectedShape->text();
        this->ui->renderAreaWidget->applyTransformScale(id, cr::Point(xScale, yScale));
        this->on_listWidgetShapes_itemClicked(selectedShape);
    }
}

void MainWindow::on_pushButtonApplyRotation_clicked()
{
    int degrees = this->ui->horizontalSliderDegrees->value();

    if (!this->ui->checkBoxApplyToAll->isChecked()) {
        qDebug() << "Checkbox is not checked";

        if (this->ui->listWidgetShapes->selectedItems().count() == 0) {
            QMessageBox::information(this, "", "You must select a shape or check 'Apply to all' to make a transformation");
            return;
        }

        QListWidgetItem *selectedShape = this->ui->listWidgetShapes->currentItem();
        QString id = selectedShape->text();

        this->ui->renderAreaWidget->applyTransformRotate(id, degrees);
        this->on_listWidgetShapes_itemClicked(selectedShape);

        return;
    }

    for (int i = 0; i < this->ui->listWidgetShapes->count(); i++) {
        QListWidgetItem *selectedShape = this->ui->listWidgetShapes->item(i);
        QString id = selectedShape->text();
        this->ui->renderAreaWidget->applyTransformRotate(id, degrees);
        this->on_listWidgetShapes_itemClicked(selectedShape);
    }
}
