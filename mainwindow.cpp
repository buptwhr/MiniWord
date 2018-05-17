#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    t=new Text;//唯一的主数据结构
    label=NULL;
    labelnum=0;
    state=state_initial;
    //定时刷新图形化界面
    QTimer* time=new QTimer;
    QObject::connect(time,SIGNAL(timeout()),this,SLOT(graphic_datastruct()));
    time->start(20);

    //------------------测试鼠标移动
//    this->setMouseTracking(true);//鼠标操作,设置自动触发
//    ui->centralWidget->setMouseTracking(true);
    //-----------------------------
}

MainWindow::~MainWindow()
{
    delete ui;
}
//完成新建文件   新建 老曹写完了
void MainWindow::on_actionNew_triggered()
{
    d_new * new_d = new d_new;
    new_d->show();
}
//完成打开文件    不完善，提示新建
void MainWindow::on_actionOpen_triggered()//点击打开文件
{
    DialogOpen * open=new DialogOpen;
    QObject::connect(open,SIGNAL(sendfilename(QString)),this,SLOT(receivefilename(QString)));
    open->show();
}
void MainWindow::receivefilename(QString filename)//打开指定文件 slot function
{
    t->Text_Set(filename.toStdString());
}
//完成保存   老曹正在写
void MainWindow::on_actionSave_triggered()
{
    d_save * save_d =new d_save;
    save_d->lineedit_settext(QString::fromStdString(t->filename));
    save_d->show();
}
//完成替换操作
void MainWindow::on_actionreplace_triggered()
{
    d_replace * replace = new d_replace;
    replace->show();
    QObject::connect(replace,SIGNAL(send_replace_message(QString ,QString )),this,SLOT(f_replace(QString,QString)));
}
void MainWindow::f_replace(QString s1, QString s2){
    if(t->Replace(s1.toStdString(),s2.toStdString(),1,1)==1){

    }else{
        QMessageBox::about(this,tr("提示信息"),tr("没有可要替换的内容"));
    }
}


/*
void MainWindow::mouseMoveEvent(QMouseEvent *event)

{
    // 鼠标事件包含了全局坐标与本地坐标，在此只需要本地坐标即可
    QPointF pt = event->localPos();
    // 将坐标点格式化
    QString strShow = QString("当前坐标：X:%1 Y:%2").arg(pt.x()).arg(pt.y());
    // 在SatusBar中实时显示
    ui->statusBar->showMessage(strShow);

}
*/
void MainWindow::graphic_datastruct()//定时执行的槽函数，模拟线程，时刻刷新数据结构到界面
{
    ui->verticalScrollBar->setGeometry(geometry().width()-16,0,16,frameGeometry().height());
    ui->horizontalScrollBar->setGeometry(0,geometry().height()-16,geometry().width(),16);
       //statusbar功能待定 影响美观 无法解决
   // ui->statusBar->showMessage("滑轮位置:"+QString::fromStdString(std::to_string(ui->verticalScrollBar->value())));

    TextNode* text_node=t->headnode;
    if(t->lines!=0){
//        ui->statusBar->showMessage("行数:"+QString::fromStdString(std::to_string(t->lines)));

        int size=20;//字体大小
        float size_width=9.47;
        QFont ft("STSong", size, 50);

        int maxline=geometry().height()/size; // 最多一个页面显示这些行
        int max_in_one_line=geometry().width()/9.47;//每行最多显示这些字符
        int max_length=300;                    //所有内容中最长的那一行有多长
        float print_start_y=0;                     //第一个label打印的高度
        float print_start_x=0;                   //第一个label打印的水平位置

        if(t->lines<=maxline){
            print_start_y=0;
        }
        else
        {
            print_start_y=-(float)size/100*(t->lines-maxline+2)*(ui->verticalScrollBar->value());
        }//控制第一个label打印的高度

        if(max_length<=max_in_one_line){
            print_start_x=0;
        }
        else{
            print_start_x=-(float)9.47/100*(max_length-max_in_one_line)*(ui->horizontalScrollBar->value());
        }


        for(int i=0;i<labelnum;i++)
        {
            label[i]->clear();
            delete label[i];
        }

        QLabel** label_temp=new QLabel*[t->lines];
        for(int i=0;i<t->lines;i++)
        {
            label_temp[i]=new QLabel(this);
        }
        labelnum=t->lines;

        label=label_temp;


 //i<t->lines
        for(int i=0;(i<(maxline-(int)print_start_y/size-1))&&(i<t->lines);i++){
            label[i]->setFont(ft);
            if(text_node->length!=0)
            {
                std::string str_temp;

                TextBlock* nexttextblock=text_node->first;
                for(int j=0,k=0;j<text_node->length;j++)
                {
                    str_temp+=nexttextblock->text[k];
                    if(k==99){
                        k=0;
                        nexttextblock=nexttextblock->next_block;
                    }else{
                        k++;
                    }
                }
                label[i]->setText(QString::fromStdString(str_temp));
//                label[i]->setGeometry(print_start_x,print_start_y+20*i,geometry().width()-16,size);
                label[i]->setGeometry(print_start_x,print_start_y+20*i,9.48*max_length,size);
                text_node=text_node->nextnode;
                label[i]->show();
            }
            else
            {
                label[i]->setText(QString::fromStdString(""));//空字符串
                text_node=text_node->nextnode;
            }

        }
//        for(int i=((0-print_start)/size-3)?((0-print_start)/size-3):0;(i<(0-print_start)/size+maxline)&&(i<t->lines);i++)
//        {
//            label[i]->show();
//        }

      //  ui->statusBar->show();


//        程序暂停一段时间
//        QTime ti;
//        ti.start();
//        while(ti.elapsed()<130)
//         QCoreApplication::processEvents();



    }

}


void MainWindow::wheelEvent(QWheelEvent *event)
{
    int para=event->angleDelta().y();
    int change_size=1;//滑轮滚动灵敏度
    if(para<0)
    {
        if(ui->verticalScrollBar->value()+change_size<100)
        {
            emit ui->verticalScrollBar->valueChanged(ui->verticalScrollBar->value()+change_size);
            ui->verticalScrollBar->setSliderPosition(ui->verticalScrollBar->value()+change_size);
        }
        else
        {
            emit ui->verticalScrollBar->valueChanged(ui->verticalScrollBar->maximum());
            ui->verticalScrollBar->setSliderPosition(ui->verticalScrollBar->maximum());
        }
    }
    else if(para>0)
    {
        if (ui->verticalScrollBar->value()-change_size>=0)
        {
            emit ui->verticalScrollBar->valueChanged(ui->verticalScrollBar->value()-change_size);
            ui->verticalScrollBar->setSliderPosition(ui->verticalScrollBar->value()-change_size);
         }
         else{

            emit ui->verticalScrollBar->valueChanged(ui->verticalScrollBar->minimum());
            ui->verticalScrollBar->setSliderPosition(ui->verticalScrollBar->minimum());
            }
}
}

void MainWindow::on_actionexit_triggered()
{
   exit(0);
}






