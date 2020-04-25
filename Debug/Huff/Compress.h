#ifndef _COMPRESS_H_
#define _COMPRESS_H_

#include<string>
#include<string.h>
#include<algorithm>
#include"Huffman.h"
#include"CharInfo.h"

void Filecompress(string input, string output)
{
	unsigned long size_of_file = 0;
	unsigned int freq[256];
	HTNode node[MAX_SIZE];
	ALPHA_FREQ alphafreq[256];

	ifstream fin(input.c_str(), ios::binary);//从input中读取输入数据
	
	if (!fin)
	{
		cout << "fail to read " << input << endl;
		return;
	}

	ofstream fout(output.c_str(), ios::binary);//向output中写入数据

	cout << "Running..." << endl;

	for (int i = 0; i<256; i++)
		freq[i] = 0;//注意传入的数组的各元素先赋值为0


	unsigned char c;
	while(true)
	{
		c = fin.get();//每次以二进制的形式读取一个byte，并把对应的字符存入c中去
		if (fin.eof()) break;//读到二进制文件末尾，不再读取
		freq[c]++;//字符c的频率加一
		size_of_file++; //记录文件的长度，即文件包含多少字节
	}
	fin.close();


	unsigned int character_Variety = 0;//用以保存string中所包含的字符种类
	for (int i = 0; i<256; i++)//计算character_Variety值
		if (freq[i])
			character_Variety++;


	ALPHA_FREQ *temp = (ALPHA_FREQ *)malloc(character_Variety * sizeof(ALPHA_FREQ));//分配一个实际出现字符种类大小的数组，保存实际出现的字符
	int cnt = 0;//
	for (int i = 0; i < 256; ++i)
	{
		if (freq[i])
		{
			temp[cnt].ch = i;
			temp[cnt].freq = freq[i];
			cnt++;
		}
	}


	if (character_Variety == 1)//只有一种字符就不用压缩
	{
		fout.write((char*)&character_Variety, sizeof(unsigned int));//把出现的字符种类写入压缩文件
		fout.write((char*)&temp[0].ch, sizeof(unsigned char));//把出现的唯一字符写入压缩文件
		fout.write((char*)&temp[0].freq, sizeof(unsigned long));//把出现的唯一字符的频率写入压缩文件
		free(temp);
	}
	else
	{
		HuffmanTree(freq, node, character_Variety);
		Encoding(node, alphafreq, character_Variety);
		fout.write((char*)&character_Variety, sizeof(unsigned int));//把出现的字符种类写入压缩文件
		for (int i = 0; i<character_Variety; i++)
		{
			fout.write((char*)&temp[i].ch, sizeof(unsigned char));//把出现的字符写入压缩文件
			fout.write((char*)&temp[i].freq, sizeof(unsigned long));//把出现的字符的频率写入压缩文件
		}
		fout.write((char *)&size_of_file, sizeof(unsigned long));		// 写入文件长度
		free(temp);

		ifstream fin2(input.c_str(), ios::binary);//从input中再次读取输入数据
		string buf;// 待存编码缓冲区
		unsigned char beta;
		while(true)
		{
			c = fin2.get();
			if (fin2.eof()) break;
			beta = c;// 每次读取1byte,作为一个字符
			for (int i = 0; i<character_Variety; i++)
			{
				if (beta == alphafreq[i].ch)//如果字符相等
				{
					buf += alphafreq[i].code;//缓冲区载入编码
					break;//匹配上了就跳出
				}		
			}
			while (buf.size() >= 8)
			{
				beta = '\0';		// 清空字符暂存空间，改为暂存字符对应编码
				for (auto iter = buf.begin(); iter<buf.begin() + 8; iter++)
				{
					beta <<= 1;		// 左移一位，为下一个bit腾出位置
					if (*iter == '1')    //如果编码为1
						beta |= 1;		// 当编码为"1"，通过 位或 操作符将其添加到字节的最低位
				}
				fout.write((char *)&beta, sizeof(unsigned char));		// 将字节对应编码存入文件
				buf = buf.substr(8);// 编码缓存去除已处理的前八位
			}
		}
		// 处理最后不足1byte编码
		if (buf.size() > 0)
		{
			beta = '\0';
			for (auto iter = buf.begin(); iter != buf.end(); iter++)
			{
				beta <<= 1;
				if (*iter == '1')
					beta |= 1;
			}
			beta <<= 8 - buf.size();       // 将编码字段从尾部移到字节的高位
			fout.write((char *)&beta, sizeof(unsigned char));       // 存入最后一个字节
		}
		fin2.close();
	}
	fout.close();


	ifstream fin3(output.c_str(), ios::binary);
	unsigned long size_of_file2 = 0;
	while(true)
	{
		c = fin3.get();//每次以二进制的形式读取1byte，并把对应的字符存入c中去
		if (fin3.eof()) break;//读到二进制文件末尾，不再读取
		size_of_file2++; //记录文件的长度，即文件包含多少字节
	}
	fin3.close();
	float compress_rate = float(size_of_file2) / float(size_of_file);//压缩率
	cout << "Compress Success!" << endl << endl;
	cout << "Compress rate is " << compress_rate << endl << endl;//输出压缩率
}
#endif
