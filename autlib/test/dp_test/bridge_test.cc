/* ***********************************************************************

  > File Name: link.cc
  > Author: Aut
  > Mail: aut@taomee.com 
  > Created Time: 2017年05月05日 星期五 14时49分09秒

 ********************************************************************** */

#include <iostream>
#include <string>

class DB
{
public:
    virtual std::string readData(std::string SQL) { return std::string(""); }
    virtual ~DB() {}
};

class Format
{
public:
    void setDB(DB *pDb) { m_db = pDb; }
    virtual void exportToFile(std::string file_name, std::string SQL) {}
    virtual ~Format() {}

protected:
    DB *m_db;
};

class Mysql : public DB
{
public:
    std::string readData(std::string SQL)
    {
        std::cout << "Read from MySQL, SQL: " << SQL << std::endl;
        return std::string("");
    }
};

class Oracle : public DB
{
public:
    std::string readData(std::string SQL)
    {
        std::cout << "Read from Oracle, SQL: " << SQL << std::endl;
        return std::string("");
    }
};

class TxtFormat : public Format
{
public:
    void exportToFile(std::string file_name, std::string SQL)
    {
        m_db->readData(SQL);
        std::cout << "Export to txt file: " << file_name << std::endl;
    }
};

class PdfFormat : public Format
{
public:
    void exportToFile(std::string file_name, std::string SQL)
    {
        m_db->readData(SQL);
        std::cout << "Export to pdf file: " << file_name << std::endl;
    }
};

int main(int argc, const char* argv[])
{
    Format *pFormat = new PdfFormat;
    DB *pDb = new Oracle;
    pFormat->setDB(pDb);
    pFormat->exportToFile("da.pdf", "select * from D.A");
    pFormat = new TxtFormat;
    pDb = new Mysql;
    pFormat->setDB(pDb);
    pFormat->exportToFile("db.txt", "select * from D.B");
    delete pFormat;
    delete pDb;

    return 0;
}
