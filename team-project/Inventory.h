#pragma once
#include "Item.h"
class Inventory

{
public :
	std::vector<Item> items;
	void AddItem(const Item& item);
	void RemoveItem(const Item& item);
	void Save(std::ofstream& file);
	void Load(std::ifstream& file);
	

};

