/*
************************************************************************************
* CopyRight (C) WangYu
*
* FileName     : config.h
* Author       :
* Description  :
* Function List:
* History      :
************************************************************************************
*/
#ifndef CONFIG_H
#define CONFIG_H

#include "lib_wfc_test.h"

#include <assert.h>
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

class CCfgItem
{
public:
    CCfgItem(){ Reset(); }
    CCfgItem(const CCfgItem &rValue)
    {
        iLine = rValue.iLine;
        iPos  = rValue.iPos;

        strItemName  = rValue.strItemName;
        strItemValue = rValue.strItemValue;
        strOldValue  = rValue.strOldValue;
    }

    void Reset()
    {
        iLine = 0;
        iPos  = 0;
        strItemName  = "";
        strItemValue = "";
        strOldValue  = "";
    }

public:
    int iLine;                          //配置项所在的源文件的行
    int iPos;                           //配置项所在的源文件的列

    string strItemName;                 //配置项名
    string strItemValue;                //配置项的值
    string strOldValue;                 //配置项更新之前的值
};

class CCfgField
{
public:
    CCfgField(){}

    CCfgField(const CCfgField &rValue)
    {
        iLine = rValue.iLine;

        strFieldName  = rValue.strFieldName;
        vecItems      = rValue.vecItems;
    }

    void Reset()
    {
        iLine = 0;
        strFieldName = "";
        vecItems.clear();
    }

public:
    int iLine;                          //域所在的行

    string           strFieldName;      //域名
    vector<CCfgItem> vecItems;          //配置项值
};

class CConfig
{
public:
    CConfig();
    ~CConfig(){}

    //初始化, 读取配置文件的内容
    int Init(const char* pszFile);

    //获取配置文件中所有域的列表
    int GetAllFields(vector<string> &vecOut);

    //获取指定域下的所有配置项的列表
    int GetAllItems(const char* pszField, vector<string> &vecOut);

    //获取某个配置域下的某个配置项的值
    int GetItem(const char* pszField, const char* pszItem, char* pszOut);

    //设置指定配置项的值
    int SetItem(const char* pszField, const char* pszItem, const char* pszValue);

    //将内存中的内容刷新到文件
    int RefreshFile();

    //重新加载内存信息;
    int ReloadCfgFile();

    //打印配置文件信息
    int Print(const char* pszField = NULL, const char* pszItem = NULL);

private:
    //解析文件中的内容
    int Parse();

    //判断指定的配置域是否存在
    int IsFieldExist(const char* pszField);

    //判断指定的配置项在当前配置域下是否存在
    int IsItemExist(const char* pszItem);

    //去掉多余的空格和Tab键
    void TrimLine(string &strLine);

    //去掉行中的注释
    void RemoveComment(string &strLine);

private:
    int    m_iIsInitOK;
    string m_strCfgFile;

    vector<string>    m_vecContent;
    vector<CCfgField> m_vecFields;
};

#endif /*CONFIG_H*/

/********************************** END *******************************************/

