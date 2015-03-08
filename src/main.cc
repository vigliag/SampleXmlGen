#include "structures.hh"

int main(){
	//1) parse input file and obtain an AST
	//
	//2) use the AST to create an elementDefinition tree
	// TODO
	//
	
	string rootElementId;
	vector<PoolDefinition> poolDefinitions;
	//

	//3) populate data for each pooldefinition
	PoolDataMap availablePoolData;
	for(PoolDefinition pd : poolDefinitions)
	{
		availablePoolData[pd.resourceId] =
			PoolDataInfo(generateRoot(pd.ed, pd.amount, availablePoolData));
	}

	availablePoolData[rootElementId].data->save(std::cout);

}