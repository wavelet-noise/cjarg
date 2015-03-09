#include "database.h"
#include "sge/prefecences.h"
#include "sge/fielsystem.h"
#include <fstream>
#include "rapidjson/document.h"
#include <logic/map/scheme.h>

void database::registerBlock(const std::string &s, StaticBlock *b)
{
    if(block_pointer.find(s) != block_pointer.end())
        LOG(info) << "block " << s << " redefinition";
    block_db.push_back(std::unique_ptr<StaticBlock>(b));
    block_back_pointer[block_db.size() - 1] = s;
    block_pointer[s] = block_db.size() - 1;

    b->id = block_db.size() - 1;
    //b->full_id = s;
    //LOG(info) << "register \"" << s << "\" as " << block_db.size() - 1;
}

void database::registerItem(const std::string &s, StaticItem *i)
{
    if(item_pointer.find(s) != item_pointer.end())
        LOG(info) << "item " << s << " redefinition";
    item_db.push_back(std::unique_ptr<StaticItem>(i));
    item_back_pointer[item_db.size() - 1] = s;
    item_pointer[s] = item_db.size() - 1;

    i->id = item_db.size() - 1;
    //i->full_id = s;
    //LOG(info) << "register \"" << s << "\" as " << block_db.size() - 1;
}

void database::Load()
{
    std::vector<std::string> files;
    getFiles(Prefecences::Instance()->getJsonDir(), files);

    for(std::string file : files)
    {
        std::ifstream fs(Prefecences::Instance()->getJsonDir() + file);
        std::string buf;
        std::string all;
        all.reserve(65536);
        while(!fs.eof())
        {
            fs >> buf;
            all += buf;
        }

        LOG(info) << "parse " << file;
        rapidjson::Document d;
         d.Parse<0>(all.c_str());
        if(d.HasParseError())
        {
            LOG(error) << d.GetParseError();
        }

        int loaded = 0;
        if(d.IsArray())
        {
            for(int i=0; i < d.Size(); i++)
            {
                rapidjson::Value &val = d[i];
                if(!val.HasMember("type"))
                {
                    LOG(error) << "record #" << i+1 << " from " << file << " has no \"type\"";
                    continue;
                }
                std::string type = val["type"].GetString();

                // === parse part ===
                if(type == "block")
                {
                    if(!val.HasMember("id"))
                    {
                        LOG(error) << "record #" << i+1 << " from " << file << " has no \"id\"";
                        continue;
                    }

                    StaticBlock *b = new StaticBlock();
                    b->etalon = std::unique_ptr<Block>(new Block());
                    b->etalon->parts = std::unique_ptr<Dynamic>(new Dynamic());

                    std::string id = val["id"].GetString();

                    if(val.HasMember("transparent"))
                    {
                        b->transparent = val["transparent"].GetBool_();
                    }

                    if(val.HasMember("alltex") || val.HasMember("tex")) {
                        if(val.HasMember("alltex")) b->setTexture(val["alltex"].GetString());
                        else
                        if(val.HasMember("tex")) {
                            rapidjson::Value &arr = val["tex"];
                            for(int a = 0; a < arr.Size(); a++)
                                b->setTexture(static_cast<StaticBlock::SIDE>(a), arr[a].GetString());
                        } else
                            LOG(error) << "block " << id << " from " << file << " has no \"tex\" | \"alltex\"";
                    }

                    if(val.HasMember("parts")) {
                        rapidjson::Value &arr = val["parts"];
                        if(val["parts"].IsArray())
                        for(int a = 0; a < arr.Size(); a++)
                        {
                            rapidjson::Value &part = arr[a];
                            if(part.HasMember("type")) {

                                /* all agents must be written here */
                                CASTER(Chest)
                                CASTER(Furnance)
                                /*else here*/ LOG(error) << "block \"" << id << "\" agent #" << a + 1 << " has unknown \"type\"";
                            }
                            else
                                LOG(error) << "block \"" << id << "\" agent #" << a + 1 << " has no type";
                        }
                        else
                            LOG(error) << "block \"" << id << "\" parts is not valid agents array";
                    }

                    if(b->etalon->parts->agents.size() == 0)
                    {
                        b->etalon->parts = nullptr;
                        b->etalon = nullptr;
                    }

                    registerBlock(id, b);
                    loaded ++;
                }

                if(type == "item")
                {
                    if(!val.HasMember("id"))
                    {
                        LOG(error) << "record #" << i+1 << " from " << file << " has no \"id\"";
                        continue;
                    }

                    StaticItem *i = new StaticItem;
                    std::string id = val["id"].GetString();

                    registerItem(id, i);
                    loaded ++;
                }

                if(type == "scheme")
                {
                    Scheme s;
                    if(!s.deserialize(val))
                    {
                        LOG(error) << "record #" << i+1 << " from " << file << " broken, skipped";
                        continue;
                    }

                    scheme_db[s.type].push_back(s);
                }
            }
        } else
            LOG(error) << "not valid object array";
        LOG(info) << loaded << " loaded";
        LOG(info) << "---";
    }
}

database::database()
{

}

database::~database()
{
}

database *database::m_inst = nullptr;
