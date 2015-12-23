/*
************************************************************************************
* CopyRight (C) WangYu
*
* FileName     : config.cpp
* Author       :
* Description  :
* Function List:
* History      :
************************************************************************************
*/

#include "lib_config.h"

/****************************************************************
* Function Name : CConfig();
* Description   : 构造函数, 初始化成员变量;
* Calls         : NA;
* Called By     : NA;
* Input         : NA;
* Output        : NA;
* Return        : NA;
* Table Accessed: NA;
* Table Updated : NA;
* Other         : NA;
****************************************************************/
CConfig::CConfig()
{
    m_iIsInitOK  = 0;
    m_strCfgFile = "";
}

/****************************************************************
* Function Name : Init();
* Description   : 读取文件, 解析文件内容到内存变量;
* Calls         : Parse();
* Called By     : ReloadCfgFile();
* Input         : pszFile: 配置文件名;
* Output        : NA;
* Return        : 0: 成功; 1: 失败;
* Table Accessed: NA;
* Table Updated : NA;
* Other         : NA;
****************************************************************/
int CConfig::Init(const char* pszFile)
{
    assert(pszFile);

    m_strCfgFile = pszFile;

    ifstream ifs;
    ifs.open(pszFile);
    if(!ifs)
    {
        PrintErr("Unable to open the configuration file: " << pszFile);
        return 1;
    }

    string strOneLine;

    //读取文件的内容到内存
    m_vecContent.clear();
    while(getline(ifs,strOneLine))
    {
        m_vecContent.push_back(strOneLine);
    }
    ifs.close();

    if(0 == Parse())
    {
        m_iIsInitOK = 1;
        return 0;
    }
    else
    {
        m_iIsInitOK = 0;
        return 1;
    }
}

/****************************************************************
* Function Name : Parse();
* Description   : 解析文件内容到内存变量;
* Calls         : RemoveComment(), TrimLine(), IsFieldExist(), IsItemExist();
* Called By     : Init();
* Input         : NA;
* Output        : NA;
* Return        : 0: 成功; 1: 失败;
* Table Accessed: NA;
* Table Updated : NA;
* Other         : NA;
****************************************************************/
int CConfig::Parse()
{
    string strOne;
    string strFName;
    string strFValue;

    int    iPos = 0;
    int    iTotalFields = 0;

    m_vecFields.clear();

    for(int i=0; i<m_vecContent.size(); ++i)
    {
        strOne = m_vecContent[i];
        RemoveComment(strOne);
        TrimLine(strOne);
        if(strOne.empty())
        {
            continue;
        }

        //为一个域
        if('[' == strOne[0])
        {
            if(']' != strOne[strOne.size()-1])
            {
                PrintErr("Invalid configuration fomat [" << m_vecContent[i] << "] at line: " << i + 1);
                return 1;
            }

            if(2 == strOne.size())
            {
                PrintErr("Empty field name at line: " << i + 1);
                return 1;
            }

            strOne = strOne.substr(1, strOne.size() - 2);
            if(IsFieldExist(strOne.c_str()))
            {
                PrintErr("The field [" << strOne << "] is already existed at line: " << i + 1);
                return 1;
            }

            CCfgField tmpField;
            tmpField.iLine = i;
            tmpField.strFieldName = strOne;
            m_vecFields.push_back(tmpField);
        }
        else
        {
            iPos = strOne.find("=");
            if(string::npos == iPos)
            {
                PrintErr("Invalid configuration fomat [" << m_vecContent[i] << "] at line: " << i + 1);
                return 1;
            }

            strFName  = strOne.substr(0, iPos);
            strFValue = strOne.substr(iPos + 1, strOne.size() - iPos);
            TrimLine(strFName);
            TrimLine(strFValue);
            if(strFName.empty())
            {
                PrintErr("Empty item name at line: " << i + 1);
                return 1;
            }

            iTotalFields = m_vecFields.size();
            if(0 == iTotalFields)
            {
                PrintErr("The item must be under a field at line: " << i + 1);
                return 1;
            }

            if(IsItemExist(strFName.c_str()))
            {
                PrintErr("The item [" << strFName << "] is already existed under the field ["
                         << m_vecFields[iTotalFields-1].strFieldName << "] at line: " << i + 1);
                return 1;
            }

            CCfgItem tmpItem;
            tmpItem.iLine = i;
            if(strFValue.empty())
            {
                tmpItem.iPos = iPos + 1;
            }
            else
            {
                tmpItem.iPos  = m_vecContent[i].find(strFValue, iPos + 1);
            }
            tmpItem.strItemName  = strFName;
            tmpItem.strItemValue = strFValue;
            tmpItem.strOldValue  = strFValue;

            m_vecFields[iTotalFields - 1].vecItems.push_back(tmpItem);
        }
    }

    return 0;
}

/****************************************************************
* Function Name : IsFieldExist();
* Description   : 判断指定的域是否已经存在;
* Calls         : NA;
* Called By     : Parse();
* Input         : pszField: 需要检测的域名;
* Output        : NA;
* Return        : 0: 不存在; 1: 存在;
* Table Accessed: NA;
* Table Updated : NA;
* Other         : NA;
****************************************************************/
int CConfig::IsFieldExist(const char* pszField)
{
    assert(pszField);

    for(int i=0; i<m_vecFields.size(); ++i)
    {
        if(pszField == m_vecFields[i].strFieldName)
        {
            return 1;
        }
    }

    return 0;
}

/****************************************************************
* Function Name : IsItemExist();
* Description   : 判断指定的配置项是否在当前的域中已经存在;
* Calls         : NA;
* Called By     : Parse();
* Input         : pszItem: 需要检测的配置项名;
* Output        : NA;
* Return        : 0: 不存在; 1: 存在;
* Table Accessed: NA;
* Table Updated : NA;
* Other         : NA;
****************************************************************/
int CConfig::IsItemExist(const char* pszItem)
{
    int iPos = m_vecFields.size() - 1;
    for(int i=0; i<m_vecFields[iPos].vecItems.size(); ++i)
    {
        if(pszItem == m_vecFields[iPos].vecItems[i].strItemName)
        {
            return 1;
        }
    }

    return 0;
}

/****************************************************************
* Function Name : TrimLine();
* Description   : 删除输入字符串首尾空格和Tab键;
* Calls         : NA;
* Called By     : Parse();
* Input         : strLine: 需要进行操作的字符串;
* Output        : strLine: 输出结果;
* Return        : NA;
* Table Accessed: NA;
* Table Updated : NA;
* Other         : NA;
****************************************************************/
void CConfig::TrimLine(string &strLine)
{
    int iStart = 0;
    int iEnd   = strLine.size() - 1;
    if(iEnd < iStart)
    {
        return;
    }

    while(iStart <= iEnd)
    {
        if(' ' == strLine[iStart] || '\t' == strLine[iStart])
        {
            ++iStart;
        }
        else
        {
            break;
        }
    }

    while(iEnd > iStart)
    {
        if(' ' == strLine[iEnd] || '\t' == strLine[iEnd])
        {
            --iEnd;
        }
        else
        {
            break;
        }
    }

    strLine = strLine.substr(iStart, iEnd - iStart + 1);
}

/****************************************************************
* Function Name : RemoveComment();
* Description   : 删除输入串中的注释信息;
* Calls         : NA;
* Called By     : Parse();
* Input         : strLine: 需要进行操作的字符串;
* Output        : strLine: 输出结果;
* Return        : NA;
* Table Accessed: NA;
* Table Updated : NA;
* Other         : NA;
****************************************************************/
void CConfig::RemoveComment(string &strLine)
{
    int iPos = strLine.find("#");

    if(string::npos != iPos)
    {
        strLine = strLine.substr(0, iPos);
    }
}

/****************************************************************
* Function Name : GetAllFields();
* Description   : 将当前配置文件中所有的域名输出到vecOut中;
* Calls         : NA;
* Called By     : NA;
* Input         : NA;
* Output        : vecOut: 域名数组;
* Return        : 0: 成功; 1: 失败;
* Table Accessed: NA;
* Table Updated : NA;
* Other         : NA;
****************************************************************/
int CConfig::GetAllFields(vector<string> &vecOut)
{
    vecOut.clear();
    if(!m_iIsInitOK)
    {
        PrintErr("The configuration object is not init yet");
        return 1;
    }

    for(int i=0; i<m_vecFields.size(); ++i)
    {
        vecOut.push_back(m_vecFields[i].strFieldName);
    }

    return 0;
}

/****************************************************************
* Function Name : GetAllItems();
* Description   : 获取指定域名下的所有配置项名;
* Calls         : NA;
* Called By     : NA;
* Input         : pszField: 域名;
* Output        : vecOut  : 输出结果;
* Return        : 0: 成功; 1: 失败;
* Table Accessed: NA;
* Table Updated : NA;
* Other         : NA;
****************************************************************/
int CConfig::GetAllItems(const char* pszField, vector<string> &vecOut)
{
    assert(pszField);
    vecOut.clear();

    if(!m_iIsInitOK)
    {
        PrintErr("The configuration object is not init yet");
        return 1;
    }

    int i = 0;
    for(i=0; i<m_vecFields.size(); ++i)
    {
        if(m_vecFields[i].strFieldName == pszField)
        {
            break;
        }
    }

    if(i == m_vecFields.size())
    {
        PrintErr("Can not find the field [" << pszField << "]");
        return 1;
    }

    int iPos = 0;
    for(iPos=0; iPos<m_vecFields[i].vecItems.size(); ++iPos)
    {
        vecOut.push_back(m_vecFields[i].vecItems[iPos].strItemName);
    }

    return 0;
}

/****************************************************************
* Function Name : GetItem();
* Description   : 获取指定域下指定变量的值;
* Calls         : NA;
* Called By     : NA;
* Input         : pszField: 域名; pszItem: 配置项名;
* Output        : pszOut: 输出结果;
* Return        : 0: 成功; 1: 失败;
* Table Accessed: NA;
* Table Updated : NA;
* Other         : NA;
****************************************************************/
int CConfig::GetItem(const char* pszField, const char* pszItem, char* pszOut)
{
    assert(pszField);
    assert(pszItem);

    if(!m_iIsInitOK)
    {
        PrintErr("The configuration object is not init yet");
        return 1;
    }

    int i = 0;
    for(i=0; i<m_vecFields.size(); ++i)
    {
        if(m_vecFields[i].strFieldName == pszField)
        {
            break;
        }
    }

    if(i == m_vecFields.size())
    {
        PrintErr("Can not find the field [" << pszField << "]");
        return 1;
    }

    int iPos = 0;
    for(iPos=0; iPos<m_vecFields[i].vecItems.size(); ++iPos)
    {
        if(m_vecFields[i].vecItems[iPos].strItemName == pszItem)
        {
            break;
        }
    }

    if(iPos == m_vecFields[i].vecItems.size())
    {
        PrintErr("Can not find the item [" << pszItem << "] under the field [" << pszField << "]");
        return 1;
    }

    sprintf(pszOut, "%s", m_vecFields[i].vecItems[iPos].strItemValue.c_str());

    return 0;
}

/****************************************************************
* Function Name : SetItem();
* Description   : 设置指定配置项的值;
* Calls         : NA;
* Called By     : NA;
* Input         : pszField: 域名; pszItem: 配置项名; pszValue: 配置项的值;
* Output        : NA;
* Return        : 0: 成功; 1: 失败;
* Table Accessed: NA;
* Table Updated : NA;
* Other         : NA;
****************************************************************/
int CConfig::SetItem(const char* pszField, const char* pszItem, const char* pszValue)
{
    assert(pszField);
    assert(pszItem);

    if(!m_iIsInitOK)
    {
        PrintErr("The configuration object is not init yet");
        return 1;
    }

    int i = 0;
    for(i=0; i<m_vecFields.size(); ++i)
    {
        if(m_vecFields[i].strFieldName == pszField)
        {
            break;
        }
    }

    if(i == m_vecFields.size())
    {
        PrintErr("Can not find the field [" << pszField << "]");
        return 1;
    }

    int iPos = 0;
    for(iPos=0; iPos<m_vecFields[i].vecItems.size(); ++iPos)
    {
        if(m_vecFields[i].vecItems[iPos].strItemName == pszItem)
        {
            break;
        }
    }

    if(iPos == m_vecFields[i].vecItems.size())
    {
        PrintErr("Can not find the item [" << pszItem << "] under the field [" << pszField << "]");
        return 1;
    }

    m_vecFields[i].vecItems[iPos].strItemValue = pszValue;

    return 0;
}

/****************************************************************
* Function Name : RefreshFile();
* Description   : 将当前内存中的配置文件的值写到文件中;
* Calls         : NA;
* Called By     : NA;
* Input         : NA;
* Output        : NA;
* Return        : 0: 成功; 1: 失败;
* Table Accessed: NA;
* Table Updated : NA;
* Other         : NA;
****************************************************************/
int CConfig::RefreshFile()
{
    if(!m_iIsInitOK)
    {
        PrintErr("The configuration object is not init yet");
        return 1;
    }

    int iPos1 = 0;
    int iPos2 = 0;
    int iLine = 0;
    int iRow  = 0;

    string strItemValue;
    string strOldValue;

    for(iPos1 = 0; iPos1 < m_vecFields.size(); ++iPos1)
    {
        for(iPos2 = 0; iPos2 < m_vecFields[iPos1].vecItems.size(); ++iPos2)
        {
            strItemValue = m_vecFields[iPos1].vecItems[iPos2].strItemValue;
            strOldValue  = m_vecFields[iPos1].vecItems[iPos2].strOldValue;
            iLine = m_vecFields[iPos1].vecItems[iPos2].iLine;
            iRow  = m_vecFields[iPos1].vecItems[iPos2].iPos;
            m_vecContent[iLine].replace(iRow, strOldValue.size(), strItemValue);
            m_vecFields[iPos1].vecItems[iPos2].strOldValue = strItemValue;
        }
    }

    ofstream ofs;
    string   strTmpFile = m_strCfgFile + ".tmp";

    ofs.open(strTmpFile.c_str());
    if(!ofs)
    {
        PrintErr("Fail to open the file: " << strTmpFile);
        return 1;
    }

    for(iPos1 = 0; iPos1 < m_vecContent.size(); ++iPos1)
    {
        ofs << m_vecContent[iPos1] << endl;
    }

    ofs.close();

    if(0 != rename(strTmpFile.c_str(), m_strCfgFile.c_str()))
    {
        PrintErr("Fail to rename the file [" << strTmpFile << "] to [" << m_strCfgFile << "]");
        return 1;
    }

    return 0;
}

/****************************************************************
* Function Name : ReloadCfgFile();
* Description   : 重新加载配置文件中的内容到内存中;
* Calls         : Init();
* Called By     : NA;
* Input         : NA;
* Output        : NA;
* Return        : 0: 成功; 1: 失败;
* Table Accessed: NA;
* Table Updated : NA;
* Other         : NA;
****************************************************************/
int CConfig::ReloadCfgFile()
{
    if(!m_iIsInitOK)
    {
        PrintErr("The configuration object is not init yet");
        return 1;
    }

    return Init(m_strCfgFile.c_str());
}

/****************************************************************
* Function Name : Print();
* Description   : 打印指定的配置文件的信息;
* Calls         : NA;
* Called By     : NA;
* Input         : NA;
* Output        : NA;
* Return        : 0: 成功; 1: 失败;
* Table Accessed: NA;
* Table Updated : NA;
* Other         : 如果没有指定域名和配置项名则打印所有配置项值;
****************************************************************/
int CConfig::Print(const char* pszField, const char* pszItem)
{
    if(!m_iIsInitOK)
    {
        PrintErr("The configuration object is not init yet");
        return 1;
    }

    PrintInfo("File: " << m_strCfgFile);
    PrintInfo("####################### BEGIN ###############################");
    int i;
    int j;

    for(i=0; i<m_vecFields.size(); ++i)
    {
        if(NULL != pszField)
        {
            if(pszField != m_vecFields[i].strFieldName)
            {
                continue;
            }
        }

        PrintInfo("[" << m_vecFields[i].strFieldName << "]");

        for(j=0; j<m_vecFields[i].vecItems.size(); ++j)
        {
            if(NULL != pszItem)
            {
                if(pszItem != m_vecFields[i].vecItems[j].strItemName)
                {
                    continue;
                }
            }

            PrintInfo("\t" << m_vecFields[i].vecItems[j].strItemName << "=" << \
                      m_vecFields[i].vecItems[j].strItemValue);
        }
    }

    PrintInfo("########################### END #############################");
}

//////////// Test code ///////////////

#if 0
#include <unistd.h>

int main()
{
    CConfig cfg;
    if(0 != cfg.Init("con.cfg"))
    {
        return 0;
    }

    cfg.Print();
    vector<string> vec;

    cfg.SetItem("system", "dir", "0000");
    cfg.RefreshFile();

    sleep(30);

    cfg.ReloadCfgFile();
    cfg.Print();
}
#endif

/********************************** END *******************************************/

