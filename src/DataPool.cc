#include "structures.hh"

DataPool:DataPool(PoolDataMap availablePoolData)
{ 
	PoolDataMap& internalMap = availablePoolData;
}

DataPool:provideDataPool(vector<NewDataPoolRequest> requests)
{
	PoolDataMap newMap = internalMap; //copy (I hope)
	for(NewDataPoolRequest r : requests)
	{
		//extract amount from old map's data following oldmap's policy (removing if needed)
		//assign new map policy based on request;
		PoolDataInfo pdi = internalMap[r.resourceId];
		xml_document* newData = new xml_document();
		
		for(int i=0; i<r.amount; i++)
		{
			xml_node currNode = pdi.data->first_child();
			if(!currNode) throw "Not enough elements in " + r.resourceId;
			newData.append_copy(currNode);
			if(exclusive)
				pdi.data->remove_child(currNode);
		}

		newMap[r.resourceId] = PoolDataInfo(newData, r.exclusiveData);
	}
	return DataPool(newMap);
}
