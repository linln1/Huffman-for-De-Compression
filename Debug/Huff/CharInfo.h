#pragma once

#ifndef _CHARINFO_H_
#define _CHARINFO_H_

#include<string>
#include<string.h>

using namespace std;


typedef struct ALPHA_FREQ
{
    unsigned char ch;//保存字符
    unsigned int freq;
    string code;//保存字符对应的编码
}ALPHA_FREQ;



#endif