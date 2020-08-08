#include "tinystr.h"
#include "tinyxml.h"
#include "common.h"
#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include<Windows.h>
#include<tchar.h>
using namespace std;

#define XMLFilePath "C:\\Users\\liuquanchang\\Desktop\\new.xml"
bool  ReadXmlInfo(wstring& fileWPath, vector<pair<wstring, wstring>>& xmlInfo);
//string转换为wstring
std::wstring stringToWstring(const std::string& str);
//更改string编码
std::string UTF8ToGB(const char* str);
//const char* -> wstring
std::wstring charToWstring(const char* ch);
bool UnicodeToUtf8(std::wstring& unicode_string, std::string& utf8_string);

bool  ReadXmlInfo(wstring& fileWPath, vector<pair<wstring, wstring>>& xmlInfo)
{
    TiXmlDocument doc;//申明一个文档类型变量，用来存储读取的xml文档
    string filePath;
    if (!UnicodeToUtf8(fileWPath, filePath))
    {
        return false;
    }

    if (!doc.LoadFile(filePath.c_str()))  //检测xml文档是否存在
    {
        return false;
    }

    TiXmlElement* MassSpectrumLab = doc.FirstChildElement();//指向xml文档的根元素

    if (MassSpectrumLab == NULL)//检测根元素存在性     
    {
        doc.Clear();
        return false;
    }
    else
    {
        TiXmlElement* term = MassSpectrumLab->FirstChildElement(); // xml的id 根节点下第一个子节点
        while (term != NULL)
        {
            wstring contactType = stringToWstring(UTF8ToGB(term->Value()));
            wstring contactValue = stringToWstring(UTF8ToGB(term->GetText()));
            xmlInfo.push_back(pair<wstring, wstring>(contactType, contactValue));
            term = term->NextSiblingElement();
        }
    }
    return true;
}


bool UnicodeToUtf8(std::wstring& unicode_string, std::string& utf8_string)
{
    utf8_string = "";
    if (_wcsicmp(unicode_string.c_str(), L"") == 0)
    {
        return false;
    }

    DWORD utf8_string_len = WideCharToMultiByte(CP_ACP, NULL, unicode_string.c_str(), -1, NULL, 0, NULL, FALSE);// WideCharToMultiByte的运用
    if (0 == utf8_string_len)
    {
        return false;
    }
    char* temp_utf8_string = new char[utf8_string_len + 1];
    memset(temp_utf8_string, 0, sizeof(char) * (utf8_string_len + 1));
    if (0 == WideCharToMultiByte(CP_ACP, NULL, unicode_string.c_str(), -1, temp_utf8_string, utf8_string_len, NULL, FALSE))
    {
        delete[] temp_utf8_string;
        temp_utf8_string = NULL;
        return false;
    }

    utf8_string = (std::string)temp_utf8_string;
    delete[] temp_utf8_string;
    temp_utf8_string = NULL;

    return true;
}

std::wstring stringToWstring(const std::string& str)
{
    std::wstring result;

    //获取缓冲区大小，并申请空间，缓冲区大小按字符计算  
    int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), NULL, 0);
    TCHAR* buffer = new TCHAR[len + 1];

    //多字节编码转换成宽字节编码  
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), buffer, len);

    //添加字符串结尾  
    buffer[len] = '\0';

    //删除缓冲区并返回值  
    result.append(buffer);
    delete[] buffer;
    return result;
}


std::string UTF8ToGB(const char* str)
{
    std::string result;
    WCHAR* strSrc;
    LPSTR szRes;

    //获得临时变量的大小
    int i = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
    strSrc = new WCHAR[i + 1];
    MultiByteToWideChar(CP_UTF8, 0, str, -1, strSrc, i);

    //获得临时变量的大小
    i = WideCharToMultiByte(CP_ACP, 0, strSrc, -1, NULL, 0, NULL, NULL);
    szRes = new CHAR[i + 1];
    WideCharToMultiByte(CP_ACP, 0, strSrc, -1, szRes, i, NULL, NULL);

    result = szRes;
    delete[]strSrc;
    delete[]szRes;

    return result;
}