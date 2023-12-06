#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QStringListModel>
#include <QColorDialog>
#include <QFileDialog>
#include <scene_manager.h>

struct UI_data{
    uint32_t amount = 0;

    float shift_x = 0, shift_y = 0, shift_z = 0;
    float rot_x = 0, rot_y = 0, rot_z = 0;
    float scale_x = 1.f, scale_y = 1.f, scale_z = 1.f;

    QImage img;
};

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    SceneManager manager;
    ~MainWindow();

private slots:
    void onListViewClicked(QModelIndex idx);
    void on_add_object_clicked();
    void on_delete_object_clicked();
    void on_add_texture_clicked();
    void on_procedural_clicked();

    void on_move_x_valueChanged(double param);
    void on_move_y_valueChanged(double param);
    void on_move_z_valueChanged(double param);
    void on_scale_x_valueChanged(double param);
    void on_scale_y_valueChanged(double param);
    void on_scale_z_valueChanged(double param);
    void on_rotate_x_valueChanged(double param);
    void on_rotate_y_valueChanged(double param);
    void on_rotate_z_valueChanged(double param);

    void on_top_light_clicked();
    void on_down_light_clicked();
    void on_left_light_clicked();
    void on_right_light_clicked();
    void on_near_light_clicked();
    void on_far_light_clicked();

    void fill_data_obj(UI_data &data_obj);
    void save_data_obj(UI_data &data_obj);
private:
    Ui::MainWindow *ui;
    QStringListModel *model;
    std::map<QString, int> name_id;
    std::map<QString, UI_data> name_data;
    QString prev_obj = "";
};

class Filter: public QObject{
    Q_OBJECT
public:
    Filter(std::function<void (type, float)> f_): f{f_}{}
protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
private:
    std::function<void (type, float)> f;
};


#endif // MAINWINDOW_H
