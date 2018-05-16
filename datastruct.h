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
private:
    int line_begin;//起始行
    int line_end;//结束行
    int position_begin;//起始行位置
    int position_end;//结束行位置
    std::string block_string;//此块所对应的字符串
};




class Text{//代表文本的主数据结构
public:
    friend class MainWindow;
    Text():block_text(){

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
    void InputFilename();
    void New_File();
    void Save_File(bool Name_Choice);
    void Quit_File();
    void Text_Set(std::string Filename);//根据提供文件名创建此类
    bool Insert(int line, int position, std::string s);//向文本中某行某位置插入字符串
    bool Delete(int line, int position);//删除某行某位置的一个字符
    bool Index(std::string string_aim, int line, int position, int &aimline, int &aimposition);//从某行某位置后面开始匹配目标字符串，若成功则返回字串的头位置。
    bool Replace(std::string string_aim, std::string string_replace, int position, int line);//从某行某位置后面开始匹配目标字符串并把其用另一个串代替
    bool Block_Set(int line_begin, int line_end, int position_begin, int positon_end);//设置块
    bool Block_Delete();//删除块
    bool Block_Copy();//复制块
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
        lines = 1 ;
        headnode = new TextNode;
        tailnode = headnode;
    }
private:
    std::fstream file;//文件
    std::string filename;//当前文件名
    TextNode* headnode;//头结点
    TextNode* tailnode;//尾结点
    int lines;//行数
    Block block_text;//块
};

