
#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QDebug>
#include<QFileDialog>
#include<QDir>
#include<QMediaPlayer>
#include<QAudioOutput>
#include<QUrl>
#include<QList>
#include<QString>
#include <QVBoxLayout>
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget

{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_pre_clicked();

    void on_pushButton_next_clicked();

    void on_listWidget_doubleClicked(const QModelIndex &index);

    void on_horizontalSlider_valueChanged(int value);

    void on_pushButton_sound_clicked();

private:
    Ui::Widget *ui;

    QList<QUrl> playList;
    QAudioOutput* audioOutput;
    QMediaPlayer* mediaPlayer;
    int curPlayindex=0;//表示播放到那一曲了
    bool issound=true;//是否开声音
    float tempsound=0;
};

#endif // WIDGET_H
