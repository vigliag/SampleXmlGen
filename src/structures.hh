#include <string>
#include <pugixml>

using pugi::xml_node;
using pugi::xml_document;
using std::string;

struct PoolDefininition {
  string resourceId;
  unsigned int amount;
  ElementDefinition* ed;
};

struct DataPoolQuery {
  string resourceId;
  unsigned int amount;
};

struct NewDataPoolRequest {
  string elementName;
  bool exclusiveData;
  unsigned int amount;
};

typedef PoolDataMap map<string, PoolDataInfo>;

struct PoolDataInfo {
  PoolDataInfo(xml_document& _data, bool _exclusive = false){
    data =  _data;
    exclusive = exclusive;
  }
  xml_document* data;
  bool exclusive;
};

class DataPool {
  public:
  DataPool(PoolDataMap availablePoolData);
  DataPool provideDataPool(vector<NewDataPoolRequest>);
  vector<xml_node> query(DataPoolQuery);
};

struct OwnChildDefinition {
  ElementDefinition node;
  unsigned int amount;
};

struct ElementDefinition {
  string name;
  bool isLeaf;
  vector<NewDataPoolRequest> poolRequests; //define how to create own dataPool, used by him and his descendants
  vector<OwnChildDefinition> ownChildren;
  vector<DataPoolQuery> childrenFromDataPool; //define how children are created querying the datapool
};


