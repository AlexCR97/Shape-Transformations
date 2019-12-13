#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QListWidget>
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    // Inspector ////////////////////////////////////////////////////////
    void on_pushButtonImportShapes_clicked();

    void on_horizontalSliderScale_valueChanged(int value);

    void on_pushButtonExportShapes_clicked();

    void on_pushButtonDeleteShape_clicked();

    void on_pushButtonClearShapes_clicked();

    void on_pushButtonAddVertex_clicked();

    void on_listWidgetShapeVertices_itemClicked(QListWidgetItem *item);

    void on_pushButtonShapeColor_clicked();

    void on_pushButtonCreateShape_clicked();

    void on_pushButtonEditShape_clicked();

    void on_listWidgetShapes_itemClicked(QListWidgetItem *item);

    void on_horizontalSliderDegrees_valueChanged(int value);

    void on_pushButtonApplyTranslate_clicked();

    void on_pushButtonApplyScale_clicked();

    void on_pushButtonApplyRotation_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
