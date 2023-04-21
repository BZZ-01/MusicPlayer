
#include "widget.h"
#include "ui_widget.h"


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowTitle("MyMusicPlayer");
    //开始加载multimedia模块
    audioOutput=new QAudioOutput(this);
    //创建媒体播放器
    mediaPlayer=new QMediaPlayer(this);
    mediaPlayer->setAudioOutput(audioOutput);
    //设置音乐（测试）
    //mediaPlayer->setSource(QUrl::fromLocalFile("C:/Users/wypAIR/Music/01. Ring of Fortune.mp3"));
    //mediaPlayer->play();
    //获取时长
    connect(mediaPlayer,&QMediaPlayer::durationChanged,this,[=](qint64 duration)
            {
                ui->totallabel->setText(QString("%1:%2").arg(duration/1000/60,2,10,QChar('0')).arg(duration/1000%60));
                ui->PlayCourseSlider->setRange(0,duration);
            });
    connect(mediaPlayer,&QMediaPlayer::positionChanged,this,[=](qint64 pos)
            {
                ui->curlabel->setText(QString("%1:%2").arg(pos/1000/60,2,10,QChar('0')).arg(pos/1000%60));
            });
    connect(ui->PlayCourseSlider,&QSlider::sliderMoved,mediaPlayer,&QMediaPlayer::setPosition);
    ui->horizontalSlider->setValue(50);
    audioOutput->setVolume(0.5);
    //让播放进度和音乐的进度同步
    connect(mediaPlayer,&QMediaPlayer::positionChanged,ui->PlayCourseSlider,&QSlider::setValue);
    curPlayindex=0;
}

Widget::~Widget()
{
    delete ui;
}


//打开文件夹
void Widget::on_pushButton_clicked()
{
    qInfo("open file");
    auto path= QFileDialog::getExistingDirectory(this,"选择播放音频所在目录","C:\\");//选择要打开的音频文件
    qInfo()<<path;
    QDir dir(path);
    auto musicList= dir.entryList(QStringList()<<"*.mp3"<<"*.wav");
    qInfo()<<musicList;
    for(auto music:musicList)
    {
        playList.append(QUrl::fromLocalFile(path+"/"+music));
        qInfo()<<playList;
    }
    //将音乐的名字放到listwidget中
    ui->listWidget->addItems(musicList);
    ui->listWidget->setCurrentItem(0);
}

//播放
void Widget::on_pushButton_4_clicked()
{
    if(playList.empty())//判错
    {
        return ;
    }
    switch(mediaPlayer->playbackState())
    {
    case QMediaPlayer::PlaybackState::StoppedState:
        {
            //如果没有播放
            curPlayindex=ui->listWidget->currentRow()+1;//这里不加一会出现（ASSERT failure in QList::operator[]: "index out of range"）的错误，即数组越界，值为-1
            qInfo()<<curPlayindex;
            mediaPlayer->setSource(playList[curPlayindex]);
            mediaPlayer->play();break;
        }
    case QMediaPlayer::PlaybackState::PlayingState://如果正在播放
        //ui->pushButton_4->setStyleSheet("QPushButton#pushButton_4{border-image: url(:/new/prefix1/images/Start.png);}");
        ui->pushButton_4->setStyleSheet("QPushButton{border-image: url(:/new/prefix1/images/Start.png);}");
        mediaPlayer->pause();
        break;
    case QMediaPlayer::PlaybackState::PausedState://暂停的
        ui->pushButton_4->setStyleSheet("QPushButton#pushButton_4{border-image: url(:/new/prefix1/images/Stop.png);}");
        mediaPlayer->play();
        break;

    }




}

//上一曲
void Widget::on_pushButton_pre_clicked()
{
    //curPlayindex=(curPlayindex-1)%playList.size();这个会过界
    ui->pushButton_4->setStyleSheet("QPushButton#pushButton_4{border-image: url(:/new/prefix1/images/Stop.png);}");
    --curPlayindex;//可能会出现下标越界，需要做判断
    if(curPlayindex<0)
    {
        curPlayindex=playList.size()-1;
    }
    ui->listWidget->setCurrentRow(curPlayindex);
    mediaPlayer->setSource(playList[curPlayindex]);
    mediaPlayer->play();
}

//下一曲
void Widget::on_pushButton_next_clicked()
{
    /*
    ++curPlayindex;//可能会出现下标越界，需要做判断
    if(curPlayindex>=playList.size())
    {
        curPlayindex=0;
    }
    */
    ui->pushButton_4->setStyleSheet("QPushButton#pushButton_4{border-image: url(:/new/prefix1/images/Stop.png);}");
    curPlayindex=(curPlayindex+1)%playList.size();
    ui->listWidget->setCurrentRow(curPlayindex);
    mediaPlayer->setSource(playList[curPlayindex]);
    mediaPlayer->play();
}



//
void Widget::on_listWidget_doubleClicked(const QModelIndex &index)
{
    curPlayindex=index.row();
    mediaPlayer->setSource(playList[curPlayindex]);
    mediaPlayer->play();
}


void Widget::on_horizontalSlider_valueChanged(int value)
{
    audioOutput->setVolume((float)value/100);//这个vlaue的值在0~100，而setVolume是0~1，所以要除100，并且要使用float而不是int
    ui->pushButton_sound->setStyleSheet("QPushButton#pushButton_sound{border-image: url(:/new/prefix1/images/sound.png);}");
}


void Widget::on_pushButton_sound_clicked()
{
    if(issound)
    {
        issound=false;
        ui->pushButton_sound->setStyleSheet("QPushButton#pushButton_sound{border-image: url(:/new/prefix1/images/Silence.png);}");
        tempsound=(float)ui->horizontalSlider->value()/100;//要先将int转化为float
        qInfo()<<tempsound;
        audioOutput->setVolume(0.0);
    }
    else{
        issound=true;
        ui->pushButton_sound->setStyleSheet("QPushButton#pushButton_sound{border-image: url(:/new/prefix1/images/sound.png);}");
        audioOutput->setVolume(tempsound);
    }
}

