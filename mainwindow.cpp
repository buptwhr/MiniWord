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



    scrollArea=new QScrollArea(this);
    setCentralWidget(scrollArea);
    screen= new QWidget;


    cursor=new QLabel("▲",screen);
    QFont ft;
    ft.setPointSize(10);
    cursor->setFont(ft);
    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::red);
    cursor->setPalette(pa);

    //定时刷新图形化界面
//    QTimer* time=new QTimer;
//    QObject::connect(time,SIGNAL(timeout()),this,SLOT(graphic_datastruct()));
//    time->start(100);
   // centralWidget()->setAttribute(Qt::WA_InputMethodEnabled, true);
        this->setFocus();
       scrollArea->setAttribute(Qt::WA_InputMethodEnabled, true);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete screen;
    delete scrollArea;
}
//完成新建文件
void MainWindow::on_actionNew_triggered()
{
    if(state==state_initial||state==state_saved){
        t->Clear();
        d_new * new_d = new d_new;
        QObject::connect(new_d,SIGNAL(sendfilename(QString)),this,SLOT(receiveNewfilename(QString)));
        new_d->show();
    }
    else{
        QMessageBox::about(this,tr("提示信息"),tr("编辑区有未保存内容，请先保存"));
    }

}
void MainWindow::receiveNewfilename(QString filename){
    t->New_File(filename.toStdString());
    graphic_datastruct();
    state=state_saved;
    QMessageBox::about(this,tr("提示信息"),tr("新建成功"));
}
//完成打开文件
void MainWindow::on_actionOpen_triggered()//点击打开文件
{
    if(state==state_editing){
        QMessageBox::about(this,tr("提示信息"),tr("编辑区有未保存内容，请先保存"));
    }
    else{
        t->Clear();
        DialogOpen * open=new DialogOpen;
        QObject::connect(open,SIGNAL(sendfilename(QString)),this,SLOT(receivefilename(QString)));
        open->show();
    }

}

void MainWindow::receivefilename(QString filename)//打开指定文件 slot function
{
    if(t->Text_Set(filename.toStdString())==0){
        QMessageBox::about(this,tr("提示信息"),tr("该文件不存在，已帮您新建"));
    }
    graphic_datastruct();
    state=state_saved;
}
//完成保存
void MainWindow::on_actionSave_triggered()

{
    if(state==state_editing){
        d_save * save_d =new d_save;
        save_d->lineedit_settext(QString::fromStdString(t->filename));
        QObject::connect(save_d,SIGNAL(sendfilename(QString)),this,SLOT(receiveSavefilename(QString)));
        save_d->show();
    }
}
void MainWindow::receiveSavefilename(QString filename){
    t->Save_File(filename.toStdString());
    state=state_saved;
    QMessageBox::about(this,tr("提示信息"),tr("保存成功"));
}
//完成退出操作
void MainWindow::on_actionexit_triggered()
{
    if(state==state_editing){
        QMessageBox::about(this,tr("提示信息"),tr("编辑区有未保存内容，请先保存"));
    }
    else{
        exit(0);
    }
}

//完成替换操作
void MainWindow::on_actionreplace_triggered()
{
    d_replace * replace = new d_replace;
    QObject::connect(replace,SIGNAL(send_replace_message(QString ,QString )),this,SLOT(f_replace(QString,QString)));
    replace->show();
}
void MainWindow::f_replace(QString s1, QString s2){
    if(t->Replace(s1.toStdString(),s2.toStdString(),1,1)==1){
        state=state_editing;
    }else{
        QMessageBox::about(this,tr("提示信息"),tr("没有可要替换的内容"));
    }
}

//使用说明
void MainWindow::on_actioninstruction_triggered()
{
    QMessageBox::about(this,tr("使用说明"),tr("光标位移：ctrl/command + I/K/J/L 完成上下左右移动\nctrl + <／> 光标移动到本行的左端或右端\nHome:移动到本行开头\tEnd:移动到本行结尾\n\n打开：ctrl + O\t新建： ctrl + N\n保存：ctrl + S\t强制退出：ctrl + Q／点叉号\n复制：ctrl + C\t粘贴：ctrl + V\n剪切：ctrl + X\t查找：ctrl + F\n替换：ctrl + R"));
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
    TextNode* text_node=t->headnode;


    int size=20;//字体大小
    float size_width=10;//字体的宽度  9.47
    QFont ft("PCMyungjo", size, 25);

    int maxline=geometry().height()/size; // 最多一个页面显示这些行
    int max_length=t->Get_Max_Length();               //所有内容中最长的那一行有多长


    screen->resize(max_length*size_width,(t->lines+1)*20);
  //释放上一轮的空间  更新label
    for(int i=0;i<labelnum;i++)
        delete label[i];

    QLabel** label_temp=new QLabel*[t->lines];
    labelnum=t->lines;
    label=label_temp;


 //字符串复制给Qlabel 数组循环
        for(int i=0;i<t->lines;i++){
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

                label[i]=new QLabel(QString::fromStdString(str_temp),screen);
                label[i]->setFont(ft);
                label[i]->move(0,20*i);
                label[i]->show();
                text_node=text_node->nextnode;

            }
            else
            {
                label[i]=new QLabel("",screen);
                label[i]->setFont(ft);
                label[i]->move(0,20*i);
                label[i]->show();

                text_node=text_node->nextnode;
            }
        }
        cursor->move((t->cursor.English*10)+(t->cursor.Chinese*20)-5,(t->cursor.line-1)*20+13);
        scrollArea->setWidget(screen);
}


//void MainWindow::wheelEvent(QWheelEvent *event)
//{
//    int para=event->angleDelta().y();
//    int change_size=1;//滑轮滚动灵敏度
//    if(para<0)
//    {
//        if(ui->verticalScrollBar->value()+change_size<100)
//        {
//            emit ui->verticalScrollBar->valueChanged(ui->verticalScrollBar->value()+change_size);
//            ui->verticalScrollBar->setSliderPosition(ui->verticalScrollBar->value()+change_size);
//        }
//        else
//        {
//            emit ui->verticalScrollBar->valueChanged(ui->verticalScrollBar->maximum());
//            ui->verticalScrollBar->setSliderPosition(ui->verticalScrollBar->maximum());
//        }
//    }
//    else if(para>0)
//    {
//        if (ui->verticalScrollBar->value()-change_size>=0)
//        {
//            emit ui->verticalScrollBar->valueChanged(ui->verticalScrollBar->value()-change_size);
//            ui->verticalScrollBar->setSliderPosition(ui->verticalScrollBar->value()-change_size);
//         }
//         else{

//            emit ui->verticalScrollBar->valueChanged(ui->verticalScrollBar->minimum());
//            ui->verticalScrollBar->setSliderPosition(ui->verticalScrollBar->minimum());
//            }
//}
//}

void MainWindow::keyPressEvent(QKeyEvent *event){
    if(state!=state_initial){//有文本时允许的操作
        switch(event->key()){
        case Qt::Key_Backspace:
               state=state_editing;
            break;
        case Qt::Key_Return://回车
            t->Insert_at_Cursor("\n");
            state=state_editing;
            break;
        case Qt::Key_Home:
            t->MoveCursor_to_start();
            break;
        case Qt::Key_End:
            t->MoveCursor_to_end();
            break;
        case Qt::Key_Left://左
            t->MoveCursor(Text::left);
            graphic_datastruct();
            break;
        case Qt::Key_Down://下
            t->MoveCursor(Text::down);
            graphic_datastruct();
            break;
        case Qt::Key_Right://右
            t->MoveCursor(Text::right);
            graphic_datastruct();
            break;
        case Qt::Key_Up://上
            t->MoveCursor(Text::up);
            graphic_datastruct();
            break;
        default://输入字符串
            QString s= event->text();
            if(s!=""){
                t->Insert_at_Cursor(s.toStdString());
                state=state_editing;
            }
            break;
        }

    }
    if(event->modifiers()==Qt::ControlModifier){//ctrl+快捷键
        switch(event->key()){
        case Qt::Key_S://保存
            this->on_actionSave_triggered();
            break;
        case Qt::Key_N://新建
            this->on_actionNew_triggered();
            break;
        case Qt::Key_R://替换
            this->on_actionreplace_triggered();
            break;
        case Qt::Key_O://打开
            this->on_actionOpen_triggered();
            break;
        case Qt::Key_Q://退出
            this->on_actionexit_triggered();
            break;
        case Qt::Key_Comma://移动到本行开头
            t->MoveCursor_to_start();
            break;
        case Qt::Key_Period://移动到本行结尾
            t->MoveCursor_to_end();
            break;
        }
    }

    switch(event->key()){//常规热键
    case Qt::Key_F1://使用说明
        this->on_actioninstruction_triggered();
        break;
    }


    graphic_datastruct();


}

void MainWindow::inputMethodEvent(QInputMethodEvent *event){//输入法事件
    if(state!=state_initial){

        QString s=event->preeditString();
        if(s!=""){
            t->Insert_at_Cursor("555");//s.toStdString()
            state=state_editing;
            graphic_datastruct();
        }
    }
}





