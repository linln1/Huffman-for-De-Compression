
#include <cstdio>
#include <iostream>
#include <fstream>

#include <string>
#include <string.h>

#include "./CharInfo.h"
#include "./Compress.h"
#include "./Decompress.h"
#include "./Huffman.h"

using namespace std;

//用一个配置文件来记录原文件字符出现的总次数和每个字符分别出现的次数，这样在解压缩的时候直接去配置文件中读取需要的信息即可

// 读入文件
// 统计字符次数
// 构建Huffman_Tree
// 再次读入文件 按照对应的Huffman_Code压缩文件
// 将字符的总次数和各个字符出现的次数写入到压缩文件里面
// 测试压缩系数比
// 计算压缩和解压分别对应的时间

// 对一个文件操作之后总共产生3个文件
// 原文件 xxx
// 原文件的压缩文件  xxx.huffman
// 原文件的解压缩文件  xxx.unhuffman

// ASCII 码编码范围是 [32,126]
// 顶多一次读入8个bits 所以最多有 (1<<8) = 256 种字符,所以一开始可以先分配一个256大小的HuffmanNode 数组
// 打开文件 读出字符，字符对应下标的count++
// 对HuffNode数组建立Huffman树。每次选两次最小值，添加一个权重为两者频率之和的点进去，注意数组的大小
// 哈夫曼树 如果结点的左右子树都为空，那他就是叶子节点，也就是说它代表的是一个字符
// 调用Compress函数时，利用字符频率的不同，将fin 转化后写入fout
// 最大频率,单个字符出现的次数不能超过这个数
// 编码结点


int main()
{
	int menu;
	cout<<"This is an application of FileCompress & FileDecompress"<<endl;
	cout<<"If you want to Compress Files, please input 1"<<endl;
	cout<<"If you want to Decompress Files, please input 2"<<endl;
	cout<<"If you want to Exit, please input 3"<<endl;
	cin >> menu;
	cout << endl;

	string input_file,output_file;
	size_t size_of_file;

	if(menu != 3){
		cout << "Please input the file's name:" << endl;
        cin>>input_file;
        FILE *fin = fopen(input_file.c_str(),"rb");
        if(fin == NULL){
            cout<<"Fail to open "<<input_file<<" , please check whether it is valid\n";
        }

		cout << "Please input destination:" << endl;
		cin >> output_file;

        long begin_idx,end_idx;//存放文件开始处的索引以及文件末尾的索引
        begin_idx = ftell(fin);//将文件的读指针的索引存到begin_idx 
        fseek(fin,0L,SEEK_END);//将指针放在文件结束的时候
        end_idx = ftell(fin);//将文件的读指针的索引存到end_idx
        size_of_file= end_idx - begin_idx;//算出该二进制文件的大小
        cout<<"The size of "<<input_file<<" is "<<size_of_file<<" bytes.\n";
        fclose(fin);
    }

	switch (menu)
	{
		case 1: 
			Filecompress(input_file, output_file); 
			break;
		case 2: 
			FileDecompress(input_file, output_file); 
			break;
		case 3: 
			exit(0);
			break;
		default: 
			system("cls");
			break;
	}
	return 0;
}