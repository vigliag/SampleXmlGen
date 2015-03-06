using pugi::xml_node;
using pugi::xml_document;
using std::string;

xml_document generateRoot(ElementDefinition root, vector<PoolDefinition> availablePools){
  DataPool dataPool(availablePools);
  xml_document doc = new xml_document();
  generate(root, doc, 1, dataPool, "");
  return doc;
}

void generate(ElementDefinition& en, xml_node parentNode, unsigned int amount, const DataPool& parentDataPool, const string prefix = "")
{
  DataPool ownDataPool = parentDataPool.provideDataPool(en.requests);
  for(int i = 0; i < amount; i++)
  {
    string childrenPrefix = prefix + en.name + std::to_string(i+1);
    xml_node currentNode = parentNode.append_child(en.name);
    if(en.isLeaf)
    {
      currentNode.append_child(pugi::node_pcdata).set_value(childrenPrefix);
    } 
    else 
    {
      for(OwnChildDefinition child : en.ownChildren)
      {
        generate(child.node, current_node, child.amount, dataPool, childrenPrefix);
      }
      for(DataPoolQuery query : en.childrenFromDataPool)
      {
        vector<xml_node> result = ownDataPool.query(child);
        for(xml_node r : result)
        {
          currentNode.append_copy(r);
        }
      }
    }
  }
}