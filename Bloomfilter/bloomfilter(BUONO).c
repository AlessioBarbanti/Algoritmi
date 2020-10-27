#include <stdio.h>
#include <stdlib.h>
#include "bloomfilter.h"

BloomFilter createBloomFilter(int k, int m)
{
	int* hashSeeds = (int*)malloc(sizeof(int) * k);
	if (!hashSeeds) {
		printf("Errore");
		exit(1);
	}
	for (int i = 1;i <= k;i++) {
		hashSeeds[i-1] = i;
	}

	bool* filter = (bool*) malloc (sizeof(bool));
	if (!filter) {
		printf("Errore");
		free(hashSeeds);
		exit(1);
	}
	for (int i = 0; i < m;i++) {
		filter[i] = false;
	}
	BloomFilter bf = { k, m, hashSeeds, filter };
	return bf;
}

void freeBloomFilter(BloomFilter bloomFilter)
{
	free(bloomFilter.hashSeeds);
	free(bloomFilter.filter);
}

void bfInsertion(BloomFilter* bloomFilter, const char* elem, int elemLen)
{
	int index = 0;
	while (index < bloomFilter->k) {
		int h = hashFunction(elem, elemLen, bloomFilter->hashSeeds[index], bloomFilter->m);
		bloomFilter->filter[h] = true;
		index++;
	}
}

void bfConstruction(BloomFilter* bloomFilter, const char** dataset, const int* datasetElemLen, int n)
{
	for (int i = 0; i < n;i++) {
		bfInsertion(bloomFilter, dataset[i], datasetElemLen[i]);
	}
}

bool bfSearch(BloomFilter bloomFilter, const char* elem, int elemLen)
{
	int i = 1;

	while (i <= bloomFilter.k) {
		if (bloomFilter.filter[hashFunction(elem, elemLen, bloomFilter.hashSeeds[i-1], bloomFilter.m)] == false){
			return false;
		}
		i++;
	}
	return true;
}

int countDatasetMembership(BloomFilter bloomFilter, const char** dataset, const int* datasetElemLen, int n)
{
	int counter = 0;
	for (int i = 0; i < n;i++) {
		if (bfSearch(bloomFilter, dataset[i], datasetElemLen[i])) {
			counter++;
		}
	}
	return counter;
}
