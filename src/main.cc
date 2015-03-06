#include "structures.hh"

int main(){
	//1) parse input file and create elementDefinition tree
	// TODO
	//
	
	string rootElementId;
	vector<PoolDefinition> poolDefinitions;
	//

	//2) populate data for each pooldefinition
	PoolDataMap availablePoolData;
	for(PoolDefinition pd : poolDefinitions)
	{
		availablePoolData[pd.resourceId] =
			PoolDataInfo(generateRoot(pd.ed, pd.amount, availablePoolData));
	}

	availablePoolData[rootElementId].data->save(std::cout);

}