#include <qing/mongo/define.hpp>
#include <qing/util/logger.hpp>
#include <qing/mongo/mongo_param_type.hpp>
#include <qing/mongo/mongo_struct_base.hpp>
#include <qing/mongo/mongo_conn.hpp>
using namespace std;
namespace qing
{
    namespace mongodb
    {
        MongoDBConn::MongoDBConn()
        {
        }

        MongoDBConn::~MongoDBConn()
        {
        }

        bool MongoDBConn::ConnectMongoDB(const std::string& sIp, int nPort, const std::string& sDbName)
        {
            std::string errMsg;
            mongo::HostAndPort hostandport(sIp, nPort);
            if (dbClientConn.connect(hostandport, errMsg))
            {
                m_sDbName = sDbName;
                return true;
            }
            else
            {
                QLOG_ERROR("ConnectMongoDB error [%s]", errMsg.c_str());
                return false;
            }
        }

        int MongoDBConn::ExecInsert(const std::string& sTableName, bson::bo *bsonObj)
        {
            dbClientConn.insert(m_sDbName + "." + sTableName, *bsonObj);
            return 1;
        }

        int MongoDBConn::ExecUpdate(const std::string& sTableName, mongo::Query query, bson::bo* bsonObj, bool updateall, bool upinsert)
        {
            //bson::bo o = BSON( "hello" << "world"<< "hello2" << "world2" );
            //for (int i=0; i<10; ++i)
            //{
            //	dbClientConn.insert("test.foo", o);

            //	std::string e = dbClientConn.getLastError();
            //	if( !e.empty() ) { 
            //		cout << "insert #1 failed: " << e << endl;
            //	}

            //	// make an index with a unique key constraint
            //	dbClientConn.ensureIndex("test.foo", BSON("hello"<<1<<"hello2"<<1), /*unique*/true);

            //	dbClientConn.insert("test.foo", o); // will cause a dup key error on "hello" field
            //}
            dbClientConn.update(m_sDbName + "." + sTableName, query, *bsonObj, upinsert, updateall);
            return 1;
        }

        int MongoDBConn::ExecDelete(const std::string& sTableName, mongo::Query query, bson::bo bsonObj)
        {
            dbClientConn.remove(m_sDbName + "." + sTableName, query, false);
            return 1;
        }

        int MongoDBConn::ExecQueryGetAllTable(const std::string& sTableName, BSON_VEC &bonsobjResultList, int* nListNum)
        {
            unsigned long long count = dbClientConn.count(m_sDbName + "." + sTableName);
            *nListNum = (int)count;
            //int tmp_id = 1020001;
            //mongo::Query que = QUERY( "id"<<tmp_id );
            //bson::bo res = dbClientConn.findOne( "Naruto_config.BuffEffect",que );
            //std::string bsons = res.toString();

            std::auto_ptr<mongo::DBClientCursor> cursor = dbClientConn.query(m_sDbName + "." + sTableName, mongo::Query());
            if (!cursor.get())
            {
                return 0;
            }

            while (cursor->more())
            {
                //cout << (cursor->next().toString()) << endl;
                bson::bo tmpBson;
                tmpBson = cursor->next().copy();
                bonsobjResultList.push_back(tmpBson);
            }
            //system("pause");
            return 1;
        }

        int MongoDBConn::ExecQueryFindOne(const std::string& sTableName, mongo::Query query, bson::bo* pBsonObj)
        {
            int nResult = 1;
            std::auto_ptr<mongo::DBClientCursor> cursor = dbClientConn.query(m_sDbName + "." + sTableName, query);
            if (!cursor.get())
            {
                return 0;
            }

            while (cursor->more())
            {
                *pBsonObj = cursor->next().copy();
                nResult = 0;
            }

            return nResult;
        }

        int MongoDBConn::ExecQueryFindAll(const std::string& sTableName, mongo::Query query, BSON_VEC &bonsobjResultList, int* nListNum)
        {
            unsigned long long count = 0;
            std::auto_ptr<mongo::DBClientCursor> cursor = dbClientConn.query(m_sDbName + "." + sTableName, query);

            if (!cursor.get())
            {
                return 0;
            }

            while (cursor->more())
            {
                count++;
                //cout << (cursor->next().toString()) << endl;
                bson::bo tmpBson;
                tmpBson = cursor->next().copy();
                bonsobjResultList.push_back(tmpBson);
            }
            *nListNum = (int)count;
            return 0;
        }

        unsigned long long MongoDBConn::Count(const std::string& sTableName, mongo::Query query)
        {
            unsigned long long n = dbClientConn.count(m_sDbName + "." + sTableName, query.obj);
            return n;
        }

        void MongoDBConn::SaveStructToMongoDB(mongo::Query query, SqlStructBase* sqlstruct, bool updateall)
        {
            bson::bo boStruct;
            sqlstruct->GetUniqueKey();
            sqlstruct->StructToBson(boStruct);
            ExecUpdate(sqlstruct->GetTableName(), query, &boStruct, updateall);
        }

        void MongoDBConn::SaveStructToMongoDB(mongo::Query query, bson::bo* bsonObj, SqlStructBase* sqlstruct, bool updateall)
        {
            ExecUpdate(sqlstruct->GetTableName(), query, bsonObj, updateall, false);
        }

        void MongoDBConn::DeleteStructToMongoDB(SqlStructBase* sqlstruct, mongo::Query query)
        {
            bson::bo boStruct;
            ExecDelete(sqlstruct->GetTableName(), query, boStruct);
        }

        void MongoDBConn::InsertStructToMongoDB(mongo::Query query, SqlStructBase* sqlstruct)
        {
            bson::bo boStruct;
            sqlstruct->GetUniqueKey();
            sqlstruct->StructToBson(boStruct);
            ExecInsert(sqlstruct->GetTableName(), &boStruct);
        }

        bool MongoDBConn::GetStructFromMongoDB(mongo::Query query, SqlStructBase* sqlstruct)
        {
            bson::bo bsonUser;
            int nResult = ExecQueryFindOne(sqlstruct->GetTableName(), query, &bsonUser);
            if (nResult == 0)
            {
                sqlstruct->BsonToStruct(bsonUser);
                return true;
            }
            return false;
        }

        bool MongoDBConn::GetFullStructFromMongoDB(mongo::Query query, SqlStructBase*sqlstruct, BSON_VEC& vec_bo)
        {
            BSON_VEC vbson;
            int nListNum = 0;

            ExecQueryFindAll(sqlstruct->GetTableName(), query, vbson, &nListNum);
            if (nListNum <= 0)
            {
                return false;
            }
            BSON_VEC::iterator itbson = vbson.begin();
            while (itbson != vbson.end())
            {
                vec_bo.push_back((*itbson));
                itbson++;
            }
            return true;
        }

        unsigned long long MongoDBConn::GetCountFromMongoDB(mongo::Query query, SqlStructBase*sqlstruct)
        {
            BSON_VEC vbson;
            unsigned long long num = Count(sqlstruct->GetTableName(), query);

            return num;
        }

        bool MongoDBConn::GetLimitStructFromMongoDB(mongo::Query query, SqlStructBase*sqlstruct, BSON_VEC& vec_bo, int limit_cnt)
        {
            BSON_VEC vbson;
            int nListNum = 0;

            ExecQueryFindLimit(sqlstruct->GetTableName(), query, vbson, &nListNum, limit_cnt);
            if (nListNum <= 0)
            {
                return false;
            }
            BSON_VEC::iterator itbson = vbson.begin();
            while (itbson != vbson.end())
            {
                vec_bo.push_back((*itbson));
                itbson++;
            }
            return true;
        }

        int	MongoDBConn::ExecQueryFindLimit(const std::string& sTableName, mongo::Query query, BSON_VEC &bonsobjResultList, int* nListNum, int limit_cnt)
        {
            unsigned long long count = 0;
            std::auto_ptr<mongo::DBClientCursor> cursor = dbClientConn.query(m_sDbName + "." + sTableName, query, limit_cnt);

            if (!cursor.get())
            {
                return 0;
            }

            while (cursor->more())
            {
                count++;
                //cout << (cursor->next().toString()) << endl;
                bson::bo tmpBson;
                tmpBson = cursor->next().copy();
                bonsobjResultList.push_back(tmpBson);
            }
            *nListNum = (int)count;
            return 0;
        }

        /*
        EXAMPLE::     !!!

        BSON_VEC bonsobjResultList;
        int nListNum;
        mongo::Query query;
        g_dbconn->ExecQueryFindGroupBy("userbaseinfo", query, bonsobjResultList, &nListNum, GTYPE_COUNT, "nUserSex",  "nUserLevel");
        */

        int MongoDBConn::ExecQueryFindGroupBy(const std::string& sTableName, mongo::Query query, BSON_VEC &bonsobjResultList, int* nListNum, GROUPBY_TYPE gtype, std::string groupbystr, std::string keystr)
        {
            const std::string ns = m_sDbName + "." + sTableName;
            std::string tmpmapstr = "function";
            tmpmapstr = tmpmapstr + " Map(){ emit(this." + groupbystr + ", {" + keystr + ": this." + keystr + "});}";
            const char *map = tmpmapstr.c_str();
            std::string tmpreducestr = "function";
            tmpreducestr = tmpreducestr + " Reduce(key, values) {	var reduced = {" + keystr + ":0};  values.forEach(function(val) { reduced." + keystr;
            switch (gtype)
            {
            case GTYPE_COUNT:
            {
                tmpreducestr += " += 1;}); return reduced;}";
                break;
            }
            case GTYPE_SUM:
            {
                tmpreducestr += " += val." + keystr + ";}); return reduced;}";
                break;
            }
            default:
                break;
            }
            const char* reduce = tmpreducestr.c_str();
            const std::string outcoll = ns + ".out";

            mongo::BSONObj out;
            out = dbClientConn.mapreduce(ns, map, reduce, mongo::BSONObj());
            MONGO_PRINT(out);
            if (!out["results"].isNull())
            {
                mongo::BSONElement ele = out["results"];
                std::vector<mongo::BSONElement> vec_ele = ele.Array();
                for (int i = 0; i<(int)vec_ele.size(); ++i)
                {
                    mongo::BSONObj tmpbo = vec_ele[i].Obj();
                    bson::bob bbtmp;
                    bbtmp.append(groupbystr, tmpbo["_id"].Number());
                    bbtmp.append(keystr, tmpbo["value"][keystr].Number());
                    bson::bo boStruct = bbtmp.obj();
                    bonsobjResultList.push_back(boStruct);
                }
            }
            return 0;
        }


        mongo::DBClientConnection& MongoDBConn::GetDBConn()
        {
            return dbClientConn;
        }

        std::string&  MongoDBConn::GetDBName()
        {
            return m_sDbName;
        }

        //void MongoDBConn::EnsureIndex(std::string sTableName)
        //{
        //    std::string m_sDbName = GetDBName();
        //    GetDBConn().createIndex(m_sDbName + "." + sTableName, BSON("lUserId" << 1));
        //}
    }
}
