#include <iostream>
#include<fstream>
#include<string>
#include<exception>
#define SIZE 100
#define state_initial 0
#define state_editing 1
#define state_saved 2
class TextBlock {//一行的最小单位，长度为100的块
private:
	char* text;
	TextBlock *next_block;
public:
	friend class TextNode;
    friend class MainWindow;
	TextBlock() {
		text = new char[SIZE];
		next_block = NULL;
	}
	~TextBlock() {
		delete[] text;
		next_block = NULL;
	}
};

class TextNode{//指示一行的结点
private:
	TextNode * nextnode;//下一行对应结点
	TextBlock * first;
	TextBlock * end;
	int size;//当前容量
	int length;//当前长度
public:
    friend class Text;
    friend class MainWindow;
    TextNode():size(0),length(0){
        nextnode = NULL;
		first = NULL;
		end = NULL;
    }
    ~TextNode(){
		TextBlock *cur, *next;
		cur = first;
		while (cur) {
			next = cur->next_block;
			delete cur;
			cur = next;
		}
        nextnode = NULL;
    }
    int Index(std::string string_aim, int position);
    void Input(std::string s);//用字符串s作为此行内容
    void Extend();//扩充容量
    void Renew();//重分配容量
	//void Insert(std::string::const_iterator begin, std::string::const_iterator end, int position);
    char& operator [](int i) {
        if (i < size) {
			int j = 0;
			TextBlock *cur = first;
			while (j < i / SIZE) {
				cur = cur->next_block;
				++j;
			}
            return cur->text[i - j *SIZE];
		}
	}
};








class Block{//指示文本中一块内容的类
public: friend class Text;
    Block(){
        line_begin = line_end = position_begin = position_end = 0;
    }
    bool Block_Set(int y1, int y2, int x1 , int x2, std::string &s){
        line_begin = y1;
        line_end = y2;
        position_begin = x1;
        position_end = x2;
        block_string = s;
        return well_set();
    }
    void Reset(){
        line_begin = line_end = position_begin = position_end = 0;
        block_string = "";
    }

    bool well_set(){
        if(line_begin == line_end && position_begin == position_end)
            return 0;
        return 1;
    }
private:
    int line_begin;//起始行
    int line_end;//结束行
    int position_begin;//起始行位置
    int position_end;//结束行位置
    std::string block_string;//此块所对应的字符串
};









class Cursor{
private:
    int line;
    int position;
    int English;
    int Chinese;
public:
    friend class Text;
    friend class MainWindow;
    Cursor():line(1), position(1), English(0), Chinese(0){}
    Cursor(int l, int p): line(l), position(p), English(0), Chinese(0){}
};







class Text{//代表文本的主数据结构
public:
    enum direction {left,right,up,down};
    friend class MainWindow;
    Text():block(), cursor(1,1){
       headnode = NULL;
       tailnode = NULL;
       lines = 0;
    }
    Text(std::string Filename) {
       this->Text_Set(Filename);
    }
    ~Text(){//析构函数
        TextNode *curnode = headnode;
        while(curnode){
            TextNode *next = curnode->nextnode;
            delete curnode;
            curnode = next;
        }
        file.close();
    }
    void Remove_BOM(char *buffer);
    void Add_BOM(char *buffer);
    void Cursor_Set(int l,int p){cursor=Cursor(l,p);};//测试用，删
    void InputFilename();
    void New_File(std::string File_Name);
    void Save_File(std::string New_Name);
	void Quit_File();
    bool Text_Set(std::string Filename);//根据提供文件名创建此类
    bool Replace(std::string string_aim, std::string string_replace, int position, int line);//从某行某位置后面开始匹配目标字符串并把其用另一个串代替
    bool Index(std::string string_aim, int line, int position, int &aimline, int &aimposition);//从某行某位置后面开始匹配目标字符串，若成功则返回字串的头位置。
    void MoveCursor(direction dir);
    void Insert_at_Cursor(std::string s);
    void Delete_at_Cursor(direction dir);
    void MoveCursor_to_start();
    void MoveCursor_to_end();
    void Count_CE();
    void Output();//输出文本全部内容
    void Clear(){//清除数据
        TextNode *curnode = headnode;
        while(curnode){
            TextNode *next = curnode->nextnode;
            delete curnode;
            curnode = next;
        }
        file.close();
        filename ="";
        lines = 0;
        headnode = NULL;
        tailnode = headnode;
    }
    int Get_Max_Length(){
        int max_length = 0;
        TextNode *cur = headnode;
        while(cur){
            max_length = max_length > cur->length ? max_length : cur->length;
            cur = cur->nextnode;
        }
        return max_length;
    }
    bool Block_Set(int line_begin, int position_begin, int line_end,  int positon_end);//设置块
    void Block_Delete(int line_begin, int position_begin, int line_end,  int positon_end);//删除块
private:      
    std::fstream file;//文件
    std::string filename;//当前文件名
    TextNode* headnode;//头结点
    TextNode* tailnode;//尾结点
    int lines;//行数
    Block block;//块
    Cursor cursor;
    bool Insert(int line, int position, std::string s);//向文本中某行某位置插入字符串
    bool Delete(int line, int position);//删除某行某位置的一个字符

};
