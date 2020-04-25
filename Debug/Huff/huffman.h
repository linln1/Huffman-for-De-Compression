#ifndef _HUFFMAN_H_
#define _HUFFMAN_H_

#define MAX_FREQ  100000000 
#define MAX_SIZE  512

#include <algorithm>
#include "CharInfo.h"

//HTNode
typedef struct HTNode
{
	unsigned char ch;
	unsigned int freq;
	int parent;
	int lchild;
	int rchild;
}HTNode;

int HuffmanTree(unsigned int (&freq)[256],HTNode (&node)[MAX_SIZE],unsigned int n)
{                                                          //n表示freq数组中实际包含的字符种类数
    unsigned char c;
    int cnt = 0;

    for(int i=0;i<256;i++)//把前n个叶结点的信息输入数组
        if(freq[i])
        {
			node[cnt].ch =(unsigned char) i;
			node[cnt].freq = freq[i];
			node[cnt].parent = 0;
			node[cnt].lchild = 0;
			node[cnt].rchild = 0;
            cnt++;
        }
    for(int i=n;i<2*n-1;i++)//处理剩下n-1个非叶子结点
    {
		node[i].ch = '#';//假设这些结构里面初始的符号都是‘#’
		node[i].freq = 0;//频率都是0
		node[i].parent = 0;
		node[i].lchild = 0;
		node[i].rchild = 0;
    }
    // 循环构造 Huffman 树
    for(int i=0; i<n-1; i++)
    {
        unsigned int idx1=MAX_FREQ;
        unsigned int idx2=MAX_FREQ; // idx1、idx2中存放两个无父结点且结点权值最小的两个结点
        int cur1=0 ; 
        int cur2=0;  //cur1、cur2：构造哈夫曼树不同过程中两个最小权值结点在数组中的序号
        /* 找出所有结点中权值最小、无父结点的两个结点，并合并之为一颗二叉树 */
        for (int j=0; j<n+i; j++)
        {
            if (node[j].freq < idx1 && node[j].parent==0)
            {            //如果当前判断的结点的权值小于最小的idx1，则把它赋给idx1，同时
                idx2=idx1;   //更新idx1结点的下标， 保持idx1是当前所有判断过的元素中是最小的
                cur2=cur1;   //再把idx1的信息赋给idx2，保持idx2是当前所有判断过的元素中是第二小的
                idx1= node[j].freq ;
                cur1=j;
            }
            else if (node[j].freq  < idx2 && node[j].parent==0)
                         //如果当前判断的结点的权值大于等于最小的idx1，但是小于idx2，
            {            //则只需把它赋给idx2，更新idx2,保持idx2是当前所有判断过的元素中是第二小的
                idx2= node[j].freq ;
                cur2=j;
            }
        }
        /* 设置找到的两个子结点 cur1、cur2 的父结点信息 */
		node[cur1].parent  = n+i;
		node[cur2].parent  = n+i;
		node[n+i].freq  = node[cur1].freq  + node[cur2].freq ;
		node[n+i].lchild = cur1;
		node[n+i].rchild = cur2;
    }
    return 0;
}
//哈夫曼编码,输出string中各种字符对应的编码
int Encoding(HTNode(&node)[MAX_SIZE],ALPHA_FREQ (&alphafreq)[256],unsigned int n)
{
    int temp;
    for(int i = 0;i < n;i++)
    {
        temp = i;//当前处理的数组下标
        alphafreq[i].ch = node[i].ch;
        while(node[temp].parent)
        {
            if(node[node[temp].parent].lchild == temp)//左孩子为0
            {
                 alphafreq[i].code += '0';
            }
            else//右孩子为1
            {
                alphafreq[i].code += '1';
            }
            temp = node[temp].parent;
        }
        reverse(alphafreq[i].code.begin(), alphafreq[i].code.end());
    }  
    return 0;
}


#endif
