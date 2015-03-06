using pugi::xml_node;
using pugi::xml_document;
using std::string;

struct PoolDefininition {
  string resourceId;
  unsigned int amount;
  xml_document* data;
}

struct OwnChildDefinition {
  ElementDefinition node;
  unsigned int amount;
}

struct ElementDefinition {
  string name;
  bool isLeaf;
  vector<NewDataPoolRequest> poolRequests; //define how to create own dataPool, used by him and his descendants
  vector<OwnChildDefinition> ownChildren;
  vector<DataPoolQuery> childrenFromDataPool; //define how children are created querying the datapool
}

struct DataPoolQuery {
  string resourceId;
  unsigned int amount;
}

struct NewDataPoolRequest {
  string elementName;
  bool esclusiveData;
  unsigned int amount;
}

class DataPool {
  public:
  DataPool(vector<PoolDefinitions>);
  DataPool provideDataPool(vector<NewDataPoolRequest>);
  vector<xml_node> query(DataPoolQuery);
}