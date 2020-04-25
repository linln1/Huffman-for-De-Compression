#ifndef _DECOMPRESS_H_
#define _DECOMPRESS_H_

#include "Huffman.h"
#include<string>
#include<string.h>

void FileDecompress(string input, string output)
{	
	unsigned int freq[256];
	HTNode node[MAX_SIZE];
	unsigned long size_of_file;

	ifstream fin(input.c_str(), ios::binary);//从input中读取输入数据
	if (!fin)//如果打开文件失败 
	{
		cout << "fail to read" << input << endl;
		return ;
	}

	cout << "Running..." << endl;

	unsigned int character_Variety;//用于保存字符的种类
	ofstream fout(output.c_str(), ios::binary);//向output中写入数据
	fin.read((char*)&character_Variety, sizeof(unsigned int));//读取字符种类
	unsigned char alpha;
	if (character_Variety == 1)//只有一个字符意味着不需要压缩
	{	
		fin.read((char *)&alpha, sizeof(unsigned char));// 读取唯一的字符
		fin.read((char *)&size_of_file, sizeof(unsigned long)); // 读取文件长度
		while (size_of_file--)
			fout.write((char *)&alpha, sizeof(unsigned char));//
	}
	else
	{
		for (int i = 0; i<256; i++)
			freq[i] = 0;//注意传入的数组的各元素先赋值为0
		for (int i = 0; i<character_Variety; i++)
		{	
			fin.read((char*)&alpha, sizeof(unsigned char));//把当前字符从压缩文件中读取出来
			fin.read((char*)&size_of_file, sizeof(unsigned long));//把当前字符的频率从压缩文件中读取出来
			freq[alpha] = size_of_file; //保存频率
		}
		HuffmanTree(freq, node, character_Variety);//重建Huffman
		unsigned int root;
		root = (character_Variety - 1)<<1;//根结点在树数组中的下标
		fin.read((char *)&size_of_file, sizeof(unsigned long));	// 读入文件长度
		unsigned long wtNum = 0;
		while(true)
		{	
			fin.read((char *)&alpha, sizeof(unsigned char));// 读取一个字符长度的编码
			
			// 处理读取的一个字符长度的编码（通常为8位）
			for (int i = 0; i < 8; ++i)
			{
				// 由根向下直至叶节点正向匹配编码对应字符
				if ((alpha >> 7) & 1)//按位与，是压缩时按位或的逆过程，alpha字符对应的二进制的最高位是1,往右孩子方向走
					root = node[root].rchild;
				else
					root = node[root].lchild;

				if (root < character_Variety)//到达叶子结点
				{
					fout.write((char *)&node[root].ch, sizeof(unsigned char));
					++wtNum;//记录读取的文件长度
					if (wtNum == size_of_file) break;		// 控制文件长度，跳出内层循环
					root = (character_Variety - 1)<<1;        // 复位为根索引，匹配下一个字符
				}
				alpha <<= 1;		// 将编码缓存的下一位移到最高位，供匹配
			}
			if (wtNum == size_of_file) break;		// 控制文件长度，跳出外层循环
		}
	}
	fin.close();
	fout.close();
	cout << "Mission compelete!" << endl ;
}
#endif