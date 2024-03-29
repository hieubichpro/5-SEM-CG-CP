#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    int width = ui->graphicsView->width();
    int height = ui->graphicsView->height();
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->graphicsView->setScene(new QGraphicsScene(0, 0, width, height));
    manager = SceneManager(width, height, Qt::black, ui->graphicsView->scene());
    auto listStr = new QStringList();
    model = new QStringListModel(*listStr);
    ui->listView->setModel(model);
    ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    connect(ui->listView, SIGNAL(clicked(QModelIndex)), this, SLOT(onListViewClicked(QModelIndex)));
    ui->graphicsView_2->setScene(new QGraphicsScene(0, 0, ui->graphicsView_2->width(), ui->graphicsView_2->height()));
    ui->graphicsView_2->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView_2->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    auto f = [&](type t, float dist)
    {
        manager.moveCamera(t, dist);
    };

    auto filter = new Filter(f);
    ui->graphicsView->installEventFilter(filter);
    manager.initialize();
    disableAll(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onListViewClicked(QModelIndex idx)
{
    disableAll(true);
    if (!idx.isValid())
        return;
    auto name_obj = this->model->index(idx.row()).data(Qt::DisplayRole).toString();
    if (prev_obj != "")
        save_data_obj(name_data.at(prev_obj));
    manager.setCurrModel(name_id.at(name_obj));
    if (name_data.count(name_obj))
        fill_data_obj(name_data.at(name_obj));
    prev_obj = name_obj;
}

void MainWindow::fill_data_obj(UI_data &data_obj)
{
    ui->move_x->setValue(data_obj.shift_x);
    ui->move_y->setValue(data_obj.shift_y);
    ui->move_z->setValue(data_obj.shift_z);

    ui->rotate_x->setValue(data_obj.rot_x);
    ui->rotate_y->setValue(data_obj.rot_y);
    ui->rotate_z->setValue(data_obj.rot_z);

    ui->scale_x->setValue(data_obj.scale_x);
    ui->scale_y->setValue(data_obj.scale_y);
    ui->scale_z->setValue(data_obj.scale_z);

    ui->ambient->setValue(data_obj.ambient);
    ui->diffuse->setValue(data_obj.diffuse);
    ui->specular->setValue(data_obj.specular);
    ui->alpha->setValue(data_obj.alpha);
}

void MainWindow::save_data_obj(UI_data &data_obj)
{
    data_obj.shift_x = ui->move_x->value();
    data_obj.shift_y = ui->move_y->value();
    data_obj.shift_z = ui->move_z->value();

    data_obj.rot_x = ui->rotate_x->value();
    data_obj.rot_y = ui->rotate_y->value();
    data_obj.rot_z = ui->rotate_z->value();

    data_obj.scale_x = ui->scale_x->value();
    data_obj.scale_y = ui->scale_y->value();
    data_obj.scale_z = ui->scale_z->value();

    data_obj.ambient = ui->ambient->value();
    data_obj.diffuse = ui->diffuse->value();
    data_obj.specular = ui->specular->value();
    data_obj.alpha = ui->alpha->value();

}

void MainWindow::on_add_object_clicked()
{
    auto name_obj = ui->obj_list->currentText();
    auto name_obj_numerical = name_obj;
    if (!name_data.count(name_obj))
        name_data.insert({name_obj, UI_data{}});
    else
    {
        auto numerical = ++name_data.at(name_obj).amount;
        name_obj_numerical += QString("%1").arg(numerical);
        name_data.insert({name_obj_numerical, UI_data{}});
    }
    int id = 0;
    manager.uploadModel(name_obj.toStdString(), id);
    if (id > 0)
    {
        model->insertRow(model->rowCount());
        QModelIndex index = model->index(model->rowCount()-1);
        model->setData(index, name_obj_numerical);
        name_id.insert({name_obj_numerical, id});
    }
    return;
}

void MainWindow::on_delete_object_clicked()
{
    auto idx = ui->listView->currentIndex();
    if (!idx.isValid())
        return;
    this->model->removeRow(idx.row());
    prev_obj = "";
    manager.deleteModel();
    return;
}

void MainWindow::on_add_texture_clicked()
{
    QString fname = QFileDialog::getOpenFileName(this, "Выберите текстуру", "C:/Users/abcd2/OneDrive/Desktop/5-SEM-CG-CP/src/textures", "JPG (*.jpg);;PNG (*.png)");
    QImage img;
    if (!img.load(fname))
        return;
    ui->graphicsView_2->scene()->addPixmap(QPixmap::fromImage(
                                               img.scaled(ui->graphicsView_2->width(), ui->graphicsView_2->height())
                                               ));
    manager.setTexture(img);
    return;
}

void MainWindow::on_procedural_clicked()
{
    QImage img= QImage(400, 400, QImage::Format_RGB32);
    float w = img.width(), h = img.height();
    auto name_function = ui->function_list->currentText().toStdString();
    if (name_function == "f = (1 + sin(50x)) / 2")
    {
        for (int i = 0; i < w; i++)
            for (int j = 0; j < h; j++)
                img.setPixelColor(i, j, qRgb((1 + sin(i * 50)) / 2 * 255.f, (1 + sin(i * 50)) / 2 * 255.f, (1 + sin(i * 50)) / 2 * 255.f));
    }
    else if (name_function == "f = 1/2 + 1/2 * sin(50x) * cos(50y)")
    {
        for (int i = 0; i < w; i++)
            for (int j = 0; j < h; j++)
                img.setPixelColor(i, j, qRgb(0.5 + 0.5 * sin(50 * i) * cos(50 * j) * 255.f, 0.5 + 0.5 * sin(50 * i) * cos(50 * j) * 255.f, 0.5 + 0.5 * sin(50 * i) * cos(50 * j) * 255.f));
    }
    else
    {
        for (int i = 0; i < w; i++)
            for (int j = 0; j < h; j++)
                img.setPixelColor(i, j, qRgb(sin(i * 50) * j / h * 255.f, sin(i * 50) * j / h * 255.f,  sin(i * 50) * j / h * 255.f));
    }
    ui->graphicsView_2->scene()->addPixmap(QPixmap::fromImage(
                                               img.scaled(ui->graphicsView_2->width(), ui->graphicsView_2->height())
                                               ));
    manager.setTexture(img);
    return;
}

void MainWindow::on_bump_clicked()
{
    manager.bump_map();
}

//void MainWindow::on_test_clicked()
//{
//    manager.test_proga();
//}

void MainWindow::on_move_x_valueChanged(double param)
{
    manager.move(MOVE_X, param);
}
void MainWindow::on_move_y_valueChanged(double param)
{
    manager.move(MOVE_Y, param);
}
void MainWindow::on_move_z_valueChanged(double param)
{
    manager.move(MOVE_Z, param);
}
void MainWindow::on_scale_x_valueChanged(double param)
{
    manager.scale(SCALE_X, param);
}
void MainWindow::on_scale_y_valueChanged(double param)
{
    manager.scale(SCALE_Y, param);
}
void MainWindow::on_scale_z_valueChanged(double param)
{
    manager.scale(SCALE_Z, param);
}

void MainWindow::on_rotate_x_valueChanged(double param)
{
    manager.rotate(ROTATE_X, param);
}
void MainWindow::on_rotate_y_valueChanged(double param)
{
    manager.rotate(ROTATE_Y, param);
}
void MainWindow::on_rotate_z_valueChanged(double param)
{
    manager.rotate(ROTATE_Z, param);
}

void MainWindow::on_intensity_valueChanged(double param)
{
    manager.changeIntensity(param);
}

void MainWindow::on_ambient_valueChanged(double param)
{
    manager.changeAmbient(param);
}
void MainWindow::on_diffuse_valueChanged(double param)
{
    manager.changeDiffuse(param);
}
void MainWindow::on_specular_valueChanged(double param)
{
    manager.changeSpecular(param);
}
void MainWindow::on_alpha_valueChanged(double param)
{
    manager.changeAlpha(param);
}

float move_dist = 0.5;
int rot_angle = 5;

bool Filter::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        switch (keyEvent->key())
        {
            case Qt::Key_W:
                f(MOVE_Z, move_dist);
                break;
            case Qt::Key_D:
                f(MOVE_X, -move_dist);
                break;
            case Qt::Key_A:
                f(MOVE_X, move_dist);
                break;
            case Qt::Key_S:
                f(MOVE_Z, -move_dist);
                break;
            case Qt::Key_H:
                f(ROTATE_Y, rot_angle);
                break;
            case Qt::Key_U:
                f(ROTATE_X, -rot_angle);
                break;
            case Qt::Key_K:
                f(ROTATE_Y, -rot_angle);
                break;
            case Qt::Key_J:
                f(ROTATE_X, rot_angle);
                break;
        }
        return true;
    }
    return QObject::eventFilter(obj, event);
}

void MainWindow::on_top_light_clicked()
{
    manager.moveLight(TOP);
}
void MainWindow::on_down_light_clicked()
{
    manager.moveLight(DOWN);
}
void MainWindow::on_left_light_clicked()
{
    manager.moveLight(LEFT);
}
void MainWindow::on_right_light_clicked()
{
    manager.moveLight(RIGHT);
}
void MainWindow::on_near_light_clicked()
{
    manager.moveLight(NEAR);
}
void MainWindow::on_far_light_clicked()
{
    manager.moveLight(FAR);
}

void MainWindow::disableAll(bool flag){

    ui->move_x->setEnabled(flag);
    ui->move_y->setEnabled(flag);
    ui->move_z->setEnabled(flag);

    ui->rotate_x->setEnabled(flag);
    ui->rotate_y->setEnabled(flag);
    ui->rotate_z->setEnabled(flag);

    ui->scale_x->setEnabled(flag);
    ui->scale_y->setEnabled(flag);
    ui->scale_z->setEnabled(flag);

    ui->ambient->setEnabled(flag);
    ui->diffuse->setEnabled(flag);
    ui->specular->setEnabled(flag);
    ui->alpha->setEnabled(flag);

    ui->add_texture->setEnabled(flag);
    ui->procedural->setEnabled(flag);
    ui->bump->setEnabled(flag);
}
