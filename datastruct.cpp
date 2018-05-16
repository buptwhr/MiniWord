#include "datastruct.h"

void TextNode::Input(std:: string s){
	TextBlock *cur = first;
    for (int i = 0; i < s.length(); i = i + SIZE) {
		if (!cur) {
			Extend();
			cur = end;
		}
        for (int j = i; j < i + SIZE && j < s.length(); ++j) {
			cur->text[j - i] = s[j];
		}
		cur = cur->next_block;
	}
	length = s.length();
}
void TextNode::Extend(){
	if (!first) {
		first = new TextBlock;
		end = first;
        size += SIZE;
		return;
	}
    size += SIZE;
	end->next_block = new TextBlock;
	end = end->next_block;
}
void TextNode::Renew(){
	if ((length - 1) / SIZE + 1 == size / SIZE)
		return;
	TextBlock *cur = first;
	for (int i = 0; i < (length-1)/SIZE; ++i) {
		cur = cur->next_block;
	}
    end = cur;
	TextBlock *next = cur->next_block;
    cur->next_block = NULL;
    cur = next;
	while (cur) {
        next = cur->next_block;
		delete cur;
        cur = next;

	}
	size = ((length - 1) / SIZE + 1)*SIZE;
}
int TextNode::Index(std::string string_aim, int position){
       int j=1;   int k=0;
	   int* next = new int[string_aim.length()];
       next[0]=0;
       while( j<string_aim.length() ) {
            if  ( k==0 || string_aim[j-1]==string_aim[k-1] )
                   { ++j;   ++k;   next[j-1]=k;  }
              else    k= next[k-1];
         }
        int i=position; j=1;
            while  (i <= length && j <= string_aim.length()){
                  if  ( j==0   ||  (*this)[i-1]==string_aim[j-1])
                        {  i++;   j++; }
                  else  j=next[j-1];
             }
             if   (j>string_aim.length())  return  i-string_aim.length();
            else
            return 0;
}
///////////////////////Text 成员

void Text::Text_Set(std::string Filename){
    file.close();
    file.open(Filename, std::fstream::in);
    if(!file){
        file.open(Filename, std::ios_base::out);
        headnode = new TextNode;
        tailnode = headnode;
    }
    else{ std::string s;
        while(std::getline(file, s)){
            if(!headnode){
                headnode = new TextNode;
                tailnode = headnode;}
            else{
                tailnode->nextnode = new TextNode;
                tailnode = tailnode->nextnode;
            }
            tailnode->Input(s);
            ++lines;
        }

    }
}

void Text::Output(){
    TextNode *curnode = headnode;
    while(curnode !=NULL){
        for(int i = 0; i<curnode->length; ++i){
            std::cout<<(*curnode)[i];
        }
        std::cout<< std::endl;
        curnode = curnode->nextnode;
    }
    std::cout << std::endl;
}
bool Text::Insert(int line, int position, std::string s){
    if(line<=lines){
        TextNode *curnode = headnode;
        for(int i=1; i<line; ++i)
            curnode = curnode->nextnode;
        if(position<=curnode->length+1){
            for(int i=0; i<s.length(); ++i){
                if(s[i]=='\n'){
                    TextNode *node_new = new TextNode;
                    node_new->nextnode = curnode->nextnode;
                    curnode->nextnode = node_new;
                    int k = 0;
                    int j = position-1;
                    while(j < curnode->length){
                        if(node_new->length == node_new->size) node_new->Extend();
                        (*node_new)[k++] = (*curnode)[j++];
                        ++node_new->length;
                    }
                    curnode->length = position-1;
                    curnode->Renew();
                    if(curnode == tailnode)
                        tailnode = node_new;
                    curnode = node_new;
                    position = 1;
                    ++lines;
                }
                else{
                    if(curnode->length == curnode->size) curnode->Extend();
                    for(int j = curnode->length; j >= position; --j)
                        (*curnode)[j] = (*curnode)[j-1];
                   (*curnode)[position-1] = s[i];
                    ++position;
                    ++curnode->length;
                }
            }
            return 1;
        }
    }
    return 0;
}

bool Text::Delete(int line, int position){
    if(line<=lines){
        if(position == 0 && line != 1){
            TextNode *prenode = headnode;
            TextNode *curnode = headnode->nextnode;
            for(int i = 2; i < line ; ++i){
                prenode = curnode;
                curnode = curnode->nextnode;
            }
            int i = prenode->length;
            int j = 0;
            while(j < curnode->length){
                if(prenode->length == prenode->size) prenode->Extend();
                (*prenode)[i++] = (*curnode)[j++];
                ++prenode->length;
            }
            prenode->nextnode = curnode->nextnode;
            if(tailnode == curnode) tailnode = prenode;
            --lines;
            delete curnode;
            return 1;
        }
        TextNode *curnode = headnode;
        for(int i = 1; i < line; ++i)
            curnode = curnode->nextnode;
        if(position > 0 && position <= curnode->length){
            for(int i = position-1; i < curnode->length-1; ++i){
                (*curnode)[i] = (*curnode)[i+1];
            }
            --curnode->length;
            curnode->Renew();
            return 1;
        }
    }
    return 0;
}

bool Text::Index(std::string string_aim, int line, int position, int &aimline, int &aimposition){
    if(line <= lines){
        aimposition = 0;
        aimline = line;
        TextNode *curnode = headnode;
        for(int i = 1; i < line ; ++i)
            curnode = curnode->nextnode;
        if(position > 0 && position <= curnode->length){
            aimposition = curnode->Index(string_aim, position);
            if(aimposition)
                   return 1;
            ++aimline;
            curnode = curnode->nextnode;
            while(aimline <= lines){
                aimposition = curnode->Index(string_aim,1);
                if(aimposition) break;
                ++aimline;
                curnode = curnode->nextnode;
            }
            return aimposition;
        }
    }
    return 0;
}

bool Text::Replace(std::string string_aim, std::string string_replace, int position, int line){
    int aimline, aimposition;
    if(Index(string_aim,line,position,aimline,aimposition)){
        int l = string_aim.length();
        for(int i = 0; i<l;++i)
            Delete(aimline, aimposition);
        Insert(aimline,aimposition,string_replace);
        return 1;
    }
    return 0 ;
}



















