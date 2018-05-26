#include "datastruct.h"

void Text::New_File(std::string File_Name){
    //新建文件
    filename = File_Name;
    file.open(File_Name, std::ios_base::out);
    headnode = new TextNode;
    tailnode = headnode;
    file.close();
}

/*void fileCopy(char *file1, char *file2)
{   //另存为
    // 对file1和file2进行判断

    ifstream in(file1);
    ofstream out(file2);
    string filename;
    string line;

    while (getline (in, line))
    {
        out << line << endl;
    }
}*/

void Text::Save_File(std::string New_Name){
    //保存文件
    file.close();
    file.open(filename, std::ios_base::out);

    TextNode *cur_node=headnode;
    //写入文件
    for(int i = 1; i <= lines && cur_node;i++){
        for(int j=0;j<cur_node->length && cur_node;j++){
            file<<(*cur_node)[j];
        }
        file<<std::endl;
        cur_node=cur_node->nextnode;
    }
    file.close();
    std::rename(filename.c_str(),New_Name.c_str());
    this->filename=New_Name;

}


void Text::Quit_File(){
    //退出系统
    file.close();
}

void Text::Remove_BOM(char *buffer){
    if(lines!=0){
        headnode->length-=3;
        for(int i=0;i<headnode->length;i++){
            buffer[i]=(*headnode)[i];
            if(i>=0&&i<=2){
                if((*headnode)[i+3]!='\0')
                    (*headnode)[i]='\0';
            }
            (*headnode)[i]=(*headnode)[i+3];

        }

    }
}

void Text::Add_BOM(char *buffer){
    if(lines!=0){
        for(int i=0;i<3;i++){

            if((*headnode)[i])
                (*headnode)[i+3]=(*headnode)[i];
            (*headnode)[i]=buffer[i];
        }
    }
}

void Text::Count_CE(){
    char ch;
    cursor.Chinese=0;
    cursor.English=0;
    TextNode *cur_node=headnode;
    int cur_position=1;
    for(int i=1;i<cursor.line;i++){
        cur_node=cur_node->nextnode;
    }
    while(cur_position<cursor.position){
        ch=(*cur_node)[cur_position-1];
        if(ch<0){//中文字符
            cursor.Chinese++;
            cur_position+=3;
        }
        else{
            cursor.English++;
            cur_position++;
        }
    }
}

void Text::MoveCursor(direction dir){
    int pre_position,cur_position;
    int cur_line=1;
    TextNode *cur_node=headnode;
    TextNode *pre_node=headnode;
    char ch;
    cursor.Chinese=0;
    cursor.English=0;
    if(dir==left){
        cur_line=cursor.line;
        if(cursor.position==1){//光标在行首
            if(cur_line!=1){//光标不在第一行
                cur_line--;
                for(int i=1;i<cur_line;i++){
                    cur_node=cur_node->nextnode;
                }
                //计算中英文字符个数
                for(int i=0;i<cur_node->length;){
                    ch=(*cur_node)[i];
                    if(ch<0){//中文字符
                        cursor.Chinese++;
                        i+=3;
                    }
                    else{
                        cursor.English++;
                        i++;
                    }
                }
                cursor.position=cur_node->length+1;
                cursor.line=cur_line;
            }

        }
        else{
            for(int i=1;i<cur_line;i++){
                cur_node=cur_node->nextnode;
            }
            cur_position=1;
            /*if(cur_line==1){//跳过BOM
                cur_position+=3;
                cursor.position+=3;
            }*/
            //光标右移同时记录前一位置
            while(cur_position!=cursor.position){
                pre_position=cur_position;
                ch=(*cur_node)[cur_position-1];

                if(ch<0){//中文字符
                    cursor.Chinese++;
                    cur_position+=3;
                }
                else{
                    cursor.English++;
                    cur_position++;
                }

            }
            cursor.line=cur_line;
            cursor.position=pre_position;
        }
    }

    else if(dir==right){
        cur_line=cursor.line;
        for(int i=1;i<cur_line;i++){
            cur_node=cur_node->nextnode;
        }

        if(cursor.position>=cur_node->length+1){//光标在行尾
            if(cur_node!=tailnode){//光标不在最后一行
                cur_line++;
                cursor.position=1;
            }
            cursor.line=cur_line;
        }
        else{
            ch=(*cur_node)[cursor.position-1];//光标右边第一个字符
            //光标位置右移
            if(ch<0){
                cursor.position+=3;
            }
            else{
                cursor.position++;
            }
            //计算中英文字符个数
            cur_position=1;
            while(cur_position!=cursor.position){
                pre_position=cur_position;
                ch=(*cur_node)[cur_position-1];
                if(ch<0){//中文字符
                    cursor.Chinese++;
                    cur_position+=3;
                }
                else{
                    cursor.English++;
                    cur_position++;
                }
            }
        }
    }

    else if(dir==up){

        cur_line=cursor.line;
        if(cur_line!=1){
            for(int i=1;i<cur_line;i++){
                pre_node=cur_node;
                cur_node=cur_node->nextnode;
            }
            int cur_len=0;//算每行对应位置(打印长度)
            pre_position=1;
            cur_position=1;
            //找到当前行打印长度cur_len

            while(cur_position!=cursor.position){
                ch=(*cur_node)[cur_position-1];
                if(ch<0){//中文字符
                    cur_len+=2;
                    cur_position+=3;
                }
                else{
                    cur_len++;
                    cur_position++;
                }
            }
           // std::cout<<"cur_len="<<cur_len<<std::endl;
            //找出上一行对应位置pre_position
            while(cur_len>0&&(pre_position<=pre_node->length)){
                ch=(*pre_node)[pre_position-1];
                if(ch<0){//中文字符
                    cursor.Chinese++;
                    pre_position+=3;
                    cur_len-=2;
                }
                else{
                    cursor.English++;
                    pre_position++;
                    cur_len--;
                }
            }

            cursor.position=pre_position;
            cursor.line--;
        }
    }

    else if(dir==down){
        cur_line=cursor.line;
        if(cur_line!=lines){
            for(int i=1;i<=cur_line;i++){
                pre_node=cur_node;
                cur_node=cur_node->nextnode;
            }
            int cur_len=0;//算每行对应位置
            pre_position=1;
            cur_position=1;
            //找到当前行打印长度cur_len
            while(pre_position!=cursor.position){
                ch=(*pre_node)[pre_position-1];
                if(ch<0){//中文字符
                    cur_len+=2;
                    pre_position+=3;
                }
                else{
                    cur_len++;
                    pre_position++;
                }
            }
            //找出下一行对应位置cur_position
            while(cur_len>0&&(cur_position<=cur_node->length)){
                ch=(*cur_node)[cur_position-1];
                if(ch<0){//中文字符
                    cursor.Chinese++;
                    cur_position+=3;
                    cur_len-=2;
                }
                else{
                    cursor.English++;
                    cur_position++;
                    cur_len--;
                }
            }
            cursor.position=cur_position;
            cursor.line++;
        }
    }
    //std::cout<<"CE SIZE "<<cursor.Chinese<<" "<<cursor.English<<std::endl;
    //std::cout<<cursor.line<<" "<<cursor.position;
}

void Text::Insert_at_Cursor(std::string s){
    Insert(cursor.line,cursor.position,s);

    TextNode *cur_node=headnode;
    char ch;

    for(int i=1;i<cursor.line;i++){
        cur_node=cur_node->nextnode;
    }
    for(int i=0;i<s.length();i++){
        ch=s[i];

        if(ch=='\n'){
            cur_node=cur_node->nextnode;
            cursor.line++;
            cursor.position=1;
        }
        else if(ch<0){
            cursor.position+=3;
            i+=2;
        }
        else{
            cursor.position++;
        }
    }
    //计算中英文字符个数
    cursor.Chinese=0;
    cursor.English=0;
    int cur_position=1;
    while(cur_position<cursor.position){
        ch=(*cur_node)[cur_position-1];
        if(ch<0){//中文字符
            cursor.Chinese++;
            cur_position+=3;
        }
        else{
            cursor.English++;
            cur_position++;
        }
    }
}

void Text::MoveCursor_to_start(){
    cursor.position=1;
    cursor.Chinese=0;
    cursor.English=0;
}

void Text::MoveCursor_to_end(){
    TextNode *cur_node=headnode;
    char ch;
    cursor.Chinese=0;
    cursor.English=0;
    for(int i=1;i<cursor.line;i++){
        cur_node=cur_node->nextnode;
    }
    cursor.position=cur_node->length+1;
    int cur_position=1;
    while(cur_position<cursor.position){
        ch=(*cur_node)[cur_position-1];
        if(ch<0){//中文字符
            cursor.Chinese++;
            cur_position+=3;
        }
        else{
            cursor.English++;
            cur_position++;
        }
    }
}

/*int main()
{
    Text T;
    T.Text_Set("F:\\ccy.txt");
    T.Output();
    //T.Remove_BOM(buffer);
    //T.Output();
    T.Cursor_Set(2,31);
    T.MoveCursor(T.up);
    //T.Insert_at_Cursor("dad ad你好");
    //T.MoveCursor_to_end();
    //T.Save_File("G:\\gzl.txt");
    return 0;
}*/

