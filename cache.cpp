#include "cache.h"

Cache::Cache(uint32_t cacheSize, uint32_t blockSize, int nWays, bool debug){

    // Cache configuration
    this->cacheSize = cacheSize;
    this->blockSize = blockSize;
    this->nWays = nWays;

    // Cache stats
    this->hits = 0;
    this->totalAccesses = 0;
    this->debug = debug;

    // Cache number of sets
    // this->sets = 16;  // Currently hard coded
	
	this->sets = cacheSize/(this->blockSize*this->nWays);

    // Address field bits
    this->offsetBits = log2(blockSize);
    //this->indexBits = 4;  // Currently hard coded. 
	this->indexBits = log2(this->sets);
    this->tagBits = ADDRESS_LENGTH - this->indexBits - this->offsetBits;
	for (int i=0; i<sets; i++)
	    {
	        vector<uint32_t> ct;
	        vector<uint32_t> lt;
	        for (int j =0; j < this->nWays; j++)
        
	        {
	            lt.push_back(-1);
	            ct.push_back(-1);
	        }
	        this->cache.push_back(ct);
	        this->lru.push_back(lt);
	    }

    // Do not change the following print statements
    printf("------------------------------\n");
    printf("Cache size: %" PRIu32 " Bytes\nBlock Size: %" PRIu32 " Bytes\nSets: %d\nWays: %d\nTag bits: %d\nIndex bits: %d\nOffset bits: %d\n", 
            this->cacheSize, this->blockSize, this->sets, this->nWays, this->tagBits, this->indexBits, this->offsetBits);
    printf("------------------------------\n");
}

bool Cache::accessCache(string address, int addressOffset){
    // Get address fields
    address_t addr = this->parseAddress(address, addressOffset);

    // Check for cache hit
    //bool hit = (cache[addr.index] == addr.tag);
	int ind_hit = 0;
	bool hit = cache[addr.index][ind_hit] == addr.tag;
    
    while (ind_hit < this->nWays){
        if (cache[addr.index][ind_hit] == addr.tag)
		{
            hit = true;
            break;
        }
        ind_hit++;
    }
    if(hit){
        // Cache hit. Yay!
		for (int i = 0; i < this->nWays; i++)
		{
            if ((lru[addr.index][i] != -1) && (i != ind_hit) && (lru[addr.index][ind_hit] > lru[addr.index][i]))
                lru[addr.index][i]++;
        }
        lru[addr.index][ind_hit] = 0;
    }
    else // If miss, then load cache block
    {
        
		int ind_miss = 0;
		//cache[addr.index] = addr.tag; 
		
        while (ind_miss < this->nWays)
		{
            if (lru[addr.index][ind_miss] == -1)
			{
                lru[addr.index][ind_miss] = 0;
				cache[addr.index][ind_miss] = addr.tag;
                
                for (int i=0; i< ind_miss; i++)
				{
                    lru[addr.index][i]++;
                }
                break;
            }
            ind_miss++;
        }
        if (ind_miss == this->nWays)
		{
            ind_miss = 0;
            while (ind_miss < this->nWays)
			{
                if (lru[addr.index][ind_miss] == this->nWays-1)
				{
                    lru[addr.index][ind_miss] = 0;
					cache[addr.index][ind_miss] = addr.tag;                    
                } 
                else 
                    lru[addr.index][ind_miss]++;
                
                ind_miss++;
            }
        }
    }

    // Update stats
    if(hit)
      this->hits += 1;
    this->totalAccesses += 1;

    return hit;
}



// DO NOT CHANGE FILE BELOW THIS LINE

address_t Cache::parseAddress(string address, int addressOffset){
    istringstream converter(address);
    uint64_t conversion;
    converter >> std::hex >> conversion;

    // Add the addressOffset to the address
    conversion += addressOffset;

    // Truncate to 32-bit address
    uint32_t conversion32;
    conversion32 = (uint32_t)(conversion);

    /*calculate the offset */ 
    unsigned int mask = (1 << this->offsetBits) - 1; 
    uint32_t offset = conversion32 & mask;

    /*calculate the index */
    mask = (1 << this->indexBits) - 1;
    uint32_t index = (conversion32 >> this->offsetBits) & mask; 

    /*calculate the tag */
    uint32_t tag = conversion32 >> (this->offsetBits + this->indexBits);

    if(this->debug){
        printf("Address: %s\n",address.c_str());
        printf("Offset: %d\n",addressOffset);
        printf("address + offest: %lu\n",(unsigned long)conversion32);
        printf("tag: %" PRIu32 "\n",(uint32_t)tag);
        printf("index: %" PRIu32 "\n",(uint32_t)index);   
        printf("offset: %" PRIu32 "\n",(uint32_t)offset);

    }  

    address_t parts = { (uint32_t)tag, (uint32_t)index, (uint32_t)offset} ;

    return parts;
}
void Cache::printStats(){ 
    if(this->totalAccesses == 0){
        printf("Error, Cannot get rate \n");
    }
    else{
        printf("Hit rate: %.2f%%\n", 100.00 * (double)(this->hits)/this->totalAccesses);
        printf("Miss rate: %.2f%%\n", 100.00 * (1 - (double)(this->hits)/this->totalAccesses));
    } 
    printf("Hits: %d\n", this->hits);
    printf("total: %d\n",this->totalAccesses);
    printf("------------------------------\n\n"); 
}
